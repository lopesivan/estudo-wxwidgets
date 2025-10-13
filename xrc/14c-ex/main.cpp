#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/checkbox.h>
#include <wx/image.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/wizard.h>

#include <wx/xrc/xmlres.h>

// Classe personalizada para o wizard com melhor controle
class MyWizard : public wxWizard {
private:
  wxWizardPageSimple *m_page1;
  wxWizardPageSimple *m_page2;
  wxWizardPageSimple *m_page3;

  wxTextCtrl *m_textUsername;
  wxTextCtrl *m_textEmail;
  wxCheckBox *m_checkTerms;
  wxStaticText *m_textSummary;

public:
  MyWizard(wxWindow *parent) : wxWizard() {
    // Carrega o wizard do XRC
    auto *xr = wxXmlResource::Get();
    if (!xr->LoadObject(this, parent, "ID_WXWIZARD", "wxWizard")) {
      wxLogError("Falha ao carregar ID_WXWIZARD");
      return;
    }

    // Cria as páginas do wizard
    CreatePages();

    // Conecta os eventos
    Bind(wxEVT_WIZARD_PAGE_CHANGING, &MyWizard::OnPageChanging, this);
    Bind(wxEVT_WIZARD_PAGE_CHANGED, &MyWizard::OnPageChanged, this);
    Bind(wxEVT_WIZARD_FINISHED, &MyWizard::OnFinish, this);
    Bind(wxEVT_WIZARD_CANCEL, &MyWizard::OnCancel, this);
  }

  bool CreatePages() {
    auto *xr = wxXmlResource::Get();

    // Cria páginas simples
    m_page1 = new wxWizardPageSimple(this);
    m_page2 = new wxWizardPageSimple(this);
    m_page3 = new wxWizardPageSimple(this);

    // Carrega e anexa os painéis do XRC
    if (!LoadAndAttachPanel(m_page1, "PAGE_1") ||
        !LoadAndAttachPanel(m_page2, "PAGE_2") ||
        !LoadAndAttachPanel(m_page3, "PAGE_3")) {
      return false;
    }

    // Conecta as páginas em cadeia
    wxWizardPageSimple::Chain(m_page1, m_page2);
    wxWizardPageSimple::Chain(m_page2, m_page3);

    // Obtém referências aos controles usando método direto
    m_textUsername =
        wxDynamicCast(m_page2->FindWindow(XRCID("TEXT_USERNAME")), wxTextCtrl);
    m_textEmail =
        wxDynamicCast(m_page2->FindWindow(XRCID("TEXT_EMAIL")), wxTextCtrl);
    m_checkTerms =
        wxDynamicCast(m_page1->FindWindow(XRCID("CHECK_TERMS")), wxCheckBox);
    m_textSummary =
        wxDynamicCast(m_page3->FindWindow(XRCID("TEXT_SUMMARY")), wxStaticText);

    // Configura validadores
    if (m_textUsername) {
      m_textUsername->SetValidator(wxTextValidator(wxFILTER_EMPTY));
    }
    if (m_textEmail) {
      wxTextValidator emailValidator(wxFILTER_EMPTY);
      m_textEmail->SetValidator(emailValidator);
    }

    return true;
  }

  wxWizardPageSimple *GetFirstPage() const { return m_page1; }

private:
  bool LoadAndAttachPanel(wxWizardPageSimple *page, const wxString &panelName) {
    auto *xr = wxXmlResource::Get();
    wxPanel *panel = xr->LoadPanel(page, panelName);

    if (!panel) {
      wxLogError("Falha ao carregar painel: %s", panelName);
      return false;
    }

    // Cria sizer e adiciona o painel
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel, 1, wxEXPAND);
    page->SetSizer(sizer);

    // Ajusta o tamanho da página baseado no conteúdo
    sizer->Fit(page);

