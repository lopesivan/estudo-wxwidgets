Esse warning do GTK:

```
Gtk-CRITICAL **: gtk_cairo_should_draw_window: assertion 'GDK_IS_WINDOW (window)' failed
```

aparece quando **alguém tenta desenhar** (cairo/GTK) num widget/janela **que ainda não tem GdkWindow válido** (não foi “realizada”/mostrada) — ou já foi destruída. Em wxGTK isso costuma vir de:

* desenhar fora de um `wxPaintEvent` (não é seu caso);
* chamar algo que força pintura/layout **antes de `Show(true)`**;
* mexer em widgets **enquanto a janela ainda não foi “realizada”**;
* usar um **parent errado/oculto** (ex.: criar um `wxPanel` novo sobrepondo o do XRC; você já tirou isso);
* raramente, glitches de tema/Wayland.

Seu código não faz desenho manual, então o mais provável é **timing de layout**. Vamos deixar a sequência 100% segura e ver se some.

## Ajuste simples (ordem de chamadas)

No construtor de `MainFrame`, troque a ordem para **mostrar primeiro** e **layout depois**:

```cpp
MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Gerenciador de Pacotes APT")
{
    if (!wxXmlResource::Get()->LoadFrame(this, nullptr, "ID_WXFRAME")) {
        wxLogError("Falha ao carregar ID_WXFRAME do XRC.");
        return;
    }

    SetSize(wxSize(900, 600));
    Centre();

    CacheControls();
    SetupLabels();
    SetupListColumns();
    BindEvents();

    // Mostra antes de qualquer layout/pintura
    Show(true);

    // Agora recalcula o layout com a janela já "realizada"
    Layout();
    Update(); // opcional: força uma passada de pintura
}
```

E **não crie** `new wxPanel(*frame)` em lugar nenhum — use sempre o `ID_PANEL` do XRC (como você já está fazendo agora).

## Outras checagens rápidas

1. **Sem desenho fora de paint**
   Não use `wxClientDC`, `wxScreenDC` ou `wxAutoBufferedPaintDC` fora de um handler de `wxEVT_PAINT`. Você não usa, então ok.

2. **Pais corretos**
   Todos os controles vêm do XRC com o `ID_PANEL` como parent. Não crie widgets com parent nulo/errado.

3. **Wayland vs X11** (alguns temas/compósitos dão avisos)
   Teste com X11:

   ```bash
   GDK_BACKEND=x11 ./app
   ```

   Se o aviso some, é ambiente/tema. Dá pra seguir assim sem prejuízo.

4. **Ver o culpado com backtrace (opcional)**
   Se ainda quiser identificar exatamente quem dispara:

   ```bash
   G_DEBUG=fatal-criticals gdb --args ./app
   (gdb) run
   # quando parar no crítico:
   (gdb) bt
   ```

   Isso mostra a stack no ponto do aviso.

5. **Compile com XRC**
   Garanta que está linkando com `--libs std,xrc`. Sem isso, o XRC instância “vazio” e pode gerar sequências estranhas de layout/pintura.

## Se persistir

* Experimente **remover** o `Layout()` do final (só `Show(true)` já resolve em 99%).
* Ou chame `Layout()` no **painel do XRC**:

  ```cpp
  if (auto* panel = XRCCTRL(*this, "ID_PANEL", wxPanel)) panel->Layout();
  ```

Na prática, esse aviso é **cosmético** quando não há desenho manual — mas vale a pena corrigir a ordem para evitar que o GTK tente pintar elementos antes da janela estar “realizada”. Quer que eu te mande os quatro arquivos já com essa ordem aplicada?

