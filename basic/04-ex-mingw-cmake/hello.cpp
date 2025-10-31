#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/wx.h>

class SpinApp : public wxApp {
public:
  virtual bool OnInit();
};

class SpinFrame : public wxFrame {
public:
  SpinFrame();

private:
  void OnSpinUp(wxSpinEvent &event);
  void OnSpinDown(wxSpinEvent &event);
  void OnSpinCtrl(wxSpinEvent &event);
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);

  wxSpinButton *m_spinButton;
  wxSpinCtrl *m_spinCtrl;
  wxStaticText *m_counterText;
  wxStaticText *m_spinCtrlText;
  int m_counter;

  DECLARE_EVENT_TABLE()
};

enum { ID_SpinButton = 1000, ID_SpinCtrl = 1001 };

// clang-format off
wxBEGIN_EVENT_TABLE(SpinFrame, wxFrame)
    EVT_SPIN_UP(ID_SpinButton, SpinFrame::OnSpinUp)
    EVT_SPIN_DOWN(ID_SpinButton, SpinFrame::OnSpinDown)
    EVT_SPINCTRL(ID_SpinCtrl, SpinFrame::OnSpinCtrl)
    EVT_MENU(wxID_EXIT, SpinFrame::OnExit)
    EVT_MENU(wxID_ABOUT, SpinFrame::OnAbout)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(SpinApp);
// clang-format on

bool SpinApp::OnInit() {
  SpinFrame *frame = new SpinFrame();
  frame->Show(true);
  return true;
}

SpinFrame::SpinFrame()
    : wxFrame(NULL, wxID_ANY,
              wxString::FromUTF8(u8"Aplicativo com Botão Giratório")),
      m_counter(0) {
  // Criar menu
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(wxID_EXIT);

  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&Arquivo");
  menuBar->Append(menuHelp, "&Ajuda");

  SetMenuBar(menuBar);

  // Criar status bar
  CreateStatusBar();
  SetStatusText("Bem-vindo ao aplicativo de contador!");

  // Criar painel principal
  wxPanel *panel = new wxPanel(this, wxID_ANY);

  // Criar sizer principal
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  // Título
  wxStaticText *title = new wxStaticText(
      panel, wxID_ANY, wxString::FromUTF8(u8"Contador com Botão Giratório"));
  wxFont titleFont = title->GetFont();
  titleFont.SetPointSize(16);
  titleFont.SetWeight(wxFONTWEIGHT_BOLD);
  title->SetFont(titleFont);
  mainSizer->Add(title, 0, wxALL | wxCENTER, 10);

  // Seção do SpinButton
  wxStaticBoxSizer *spinButtonBox = new wxStaticBoxSizer(
      wxVERTICAL, panel, wxString::FromUTF8(u8"Botão Giratório"));

  wxBoxSizer *spinButtonSizer = new wxBoxSizer(wxHORIZONTAL);

  // Texto do contador
  m_counterText = new wxStaticText(panel, wxID_ANY, "0");
  wxFont counterFont = m_counterText->GetFont();
  counterFont.SetPointSize(20);
  counterFont.SetWeight(wxFONTWEIGHT_BOLD);
  m_counterText->SetFont(counterFont);

  // Botão giratório
  m_spinButton = new wxSpinButton(panel, ID_SpinButton, wxDefaultPosition,
                                  wxDefaultSize, wxSP_VERTICAL);
  m_spinButton->SetRange(-100, 100);
  m_spinButton->SetValue(0);

  spinButtonSizer->Add(m_counterText, 0, wxALL | wxCENTER, 5);
  spinButtonSizer->Add(m_spinButton, 0, wxALL | wxCENTER, 5);

  spinButtonBox->Add(spinButtonSizer, 0, wxALL | wxCENTER, 5);

  // Seção do SpinCtrl (alternativa mais completa)
  wxStaticBoxSizer *spinCtrlBox =
      new wxStaticBoxSizer(wxVERTICAL, panel, "Controle de Spin com Texto");

  wxBoxSizer *spinCtrlSizer = new wxBoxSizer(wxHORIZONTAL);

  wxStaticText *spinCtrlLabel = new wxStaticText(panel, wxID_ANY, "Valor:");
  m_spinCtrl = new wxSpinCtrl(panel, ID_SpinCtrl, "0", wxDefaultPosition,
                              wxDefaultSize, wxSP_ARROW_KEYS, -50, 50, 0);

  m_spinCtrlText = new wxStaticText(panel, wxID_ANY, "Valor atual: 0");

  spinCtrlSizer->Add(spinCtrlLabel, 0, wxALL | wxCENTER, 5);
  spinCtrlSizer->Add(m_spinCtrl, 0, wxALL | wxCENTER, 5);

  spinCtrlBox->Add(spinCtrlSizer, 0, wxALL | wxCENTER, 5);
  spinCtrlBox->Add(m_spinCtrlText, 0, wxALL | wxCENTER, 5);

  // Adicionar tudo ao sizer principal
  mainSizer->Add(spinButtonBox, 0, wxALL | wxEXPAND, 10);
  mainSizer->Add(spinCtrlBox, 0, wxALL | wxEXPAND, 10);

  // Instruções
  wxStaticText *instructions = new wxStaticText(
      panel, wxID_ANY,
      wxString::FromUTF8(
          u8"Use as setas do botão giratório para incrementar/decrementar o "
          u8"contador.\n"
          u8"O SpinCtrl permite digitar o valor diretamente ou usar as "
          u8"setas."));
  instructions->Wrap(400);
  mainSizer->Add(instructions, 0, wxALL | wxCENTER, 10);

  panel->SetSizer(mainSizer);

  // Ajustar tamanho da janela
  SetSize(450, 350);
  Center();
}

void SpinFrame::OnSpinUp(wxSpinEvent &event) {
  m_counter++;
  m_counterText->SetLabel(wxString::Format("%d", m_counter));
  SetStatusText(wxString::Format("Contador incrementado para: %d", m_counter));
}

void SpinFrame::OnSpinDown(wxSpinEvent &event) {
  m_counter--;
  m_counterText->SetLabel(wxString::Format("%d", m_counter));
  SetStatusText(wxString::Format("Contador decrementado para: %d", m_counter));
}

void SpinFrame::OnSpinCtrl(wxSpinEvent &event) {
  int value = m_spinCtrl->GetValue();
  m_spinCtrlText->SetLabel(wxString::Format("Valor atual: %d", value));
  SetStatusText(wxString::Format("SpinCtrl alterado para: %d", value));
}

void SpinFrame::OnExit(wxCommandEvent &event) { Close(true); }

void SpinFrame::OnAbout(wxCommandEvent &event) {
  wxMessageBox(wxString::FromUTF8(u8"Aplicativo demonstrando o uso de "
                                  u8"controles giratórios em wxWidgets"),
               "Sobre", wxOK | wxICON_INFORMATION);
}
