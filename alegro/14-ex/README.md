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

