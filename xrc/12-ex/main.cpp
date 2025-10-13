#include <wx/wizard.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

class MyApp : public wxApp {
public:
  bool OnInit() override;
};

bool MyApp::OnInit() {
  // Inicializa handlers de imagem (PNG, JPG, etc)
  wxInitAllImageHandlers();

  // Inicializa XRC
  wxXmlResource::Get()->InitAllHandlers();

  if (!wxXmlResource::Get()->Load("ui.xrc")) {
    wxLogError("Erro ao carregar ui.xrc");
    return false;
  }

  // Carrega o wizard do XRC
  wxWizard *wizard = new wxWizard();
  if (!wxXmlResource::Get()->LoadObject(wizard, nullptr, "ID_WIZARD",
                                        "wxWizard")) {
    wxLogError("Erro ao carregar wizard do XRC");
    return false;
  }

  // Define bitmap do wizard (se não vier do XRC)
  wxBitmap wizardBitmap;
  if (wxFileExists("wizard.png")) {
    wizardBitmap.LoadFile("wizard.png", wxBITMAP_TYPE_PNG);
    wizard->SetBitmap(wizardBitmap);
  }

  // Cria página 1
  wxWizardPageSimple *page1 = new wxWizardPageSimple(wizard);
  wxBoxSizer *sizer1 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *text1 =
      new wxStaticText(page1, wxID_ANY,
                       "Bem-vindo ao Assistente!\n\n"
                       "Esta é a primeira página.\n"
                       "Clique em 'Próximo' para continuar.",
                       wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

  sizer1->Add(text1, 0, wxALL | wxEXPAND, 20);
  page1->SetSizer(sizer1);

  // Cria página 2
  wxWizardPageSimple *page2 = new wxWizardPageSimple(wizard);
  wxBoxSizer *sizer2 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *text2 =
      new wxStaticText(page2, wxID_ANY,
                       "Segunda Página\n\n"
                       "Aqui você pode adicionar controles como:\n"
                       "- Caixas de texto\n"
                       "- Checkboxes\n"
                       "- Radio buttons\n"
                       "- etc.",
                       wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

  wxTextCtrl *input =
      new wxTextCtrl(page2, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

  sizer2->Add(text2, 0, wxALL | wxEXPAND, 20);
  sizer2->Add(new wxStaticText(page2, wxID_ANY, "Digite seu nome:"), 0,
              wxLEFT | wxRIGHT | wxTOP, 20);
  sizer2->Add(input, 0, wxALL | wxEXPAND, 20);
  page2->SetSizer(sizer2);

  // Cria página 3 (final)
  wxWizardPageSimple *page3 = new wxWizardPageSimple(wizard);
  wxBoxSizer *sizer3 = new wxBoxSizer(wxVERTICAL);

  wxStaticText *text3 =
      new wxStaticText(page3, wxID_ANY,
                       "Página Final\n\n"
                       "Configuração concluída!\n\n"
                       "Clique em 'Concluir' para finalizar.",
                       wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

  sizer3->Add(text3, 0, wxALL | wxEXPAND, 20);
  page3->SetSizer(sizer3);

  // Encadeia as páginas
  wxWizardPageSimple::Chain(page1, page2);
  wxWizardPageSimple::Chain(page2, page3);

  // Define tamanho mínimo
  wizard->SetPageSize(wxSize(400, 300));
  wizard->FitToPage(page1);
  wizard->CentreOnScreen();

  // Executa o wizard
  bool completed = wizard->RunWizard(page1);

  if (completed) {
    wxMessageBox("Wizard concluído com sucesso!", "Sucesso",
                 wxOK | wxICON_INFORMATION);
  } else {
    wxMessageBox("Wizard cancelado.", "Cancelado", wxOK | wxICON_WARNING);
  }

  wizard->Destroy();

  return false; // Encerra a aplicação
}

wxIMPLEMENT_APP(MyApp);
