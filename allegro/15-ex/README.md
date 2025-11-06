1) Bridge (wx como abstração • Allegro como implementor)
Parte 1 — Interface & Estrutura

Crie a arquitetura inicial do padrão Bridge para integrar wxWidgets (UI/host) e um backend de renderização (Allegro).
Sem código de Allegro ainda.
Entregue:

pastas/arquivos (ui/, render/, core/),

interface IRenderBackend (métodos essenciais: init(handle,w,h), resize, render, shutdown),

classe RenderView (wxPanel) que conhece apenas a interface,

diagrama textual de dependências.
Não escreva código ainda; apenas explique as responsabilidades de cada arquivo e como se conectam.

Parte 2 — Implementor Allegro

Agora implemente o implementor do Bridge: AllegroBackend que implementa IRenderBackend.
Entregue: passos de inicialização, ciclo de draw, encerramento e como receber o native handle do RenderView.
Mostre como injetar a instância no RenderView (sem código, só o plano).

Parte 3 — Troca fácil de backend

Mostre como criar um segundo backend “Mock” (ou SDL) e como alternar entre backends sem mudar a UI.
Entregue: pontos exatos onde a troca acontece e checklist de teste manual.

# perfeito — vamos montar isso em duas partes, no estilo “Bridge”: **wxWidgets = Abstraction (UI/host)** e **Allegro = Implementor (renderização)**. Vou me apoiar no comportamento do seu código atual (painel wx renderizando via Allegro em bitmap offscreen e blit para o wx).

---

# Parte 1 — Interface & Estrutura (sem código)

## 📁 Estrutura de pastas/arquivos

```
project-root/
├─ core/
│  ├─ IRenderBackend.hpp            # Interface do implementor (Bridge)
│  ├─ RenderModel.hpp               # Modelo do “mouse_pos.c” (estado + lógica km↔px)
│  ├─ RenderPipeline.hpp            # Orquestra pipeline (pré/pós draw, stats, etc.) (opcional)
│  └─ BackendFactory.hpp            # Cria o backend certo (Allegro, mock, etc.)
│
├─ render/
│  ├─ AllegroBackend.hpp            # Implementor: Allegro (só declarado aqui; parte 2 descreve)
│  └─ (futuros) OtherBackend.hpp    # Implementor alternativo (ex.: OpenGL puro, Skia)
│
├─ ui/
│  ├─ RenderView.hpp                # Abstraction: wxPanel que fala só com IRenderBackend
│  ├─ AppFrame.hpp                  # wxFrame que hospeda o RenderView
│  └─ App.hpp                       # wxApp: cria a janela e injeta o backend
│
└─ docs/
   └─ bridge-deps.txt               # Diagrama textual de dependências
```

## 🎯 Responsabilidades (arquivo a arquivo)

### core/IRenderBackend.hpp

* **O que é**: a interface “Implementor” do Bridge.
* **Métodos essenciais** (assinaturas conceituais, sem código):

  * `bool init(void* native_handle, int width, int height);`

    * Recebe o *handle nativo* do `RenderView` (se o backend precisar) e o tamanho inicial.
    * **No nosso plano com Allegro offscreen**, o `native_handle` pode ser ignorado (ou usado futuramente).
  * `void resize(int width, int height);`
  * `void render(const RenderModel& model);`

    * Desenha o frame com base no estado (ex.: pontos/linhas como no *mouse_pos.c*).
  * `void shutdown();`
  * **Canal de saída** (uma das duas estratégias, definidas pela interface):

    * `wxBitmap getFrameBitmap();` **ou** `const uint8_t* mapFrameRGB(size_t& stride);` (para o `RenderView` transformar num `wxBitmap`).
    * Mantém o Bridge agnóstico de Allegro — só expõe pixels prontos para a UI.

### core/RenderModel.hpp

* **O que é**: Porta o estado do seu `mouse_pos.c` (sem depender de UI nem de Allegro).
* **Contém**:

  * `pos_km_x[2], pos_km_y[2], points_stored, mouse_x_px, mouse_y_px, width, height`.
  * Funções utilitárias idênticas ao que você já usa:

    * `armazenar_posicao_mouse(px, py, w, h, index)`
    * `converter_km_para_pixel(km_x, km_y, w, h, &px, &py)`
    * `calcular_modulo_vetor(...)`
    * `resetar_posicoes(...)`
