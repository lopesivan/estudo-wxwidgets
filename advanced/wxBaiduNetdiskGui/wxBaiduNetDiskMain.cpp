/***************************************************************
 * Name:      wxBaiduNetDiskMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2024-10-27
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "icon1.xpm"
#include "icon3.xpm"
#include "wxBaiduNetDiskMain.h"
#include <map>
#include <wx/filename.h>
#include <wx/txtstrm.h>

// helper functions
enum wxbuildinfoformat { short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format) {
  wxString wxbuild(wxVERSION_STRING);

  if (format == long_f) {
#if defined(__WXMSW__)
    wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
    wxbuild << _T("-Mac");
#elif defined(__UNIX__)
    wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
    wxbuild << _T("-Unicode build");
#else
    wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
  }

  return wxbuild;
}

BEGIN_EVENT_TABLE(wxBaiduNetDiskFrame, wxFrame)
EVT_CLOSE(wxBaiduNetDiskFrame::OnClose)
EVT_MENU(idMenuQuit, wxBaiduNetDiskFrame::OnQuit)
EVT_MENU(idMenuAbout, wxBaiduNetDiskFrame::OnAbout)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY, wxBaiduNetDiskFrame::OnListItemActived)
EVT_LIST_ITEM_CHECKED(wxID_ANY, wxBaiduNetDiskFrame::OnListItemChecked)
EVT_LIST_ITEM_UNCHECKED(wxID_ANY, wxBaiduNetDiskFrame::OnListItemUnChecked)
EVT_BUTTON(id_btn_dir_prev, wxBaiduNetDiskFrame::OnBtnDirectoryUp)
EVT_BUTTON(id_btn_download, wxBaiduNetDiskFrame::OnBtnDownload)
EVT_BUTTON(id_btn_upload, wxBaiduNetDiskFrame::OnBtnUploadFile)
EVT_BUTTON(id_btn_upload_dir, wxBaiduNetDiskFrame::OnBtnUploadDir)
EVT_BUTTON(id_btn_mkdir, wxBaiduNetDiskFrame::OnBtnMkdir)
EVT_BUTTON(id_btn_rm, wxBaiduNetDiskFrame::OnBtnRm)
EVT_CHECKBOX(id_btn_sel_all, wxBaiduNetDiskFrame::OnBtnSelectAll)
END_EVENT_TABLE()

wxBaiduNetDiskFrame::wxBaiduNetDiskFrame(wxFrame *frame, const wxString &title)
    : wxFrame(frame, -1, title) {
  m_pcsRespProcessor[wxT("ls")] = std::bind(
      &wxBaiduNetDiskFrame::ProcessNetdiskRespLs, this, std::placeholders::_1);
  m_pcsRespProcessor[wxT("pwd")] = std::bind(
      &wxBaiduNetDiskFrame::ProcessNetdiskRespPwd, this, std::placeholders::_1);

#if wxUSE_MENUS
  // create a menu bar
  wxMenuBar *mbar = new wxMenuBar();
  wxMenu *fileMenu = new wxMenu(_T(""));
  fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
  mbar->Append(fileMenu, _("&File"));

  wxMenu *helpMenu = new wxMenu(_T(""));
  helpMenu->Append(idMenuAbout, _("&About\tF1"),
                   _("Show info about this application"));
  mbar->Append(helpMenu, _("&Help"));

  SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
  // create a status bar with some information about the used wxWidgets version
  CreateStatusBar(2);
  SetStatusText(_("Hello Code::Blocks user!"), 0);
  SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

  m_imagesSmall.push_back(wxIcon(icon1_xpm));
  m_imagesSmall.push_back(wxIcon(icon3_xpm));

  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

  // Path
  wxBoxSizer *pathSizer = new wxBoxSizer(wxHORIZONTAL);
  pathSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Current Path:")));
  txt_path = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                            wxDefaultSize, wxTE_READONLY);
  btn_path_prev = new wxButton(this, id_btn_dir_prev, wxT("Up One Level"));
  btn_download = new wxButton(this, id_btn_download, wxT("Download"));
  btn_upload = new wxButton(this, id_btn_upload, wxT("Upload File"));
  btn_upload_dir =
      new wxButton(this, id_btn_upload_dir, wxT("Upload Directory"));
  btn_mkdir = new wxButton(this, id_btn_mkdir, wxT("Create Directory"));
  btn_rm = new wxButton(this, id_btn_rm, wxT("Delete Item"));
  chk_sel_all = new wxCheckBox(this, id_btn_sel_all, wxT(""));
  pathSizer->Add(txt_path, 1, wxRIGHT, 1);
  topSizer->Add(pathSizer, 0, wxEXPAND | wxALL, 5);

  wxBoxSizer *btn_sizer = new wxBoxSizer(wxHORIZONTAL);
  btn_sizer->Add(chk_sel_all, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL,
                 12);
  btn_sizer->Add(btn_path_prev);
  btn_sizer->Add(btn_download);
  btn_sizer->Add(btn_upload);
  btn_sizer->Add(btn_upload_dir);
  btn_sizer->Add(btn_mkdir);
  btn_sizer->Add(btn_rm);

  topSizer->Add(btn_sizer);

  m_logWindow = new wxTextCtrl(
      this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
      wxTE_READONLY | wxTE_MULTILINE | wxSUNKEN_BORDER);

  m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_logWindow));

  RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);

  topSizer->Add(m_listCtrl, 1, wxEXPAND, 1);
  topSizer->Add(m_logWindow, 0, wxEXPAND);

  ProcessNetdiskLs();

  SetMinClientSize(wxSize(1200, 760));

  SetSizer(topSizer);
  topSizer->Fit(this);
}

void wxBaiduNetDiskFrame::RecreateList(long flags, bool withText) {
  m_listCtrl = new MyListCtrl(this, LIST_CTRL, wxDefaultPosition, wxDefaultSize,
                              flags | wxBORDER_THEME | wxLC_EDIT_LABELS);

  m_listCtrl->EnableCheckBoxes(true);

  switch (flags & wxLC_MASK_TYPE) {
  case wxLC_REPORT:
    InitWithReportItems();
    break;

  default:
    wxFAIL_MSG("unknown listctrl mode");
  }

  m_logWindow->Clear();
}

void wxBaiduNetDiskFrame::InitWithReportItems() {
  m_listCtrl->SetSmallImages(m_imagesSmall);

  wxListItem itemCol;
  itemCol.SetText(wxT(""));
  itemCol.SetImage(-1);
  itemCol.SetAlign(wxLIST_FORMAT_CENTRE);
  m_listCtrl->InsertColumn(0, itemCol);
  m_listCtrl->SetColumnWidth(0, 40);

  itemCol.SetText(wxT("File Name"));
  itemCol.SetAlign(wxLIST_FORMAT_LEFT);
  m_listCtrl->InsertColumn(1, itemCol);
  m_listCtrl->SetColumnWidth(1, 400);

  itemCol.SetText(wxT("Modified Time"));
  itemCol.SetImage(-1);
  itemCol.SetAlign(wxLIST_FORMAT_CENTRE);
  m_listCtrl->InsertColumn(2, itemCol);

  itemCol.SetText(wxT("Type"));
  itemCol.SetImage(-1);
  m_listCtrl->InsertColumn(3, itemCol);

  itemCol.SetText(wxT("Size"));
  itemCol.SetImage(-1);
  m_listCtrl->InsertColumn(4, itemCol);

  m_listCtrl->SetColumnWidth(0, 40);
  m_listCtrl->SetColumnWidth(1, 440);
  m_listCtrl->SetColumnWidth(2, 200);
  m_listCtrl->SetColumnWidth(3, 80);
}

wxBaiduNetDiskFrame::~wxBaiduNetDiskFrame() {}

void wxBaiduNetDiskFrame::OnClose(wxCloseEvent &event) { Destroy(); }

void wxBaiduNetDiskFrame::OnQuit(wxCommandEvent &event) { Destroy(); }

void wxBaiduNetDiskFrame::OnAbout(wxCommandEvent &event) {
  wxString msg = wxbuildinfo(long_f);
  wxMessageBox(msg, _("Welcome to..."));
}

static bool MakeSurePwdResp(const wxString &resp) {
  return resp.Length() > 0 && resp.at(0) == '/';
}

static bool MakeSureLsResp(const wxString &resp) {
  return resp.Length() > 1 &&
         resp.Find(wxT("Current directory: ")) != wxNOT_FOUND;
}

typedef bool (*PFCheckRespStr)(const wxString &);

std::map<wxString, PFCheckRespStr> RespChecker = {{wxT("pd"), MakeSurePwdResp},
                                                  {wxT("ls"), MakeSureLsResp}};

bool wxBaiduNetDiskFrame::CheckNetdiskResp(const wxString &cmd,
                                           const wxString &resp) {
  auto it = RespChecker.find(cmd);
  if (it != RespChecker.end()) {
    return it->second(resp);
  }
  return false;
}

bool wxBaiduNetDiskFrame::ExecuteNetdiskCmd(const wxString &cmd,
                                            wxString &resp) {
  wxProcess *process{new wxProcess(wxPROCESS_REDIRECT)};
  wxExecute(wxT("baidupcs ") + cmd, wxEXEC_ASYNC, process);
  wxInputStream *iStream{process->GetInputStream()};
  wxTextInputStream tiStream{*iStream};
  resp = tiStream.ReadLine();
  while (!iStream->Eof()) {
    resp.Append(wxT("\n")).Append(tiStream.ReadLine());
  }
  return CheckNetdiskResp(cmd, resp);
}

bool wxBaiduNetDiskFrame::ProcessNetdiskRespLs(const wxString &resp) {
  const wxString start_str = wxT("Current directory: ");
  size_t pos = resp.Find(start_str);

  if (pos != (size_t)wxNOT_FOUND) {
    pos += start_str.Length();
    do {
      auto ch = resp.at(pos);
      if (ch == ' ' || ch == '\t') {
        pos++;
      } else {
        break;
      }
    } while (pos < resp.Length());
    auto pos2 = pos;
    while (pos2 < resp.Length()) {
      auto ch = resp.at(pos2);
      if (ch == ' ' || ch == '\t' || ch == '\n') {
        break;
      }
      pos2++;
    }
    m_listCtrl->DeleteAllItems();
    if (pos != pos2) {
      auto cur_path = resp.Mid(pos, pos2 - pos);
      txt_path->SetValue(cur_path);
      pos = resp.find_first_of('\n', pos2);
      if (pos != (size_t)wxNOT_FOUND) {
        pos++;
        while (pos < resp.Length()) {
          wxString line;
          auto pos2 = resp.find_first_of('\n', pos);
          if (pos2 != (size_t)wxNOT_FOUND) {
            line = resp.Mid(pos, pos2 - pos);
          } else {
            line = resp.Mid(pos);
          }
          InsertNewFileItem(line);
          pos = pos2 + 1;
        }
      }
    }
  }
  return false;
}

bool wxBaiduNetDiskFrame::InsertNewFileItem(const wxString &line) {
  size_t pos = 0;
  while (pos < line.Length()) {
    auto ch = line.at(pos);
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
      pos++;
    } else {
      break;
    }
  }
  if (pos < line.Length()) {
    auto ch = line.at(pos);
    if (ch >= '0' && ch <= '9') {
      auto pos2 = pos;
      while (pos2 < line.Length() &&
             (line.at(pos2) >= '0' && line.at(pos2) <= '9'))
        pos2++;
      if (pos2 < line.Length()) {
        wxString index = line.Mid(pos, pos2 - pos);
        pos = line.find_first_not_of(' ', pos2);
        if (pos != (size_t)wxNOT_FOUND) {
          pos2 = line.find_first_of(' ', pos + 1);
          if (pos2 != (size_t)wxNOT_FOUND) {
            wxString size = line.Mid(pos, pos2 - pos);

            pos = line.find_first_not_of(' ', pos2 + 1);
            if (pos != (size_t)wxNOT_FOUND) {
              pos2 = line.find_first_of(' ', pos + 1);
              if (pos2 != (size_t)wxNOT_FOUND) {
                pos2 = line.find_first_not_of(' ', pos2 + 1);
                if (pos2 != (size_t)wxNOT_FOUND) {
                  pos2 = line.find_first_of(' ', pos2 + 1);
                  wxString date = line.Mid(pos, pos2 - pos);

                  pos = line.find_first_not_of(' ', pos2 + 1);
                  if (pos != (size_t)wxNOT_FOUND) {
                    wxString name;
                    pos2 = line.find_first_of(' ', pos + 1);
                    if (pos2 == (size_t)wxNOT_FOUND) {
                      name = line.Mid(pos, pos2 - pos);
                    } else {
                      pos2 = line.find_first_of('\n', pos2 + 1);
                      if (pos2 != (size_t)wxNOT_FOUND) {
                        name = line.Mid(pos, pos2 - pos);
                      } else {
                        name = line.Mid(pos);
                      }
                    }
                    int icon_idx = 0;
                    name.Trim();
                    wxString type = wxT("File");
                    if (size.Cmp(wxT("-")) == 0) {
                      icon_idx = 1;
                      type = wxT("Directory");

                      if (name.at(name.Length() - 1) == '/') {
                        name = name.Left(name.Length() - 1);
                      }
                    }
                    auto item_idx = m_listCtrl->GetItemCount();
                    long new_id = m_listCtrl->InsertItem(item_idx, name, 1);
                    m_listCtrl->SetItem(new_id, 1, name);
                    m_listCtrl->SetItem(new_id, 2, date);
                    m_listCtrl->SetItem(new_id, 3, type);
                    m_listCtrl->SetItem(new_id, 4, size);
                    m_listCtrl->SetItemImage(new_id, icon_idx, 1);
                    m_listCtrl->SetItemData(new_id, icon_idx);

                    wxListItem info;
                    info.SetImage(icon_idx);
                    info.SetId(new_id);
                    info.SetColumn(1);
                    m_listCtrl->SetItem(info);

                    return true;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return false;
}

bool wxBaiduNetDiskFrame::ProcessNetdiskRespPwd(const wxString &resp) {
  return false;
}

bool wxBaiduNetDiskFrame::ProcessNetdiskResp(const wxString &cmd,
                                             const wxString &resp) {
  auto it = m_pcsRespProcessor.find(cmd);
  if (it != m_pcsRespProcessor.end()) {
    return it->second(resp);
  }
  return false;
}

void wxBaiduNetDiskFrame::ProcessNetdiskLs() {
  wxString resp;
  wxString cmd = wxT("ls");
  bool ret = ExecuteNetdiskCmd(cmd, resp);
  if (ret == true) {
    ProcessNetdiskResp(cmd, resp);
  }
  checked_items_cnt = 0;
}

void wxBaiduNetDiskFrame::OnListItemActived(wxListEvent &event) {
  auto item = event.GetItem();
  wxString name = item.GetText();
  long icon_idx = item.GetData();
  if (icon_idx == 1) {
    wxString new_cmd = wxT("cd ") + txt_path->GetValue() + wxT("/") + name;
    wxString resp;
    ExecuteNetdiskCmd(new_cmd, resp);
    ProcessNetdiskLs();
  } else {
    wxString cmd = wxT("d ") + name;
    wxString resp;
    ExecuteNetdiskCmd(cmd, resp);
    if (resp.Find(wxT("Download completed")) != wxNOT_FOUND) {
      wxMessageBox(wxT("Download complete, file saved in download directory"),
                   wxT("Success"));
    } else {
      wxMessageBox(wxT("Download failed, please check if disk is available or "
                       "current network!"),
                   wxT("Failed"), wxYES | wxICON_ERROR);
    }
  }
}

void wxBaiduNetDiskFrame::OnBtnDirectoryUp(wxCommandEvent &event) {
  wxString resp;
  wxString cmd = wxT("cd ..");
  ExecuteNetdiskCmd("cd ..", resp);
  ProcessNetdiskLs();
}

void wxBaiduNetDiskFrame::OnBtnDownload(wxCommandEvent &event) {
  int success = 0;
  auto n = m_listCtrl->GetItemCount();
  for (int i = 0; i < n; ++i) {
    if (m_listCtrl->IsItemChecked(i)) {
      success += (DownloadItem(i) == true ? 1 : 0);
    }
  }
  wxString msg = wxString::Format(
      wxT("Task completed, %d items successfully downloaded"), success);
  wxMessageBox(msg);
}

bool wxBaiduNetDiskFrame::DownloadItem(long i) {
  wxListItem currentItem;
  currentItem.SetId(i);
  if (m_listCtrl->GetItem(currentItem)) {
    wxString name = currentItem.GetText();
    wxString cmd = wxT("d \"") + name + wxT("\"");
    wxString resp;
    ExecuteNetdiskCmd(cmd, resp);
    if (resp.Find(wxT("Download completed")) != wxNOT_FOUND) {
      return true;
    }
  }
  return false;
}

void wxBaiduNetDiskFrame::OnBtnUploadFile(wxCommandEvent &event) {
  wxString wildcards = wxString::Format(wxT("All files (%s)|%s"),
                                        wxFileSelectorDefaultWildcardStr,
                                        wxFileSelectorDefaultWildcardStr);
  wxFileDialog dialog(this, wxT("Select files to upload..."), wxEmptyString,
                      wxEmptyString, wildcards, wxFD_OPEN | wxFD_MULTIPLE);

  dialog.Centre(wxCENTER_ON_SCREEN);

  if (dialog.ShowModal() == wxID_OK) {
    wxArrayString paths, filenames;
    dialog.GetPaths(paths);

    wxString msg(wxT("u ")), s;
    size_t count = paths.GetCount();
    for (size_t n = 0; n < count; n++) {
      s.Printf("\"%s\" ", paths[n]);
      msg += s;
    }
    msg.Append(txt_path->GetValue());
    wxString resp;
    ExecuteNetdiskCmd(msg, resp);
    if (resp.Find(wxT("Upload file successful")) != wxNOT_FOUND) {
      wxMessageBox(wxT("Upload successful (if multiple selections, some files "
                       "may have failed, please verify)! "));
      ProcessNetdiskLs();
    }
  }
}

void wxBaiduNetDiskFrame::OnBtnUploadDir(wxCommandEvent &event) {
  wxString dirHome;
  wxGetHomeDir(&dirHome);

  wxDirDialog dialog(this, "Testing directory picker", dirHome,
                     wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

  if (dialog.ShowModal() == wxID_OK) {
    wxString cmd = wxT("u \"") + dialog.GetPath() + wxT("\"  \"") +
                   txt_path->GetValue() + wxT("\"");
    wxString resp;
    ExecuteNetdiskCmd(cmd, resp);
    if (resp.Find(wxT("Upload file successful")) != wxNOT_FOUND) {
      wxMessageBox(wxT("Upload directory successful!"));
      if (resp.Find(wxT("Upload file failed")) != wxNOT_FOUND) {
        wxMessageBox(wxT("Although directory upload was successful, some files "
                         "failed to upload, please verify and retry"),
                     wxT("Tip"), wxYES | wxICON_WARNING);
      }
      wxFileName fn(dialog.GetPath());
      cmd = wxT("cd \"") + fn.GetName() + wxT("\"");
      ExecuteNetdiskCmd(cmd, resp);
      ProcessNetdiskLs();
    }
  }
}

void wxBaiduNetDiskFrame::OnBtnMkdir(wxCommandEvent &event) {
  wxTextEntryDialog dialog(this,
                           wxT("Will create new directory in current "
                               "directory, please enter directory name:"),
                           wxT("Please enter directory name"), "",
                           wxOK | wxCANCEL);

  if (dialog.ShowModal() == wxID_OK) {
    wxString cmd = wxT("mkdir \"") + dialog.GetValue() + wxT("\"");
    wxString resp;
    ExecuteNetdiskCmd(cmd, resp);
    if (resp.Find(wxT("Create directory successful")) != wxNOT_FOUND) {
      wxMessageBox(wxT("Create new directory successful!"));
      cmd = wxT("cd \"") + dialog.GetValue() + wxT("\"");
      ExecuteNetdiskCmd(cmd, resp);
      ProcessNetdiskLs();
    } else {
      wxMessageBox(wxT("Create directory failed, please check if target "
                       "already exists or network"),
                   wxT("Error"), wxOK | wxICON_ERROR);
    }
  }
}

void wxBaiduNetDiskFrame::OnBtnRm(wxCommandEvent &event) {
  int success = 0;
  auto n = m_listCtrl->GetItemCount();
  for (int i = 0; i < n; ++i) {
    if (m_listCtrl->IsItemChecked(i)) {
      success += (RemoveItem(i) == true ? 1 : 0);
    }
  }
  wxString msg = wxString::Format(
      wxT("Task completed, %d items successfully deleted"), success);
  wxMessageBox(msg);
  if (success > 0) {
    ProcessNetdiskLs();
  }
}

bool wxBaiduNetDiskFrame::RemoveItem(long i) {
  wxListItem currentItem;
  currentItem.SetId(i);
  if (m_listCtrl->GetItem(currentItem)) {
    wxString name = currentItem.GetText();
    wxString cmd = wxT("rm \"") + name + wxT("\"");
    wxString resp;
    ExecuteNetdiskCmd(cmd, resp);
    if (resp.Find(wxT("Operation successful")) != wxNOT_FOUND) {
      return true;
    }
  }
  return false;
}

void wxBaiduNetDiskFrame::OnBtnSelectAll(wxCommandEvent &event) {
  bool check = true;
  if (checked_items_cnt == m_listCtrl->GetItemCount()) {
    check = false;
  }
  for (int i = 0; i < m_listCtrl->GetItemCount(); ++i) {
    m_listCtrl->CheckItem(i, check);
  }
}

void wxBaiduNetDiskFrame::OnListItemChecked(wxListEvent &event) {
  checked_items_cnt++;
  if (checked_items_cnt == m_listCtrl->GetItemCount()) {
    chk_sel_all->SetValue(true);
  }
}

void wxBaiduNetDiskFrame::OnListItemUnChecked(wxListEvent &event) {
  checked_items_cnt--;
  chk_sel_all->SetValue(false);
}
