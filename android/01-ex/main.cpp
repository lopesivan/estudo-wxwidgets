#include <wx/wx.h>
#include <cstdlib>
#include <string>

static void adb_key(int key)
{
    std::string cmd =
        "adb shell input keyevent " + std::to_string(key);
    std::system(cmd.c_str());
}

class MainFrame : public wxFrame
{
  public:
    MainFrame()
        : wxFrame(nullptr,
                  wxID_ANY,
                  "Controle ADB - Android",
                  wxDefaultPosition,
                  wxSize(350, 250))
    {
        auto* panel = new wxPanel(this);

        auto* mainSizer = new wxBoxSizer(wxVERTICAL);

        auto* grid = new wxGridSizer(3, 3, 5, 5);

        auto* empty1 = new wxStaticText(panel, wxID_ANY, "");
        auto* btnUp  = new wxButton(panel, wxID_ANY, "Cima");
        auto* empty2 = new wxStaticText(panel, wxID_ANY, "");

        auto* btnLeft  = new wxButton(panel, wxID_ANY, "Esquerda");
        auto* btnOk    = new wxButton(panel, wxID_ANY, "OK");
        auto* btnRight = new wxButton(panel, wxID_ANY, "Direita");

        auto* empty3  = new wxStaticText(panel, wxID_ANY, "");
        auto* btnDown = new wxButton(panel, wxID_ANY, "Baixo");
        auto* empty4  = new wxStaticText(panel, wxID_ANY, "");

        grid->Add(empty1, 1, wxEXPAND);
        grid->Add(btnUp, 1, wxEXPAND);
        grid->Add(empty2, 1, wxEXPAND);

        grid->Add(btnLeft, 1, wxEXPAND);
        grid->Add(btnOk, 1, wxEXPAND);
        grid->Add(btnRight, 1, wxEXPAND);

        grid->Add(empty3, 1, wxEXPAND);
        grid->Add(btnDown, 1, wxEXPAND);
        grid->Add(empty4, 1, wxEXPAND);

        mainSizer->Add(grid, 1, wxALL | wxEXPAND, 20);
        panel->SetSizer(mainSizer);

        btnUp->Bind(wxEVT_BUTTON,
                    [](wxCommandEvent&) { adb_key(19); });

        btnDown->Bind(wxEVT_BUTTON,
                      [](wxCommandEvent&) { adb_key(20); });

        btnLeft->Bind(wxEVT_BUTTON,
                      [](wxCommandEvent&) { adb_key(21); });

        btnRight->Bind(wxEVT_BUTTON,
                       [](wxCommandEvent&) { adb_key(22); });

        btnOk->Bind(wxEVT_BUTTON,
                    [](wxCommandEvent&) { adb_key(23); });
    }
};

class App : public wxApp
{
  public:
    bool OnInit() override
    {
        auto* frame = new MainFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(App);