* **Por quê**: separa completamente a **lógica de domínio** (seu “mouse_pos”) do backend e da UI.

### core/RenderPipeline.hpp (opcional)

* **O que é**: lugar para passos comuns pré/pós-render (limpar fundo, medir FPS, compose HUD).
* **Uso**: se você quiser que *parte* do HUD (textos) seja responsabilidade do backend, esta classe define “o que” e o backend decide “como” (fonte, primitivos, etc.).

### core/BackendFactory.hpp

* **O que é**: resolve qual `IRenderBackend` instanciar (ex.: via enum, flags, config).
* **Responsa**: criar e retornar um `std::unique_ptr<IRenderBackend>` para a UI.

### render/AllegroBackend.hpp

* **O que é**: declaração do implementor Allegro (detalhado na Parte 2).
* **Importante**: **não** expor cabeçalhos de wx aqui (mantém *render* independente de UI).

### ui/RenderView.hpp

* **O que é**: `wxPanel` (Abstraction) que:

  * recebe um `std::unique_ptr<IRenderBackend>` (injeção),
  * mantém um `RenderModel` interno,
  * trata eventos do wx (mouse/teclado/resize) **apenas** atualizando o `RenderModel`,
  * no `OnPaint` chama `backend->render(model)` e **puxa pixels** (`getFrameBitmap()` ou `mapFrameRGB` → `wxImage/wxBitmap`) para desenhar no `wxPaintDC`.
* **Não conhece Allegro**: só a interface `IRenderBackend`.

### ui/AppFrame.hpp / ui/App.hpp

* **O que é**: inicialização do app wx, cria `AppFrame`, `RenderView` e injeta o backend via `BackendFactory`.

---

## 🔗 Diagrama textual de dependências (docs/bridge-deps.txt)

```
[ui/App] ──creates──> [ui/AppFrame] ──owns──> [ui/RenderView]
[ui/RenderView] ──has──> [core/RenderModel]
[ui/RenderView] ──depends-on──> [core/IRenderBackend]   (interface)
[core/BackendFactory] ──creates──> [render/AllegroBackend] : IRenderBackend
[render/AllegroBackend] ──uses──> Allegro5 (al_init, bitmaps, draw, etc.)
```

* **Fluxo de desenho**: `RenderView::OnPaint()` → `IRenderBackend::render(model)` → `RenderView` pega pixels do backend → desenha com `wxDC`.
* **Eventos**: `RenderView` atualiza o `RenderModel`; o backend **nunca** fala direto com wx.

---

# Parte 2 — Implementor Allegro (sem código, só o plano)

## 🧱 Classe `AllegroBackend` (Implementor de `IRenderBackend`)

### Estado interno

* `ALLEGRO_BITMAP* backbuffer_` (bitmap em memória, **offscreen**).
* `ALLEGRO_FONT* font_` (usa sua TTF `"SauceCodeProNerdFontMono-Regular.ttf"` com fallback).
* Largura/altura atuais.
* Buffer de saída (ex.: `std::vector<uint8_t> rgb_`) **ou** um `wxBitmap` cacheado (se optar por retornar `wxBitmap` direto).
* Flag de “inicializado”.

### 1) Inicialização (`init(handle, w, h)`)

* **Contexto Allegro**:

  * `al_init()`, `al_install_mouse()`, `al_install_keyboard()`,
  * `al_init_font_addon()`, `al_init_ttf_addon()`, `al_init_primitives_addon()`.
* **Bitmap offscreen**:

  * `al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);`
  * `backbuffer_ = al_create_bitmap(w, h);`
  * `al_set_target_bitmap(backbuffer_);`
  * `al_clear_to_color(al_map_rgb(0,0,0));`
* **Fonte**:

  * `font_ = al_load_font("SauceCodeProNerdFontMono-Regular.ttf", 20, 0);`
  * fallback: `font_ = al_create_builtin_font();`
* **Handle nativo**:

  * **Offscreen mode (recomendado e igual ao seu código atual):** `native_handle` **não é necessário**.
  * (Opcional/futuro) *Native window mode*: receber `HWND` (Windows), `XID` (X11) etc. → **não recomendado** com Allegro 5 por portabilidade; manter como extensão futura.

### 2) Redimensionamento (`resize(w, h)`)

