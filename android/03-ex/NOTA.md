Aqui está usando `wxXRC` — o layout sai do `.cpp` e vai para um arquivo `.xrc`:

**`main.xrc`:**
```xml
<?xml version="1.0" encoding="UTF-8"?>
<resource>
  <object class="wxFrame" name="MainFrame">
    <title>Controle ADB - Android</title>
    <size>350,250</size>
    <object class="wxPanel" name="panel">
      <object class="wxBoxSizer">
        <orient>wxVERTICAL</orient>
        <object class="sizeritem">
          <flag>wxALL|wxEXPAND</flag>
          <border>20</border>
          <object class="wxGridSizer">
            <cols>3</cols>
            <rows>3</rows>
            <vgap>5</vgap>
            <hgap>5</hgap>
            <!-- linha 1 -->
            <object class="sizeritem">
              <flag>wxEXPAND</flag>
              <object class="wxStaticText" name="empty1">
                <label></label>
              </object>
            </object>
            <object class="sizeritem">
              <flag>wxEXPAND</flag>
              <object class="wxButton" name="btnUp">
                <label>Cima</label>
              </object>
            </object>
            <object class="sizeritem">
              <flag>wxEXPAND</flag>
              <object class="wxStaticText" name="empty2">
                <label></label>
              </object>
            </object>
            <!-- linha 2 -->
            <object class="sizeritem">
              <flag>wxEXPAND</flag>
              <object class="wxButton" name="btnLeft">
                <label>Esquerda</label>
              </object>
            </object>
            <object class="sizeritem">
              <flag>wxEXPAND</flag>
              <object class="wxButton" name="btnOk">
                <label>OK</label>
              </object>
            </object>
            <object class="sizeritem">
              <flag>wxEXPAND</flag>
              <object class="wxButton" name="btnRight">
                <label>Direita</label>
              </object>
            </object>
            <!-- linha 3 -->
            <object class="sizeritem">
              <flag>wxEXPAND</flag>
              <object class="wxStaticText" name="empty3">
                <label></label>
              </object>
            </object>
            <object class="sizeritem">
              <flag>wxEXPAND</flag>
              <object class="wxButton" name="btnDown">
                <label>Baixo</label>
              </object>
            </object>
            <object class="sizeritem">
              <flag>wxEXPAND</flag>
              <object class="wxStaticText" name="empty4">
                <label></label>
              </object>
            </object>
          </object>
        </object>
      </object>
    </object>
  </object>
</resource>
```

**`main.cpp`:**
```cpp
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <cstdlib>
#include <string>

static void adb_key(int key)
{
    std::string cmd = "adb shell input keyevent " + std::to_string(key);
    std::system(cmd.c_str());
}

class MainFrame : public wxFrame
{
public:
    MainFrame()
    {
        wxXmlResource::Get()->LoadFrame(this, nullptr, "MainFrame");

        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(19); }, wxXmlResource::GetXRCID("btnUp"));
        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(20); }, wxXmlResource::GetXRCID("btnDown"));
        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(21); }, wxXmlResource::GetXRCID("btnLeft"));
        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(22); }, wxXmlResource::GetXRCID("btnRight"));
        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(23); }, wxXmlResource::GetXRCID("btnOk"));
    }
};

class App : public wxApp
{
public:
    bool OnInit() override
    {
        wxXmlResource::Get()->InitAllHandlers();
        wxXmlResource::Get()->Load("main.xrc");

        auto* frame = new MainFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(App);
```

O `main.xrc` pode ser embutido no binário com `wxrc` para não precisar distribuir o arquivo separado:

```bash
# gera main.xrc.cpp com o XRC embutido
wxrc main.xrc --cpp-code --output=main.xrc.cpp --function=InitXmlResource
```

E no `main.cpp` troca o `Load` por:

```cpp
// declaração gerada pelo wxrc
void InitXmlResource();

bool OnInit() override
{
    wxXmlResource::Get()->InitAllHandlers();
    InitXmlResource();  // XRC embutido, sem arquivo externo

    auto* frame = new MainFrame();
    frame->Show();
    return true;
}
```

Assim o binário final é autossuficiente.