    return true;
  }

  void OnPageChanging(wxWizardEvent &event) {
    wxWizardPage *currentPage = event.GetPage();

    // Validações antes de sair da página atual
    if (currentPage == m_page1) {
      if (!ValidatePage1()) {
        event.Veto();
        return;
      }
    } else if (currentPage == m_page2) {
      if (!ValidatePage2()) {
        event.Veto();
        return;
      }
    }

    event.Skip();
  }

  void OnPageChanged(wxWizardEvent &event) {
    if (event.GetPage() == m_page3) {
      UpdateSummaryPage();
    }
    event.Skip();
  }

  bool ValidatePage1() {
    if (m_checkTerms && !m_checkTerms->GetValue()) {
      wxMessageBox("Você deve aceitar os termos e condições para continuar.",
                   "Validação", wxOK | wxICON_WARNING);
      return false;
    }
    return true;
  }

  bool ValidatePage2() {
    if (m_textUsername && m_textUsername->GetValue().Trim().IsEmpty()) {
      wxMessageBox("Por favor, informe um nome de usuário.", "Validação",
                   wxOK | wxICON_WARNING);
      if (m_textUsername)
        m_textUsername->SetFocus();
      return false;
    }

    if (m_textEmail && m_textEmail->GetValue().Trim().IsEmpty()) {
      wxMessageBox("Por favor, informe um endereço de email.", "Validação",
                   wxOK | wxICON_WARNING);
      if (m_textEmail)
        m_textEmail->SetFocus();
      return false;
    }

    // Validação básica de email
    if (m_textEmail) {
      wxString email = m_textEmail->GetValue();
      if (!email.Contains("@") || !email.Contains(".")) {
        wxMessageBox("Por favor, informe um endereço de email válido.",
                     "Validação", wxOK | wxICON_WARNING);
        m_textEmail->SetFocus();
        return false;
      }
    }

    return true;
  }

  void UpdateSummaryPage() {
    if (!m_textSummary)
      return;

    wxString username =
        m_textUsername ? m_textUsername->GetValue() : "Não informado";
    wxString email = m_textEmail ? m_textEmail->GetValue() : "Não informado";
    wxString terms = (m_checkTerms && m_checkTerms->GetValue()) ? "Sim" : "Não";

    wxString summary =
        wxString::Format("Resumo da Configuração:\n\n"
                         "Nome de usuário: %s\n"
                         "Email: %s\n"
                         "Termos aceitos: %s\n\n"
                         "Clique em 'Concluir' para finalizar a configuração.",
                         username, email, terms);

    m_textSummary->SetLabel(summary);

    // Atualiza o layout
    if (m_page3->GetSizer()) {
      m_page3->GetSizer()->Layout();
    }
  }

  void OnFinish(wxWizardEvent &event) {
    // Validações finais
    if (!ValidatePage1() || !ValidatePage2()) {
      event.Veto();
      return;
    }

    // Coleta dados finais
    wxString username = m_textUsername ? m_textUsername->GetValue() : "";
    wxString email = m_textEmail ? m_textEmail->GetValue() : "";

    wxString message =
        wxString::Format("Configuração concluída com sucesso!\n\n"
                         "Nome de usuário: %s\n"
                         "Email: %s\n\n"
                         "As configurações serão aplicadas agora.",
                         username, email);

    wxMessageBox(message, "Configuração Concluída", wxOK | wxICON_INFORMATION);

    wxLogMessage("Configuração salva - Usuário: %s, Email: %s", username,
                 email);
    event.Skip();
  }

  void OnCancel(wxWizardEvent &event) {
    int result =
        wxMessageBox("Tem certeza que deseja cancelar a configuração?\n"
                     "Todas as informações não salvas serão perdidas.",
                     "Confirmar Cancelamento", wxYES_NO | wxICON_QUESTION);

    if (result == wxYES) {
      wxLogMessage("Wizard cancelado pelo usuário");
      event.Skip();
    } else {
      event.Veto();
    }
  }
};

class MyApp : public wxApp {
public:
  bool OnInit() override {
    // Configura logging
    wxLog::SetActiveTarget(new wxLogStderr());
    wxLog::SetLogLevel(wxLOG_Message);

    // Inicializa handlers de imagem
    wxInitAllImageHandlers();

    // Inicializa sistema XRC
    auto *xr = wxXmlResource::Get();
    xr->InitAllHandlers();

    // Carrega arquivo XRC
    if (!xr->Load("ui.xrc")) {
      wxLogError("Não foi possível carregar ui.xrc");
      wxMessageBox("Erro ao carregar o arquivo de interface.", "Erro",
                   wxOK | wxICON_ERROR);
      return false;
    }

    // Cria o wizard personalizado
    MyWizard *wiz = new MyWizard(nullptr);

    // Verifica se o wizard foi criado com sucesso
    if (!wiz) {
      wxLogError("Falha ao criar o wizard");
      delete wiz;
      return false;
    }

    // Executa o wizard
    wiz->CentreOnScreen();
    bool success = wiz->RunWizard(wiz->GetFirstPage());

    wxLogMessage("Wizard finalizado: %s", success ? "Sucesso" : "Cancelado");

    // Limpeza
    wiz->Destroy();

    return false;
  }
};

wxIMPLEMENT_APP(MyApp);