* Destroi o `backbuffer_` e recria com novo tamanho (memória).
* Limpa o fundo para preto.
* Ajusta buffers de saída (`rgb_`) conforme a estratégia de “entrega”.

### 3) Desenho (`render(model)`)

* **Setup**: `al_set_target_bitmap(backbuffer_); al_clear_to_color(0,0,0);`
* **Usa *somente* o `RenderModel`:**

  * Converte km → px para cada ponto via funções do **model**.
  * Desenha círculos/linha como no *mouse_pos.c* (cores corretas).
  * Escreve os textos (posição, deltas, módulo).
  * (Mantém a lógica de `resetar_posicoes` **no model/UI**; o backend só desenha o que o model manda.)
* **Entrega dos pixels** para a UI (escolha 1):

  * **(A) Pull RGB “pronto”:** trava `backbuffer_` no formato mais conveniente e preenche `rgb_` (24bpp RGB). *Vimos que formatos variam por backend; para fidelidade total*, pode usar:

    * `al_get_pixel` + `al_unmap_rgba` (mais simples, independente de formato, custo ok para 720p).
    * **ou** detecção de formato (`al_get_bitmap_format`) e conversão correta (melhor performance).
  * **(B) Retornar `wxBitmap`:** backend já devolve um `wxBitmap` pronto. (Traz acoplamento leve a wx via header, por isso a opção A é mais “pura”.)

### 4) Encerramento (`shutdown()`)

* `al_destroy_font(font_)`, `al_destroy_bitmap(backbuffer_)`.
* `al_shutdown_primitives_addon();`
* `al_uninstall_system();` (ou deixar o host encerrar — mas no seu fluxo atual, desligar aqui funciona).

> 🔎 **Por que offscreen/backbuffer?**
> É exatamente o que funcionou melhor no seu código: Allegro desenha num bitmap em memória, e o `RenderView` copia para a tela via `wxPaintDC`. Evita todas as dores de integrar uma janela Allegro como filho do wx (não é cross-platform no Allegro 5).

---

## 🪝 “Native handle” do `RenderView` (quando/como usar)

* **Render offscreen (recomendado):** `native_handle` **não é usado** — só serve para manter a assinatura estável se no futuro você quiser um backend que precise disso.
* **Se um dia quiser “native window mode”**:

  * **Windows (MSW):** `HWND hwnd = (HWND) renderView->GetHandle();`
  * **GTK/X11:** `GdkWindow* gw = gtk_widget_get_window(...);` → `Window xid = gdk_x11_window_get_xid(gw);`
  * **Wayland:** não fornece XID; embedding não é trivial.
  * **Allegro 5** não tem API estável para **adotar** uma janela estrangeira em todos os backends. Por isso, recomendo ficar no **offscreen**.

---

## 🧩 Injeção do Allegro no `RenderView` (sem código, só o plano)

1. **Criação** (em `ui/App`):

   * `auto backend = BackendFactory::make(RenderBackendType::Allegro);`
   * `renderView->setBackend(std::move(backend));`

2. **Ciclo de vida** (em `RenderView`):

   * `Create/Show`: chama `backend->init(native_handle, width, height)`.
   * `wxEVT_SIZE`: `backend->resize(w,h)`.
   * `wxEVT_PAINT`:

     * atualiza `RenderModel` com mouse/teclado (como no seu fluxo atual),
     * chama `backend->render(model)`,
     * **puxa pixels** (`getFrameBitmap()` ou `mapFrameRGB` → `wxImage/wxBitmap`) e desenha.
   * `Destroy`: `backend->shutdown()`.

3. **Modelo**:

   * `RenderView` é o único que muta o **RenderModel** (armazenar pontos, resetar, converter km↔px).
   * O `AllegroBackend` só **lê** o `RenderModel` para desenhar.

---

## ✅ Resultado

* **UI (wx)** completamente desacoplada do **render** (Allegro).
* Comportamento idêntico ao do seu programa atual (pontos, linha, textos, fundo preto), mas agora **organizado** no padrão **Bridge**.
* Preparado para trocar o backend (ex.: OpenGL puro, Skia) sem tocar em `RenderView`/`AppFrame`.

Se quiser, eu já te entrego os **arquivos vazios** (headers) com as **assinaturas** e comentários prontos pra você só preencher — mantendo exatamente essa arquitetura.
