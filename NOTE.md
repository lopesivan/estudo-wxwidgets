Aqui está a tradução completa do artigo mantendo o código original em C e C++ intacto:

---

# wxWidgets: A Porta de Entrada Amigável para o Desenvolvimento Desktop no Linux — DevMindscape

Alguém deixou um comentário em um dos meus vídeos sobre wxWidgets dizendo que passou uma semana tentando entender o GTK e o gtkmm e que, depois de assistir ao vídeo, avançou mais em poucas horas do que em toda aquela semana[cite: 1]. Isso não é incomum, e tem muito a ver com a forma como o GTK é projetado[cite: 1].

O GTK é escrito em C, mas implementa um framework fortemente orientado a objetos por cima dele usando o sistema de tipos GObject[cite: 1]. A documentação do GObject explica o motivo[cite: 1]:

> *É provável que programadores C fiquem confusos com a complexidade dos recursos expostos nos capítulos a seguir se esquecerem que a biblioteca GType/GObject não foi projetada apenas para oferecer recursos no estilo OO para programadores C, mas também para interoperabilidade transparente entre diferentes linguagens.*
> *(Fonte: GObject Type System Concepts)*[cite: 1]

Esse design facilita a criação de *bindings* para linguagens de nível mais alto, como Python e JavaScript[cite: 1]. Mas para desenvolvedores C e C++, ele adiciona muita complexidade[cite: 1]. O `gtkmm`, que é o *binding* C++ oficial, ajuda em parte disso, mas ainda herda os arquivos de interface baseados em XML, o sistema de sinais e todo o peso conceitual do GTK[cite: 1].

O wxWidgets adota uma abordagem diferente[cite: 1]. Ele envolve (*wrap*) o GTK 3 no Linux (e usa APIs nativas no Windows e macOS), fornecendo uma API C++ direta e simples[cite: 1]. Neste artigo, vamos implementar o mesmo aplicativo simples em todas as três abordagens — GTK 3 em C, gtkmm e wxWidgets — para comparar a experiência do desenvolvedor[cite: 1].

---

## GTK 3 em C

Queremos uma janela com um rótulo (*label*) e um botão que exibe uma caixa de diálogo (*message dialog*) quando clicado[cite: 1]. A abordagem padrão no GTK é definir o layout em um arquivo XML e carregá-lo com o `GtkBuilder`[cite: 1].

Aqui está a definição da UI:

```xml
<!-- ui/window.ui -->
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk+" version="3.24"/>
  <object class="GtkWindow" id="main_window">
    <property name="title">GTK 3 Example</property>
    <property name="default-width">400</property>
    <property name="default-height">300</property>
    <signal name="destroy" handler="gtk_main_quit"/>
    <child>
      <object class="GtkBox" id="main_box">
        <property name="orientation">vertical</property>
        <property name="spacing">10</property>
        <property name="border-width">20</property>
        <child>
          <object class="GtkLabel" id="main_label">
            <property name="label">Welcome to GTK 3</property>
            <property name="vexpand">True</property>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="click_button">
            <property name="label">Click Me</property>
            <signal name="clicked" handler="on_button_clicked"/>
          </object>
        </child>
      </object>
    </child>
  </object>
</interface>

```

E o código C que o carrega:

```c
#include <gtk/gtk.h>

void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window = GTK_WIDGET(data);
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(window),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Hello from GTK 3!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("ui/window.ui");

    GtkWidget *window = GTK_WIDGET(
        gtk_builder_get_object(builder, "main_window"));
    GtkWidget *button = GTK_WIDGET(
        gtk_builder_get_object(builder, "click_button"));

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    g_object_unref(builder);

    gtk_main();
    return 0;
}

```

Acabamos tendo dois arquivos apenas para exibir um rótulo e um botão[cite: 1]. O XML usa uma estrutura aninhada de tags `<object>`, `<property>` e `<child>`[cite: 1]. No código C, os widgets são buscados por IDs em formato de texto (`"main_window"`, `"click_button"`), convertidos via macros (`GTK_WIDGET`, `GTK_WINDOW`, `GTK_DIALOG`), e os sinais são conectados usando nomes em texto com *callbacks* do tipo `gpointer`[cite: 1]. Um erro de digitação no nome do sinal ou no ID do widget não gerará um erro de compilação — ele apenas falhará silenciosamente em tempo de execução[cite: 1].

---

## gtkmm (C++)

Vamos tentar o jeito C++ com o `gtkmm`[cite: 1]. Podemos reutilizar o mesmo arquivo XML e carregá-lo com `Gtk::Builder`[cite: 1]:

```cpp
#include <gtkmm.h>
#include <iostream>

class MyWindow : public Gtk::Window {
public:
    MyWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
        : Gtk::Window(cobject), m_builder(builder) {
        m_builder->get_widget("click_button", m_button);
        m_builder->get_widget("main_label", m_label);

        if (m_button) {
            m_button->signal_clicked().connect(
                sigc::mem_fun(*this, &MyWindow::on_button_clicked));
        }

        show_all_children();
    }

protected:
    void on_button_clicked() {
        Gtk::MessageDialog dialog(*this, "Hello from gtkmm!");
        dialog.set_secondary_text("This is a message dialog.");
        dialog.run();
    }

    Glib::RefPtr<Gtk::Builder> m_builder;
    Gtk::Button *m_button = nullptr;
    Gtk::Label *m_label = nullptr;
};

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.example.gtkmm");
    auto builder = Gtk::Builder::create_from_file("ui/window.ui");

    MyWindow *window = nullptr;
    builder->get_widget_derived("main_window", window);

    if (window) {
        app->run(*window);
    }

    delete window;
    return 0;
}

```

Agora temos classes reais e conexões de sinais com segurança de tipos (*type-safe*) via `sigc::mem_fun`[cite: 1]. Não há mais *casts* com macros[cite: 1]. Mas note a assinatura do construtor — o parâmetro `BaseObjectType *cobject` é uma exigência do sistema GObject[cite: 1]. Ainda buscamos os widgets por IDs de texto com `get_widget`, e a biblioteca `sigc` para manipulação de sinais exige um certo tempo de adaptação[cite: 1].

O arquivo XML mantém a mesma hierarquia aninhada[cite: 1]. Para o nosso pequeno exemplo, ele já passa de 20 linhas[cite: 1]. Para uma aplicação real com barras de ferramentas, menus e caixas de diálogo, esses arquivos ficam enormes[cite: 1]. O designer de UI Glade ajuda a gerá-los, mas depurar problemas de layout frequentemente significa ler o XML puro[cite: 1].

---

## wxWidgets

Agora a mesma coisa com wxWidgets[cite: 1]:

```cpp
#include <wx/wx.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "wxWidgets Example", wxDefaultPosition, wxDefaultSize) {
        auto sizer = new wxBoxSizer(wxVERTICAL);

        auto label = new wxStaticText(this, wxID_ANY, "Welcome to wxWidgets");
        sizer->Add(label, 1, wxALIGN_CENTER | wxALL, 20);

        auto button = new wxButton(this, wxID_ANY, "Click Me");
        sizer->Add(button, 0, wxALIGN_CENTER | wxBOTTOM, 20);

        button->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event) {
            wxMessageBox("Hello from wxWidgets!", "Message", wxOK | wxICON_INFORMATION);
        });

        this->SetSizerAndFit(sizer);
    }
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    auto frame = new MyFrame();
    frame->Show(true);
    return true;
}

```

O tratamento de eventos é apenas uma expressão lambda passada para `Bind`[cite: 1]. O layout é feito diretamente no código com sizers[cite: 1]. Sem arquivos XML, sem bibliotecas de sinais complexas, sem *casts* de macros[cite: 1]. Ele é lido exatamente como C++ padrão[cite: 1].

Além disso, o wxWidgets usa seu próprio mecanismo de contagem de referências[cite: 1]. Criamos nossos controles usando o operador `new`, mas nunca os deletamos manualmente[cite: 1]. O controle pai (ou o próprio objeto Application, no caso do `wxFrame` principal) deletará automaticamente seus filhos ao encerrar a aplicação[cite: 1].

No Linux, o wxWidgets utiliza o GTK por baixo dos panos[cite: 1]. Sua aplicação renderiza widgets GTK reais — ele não desenha seus próprios controles do zero[cite: 1]. Você obtém a aparência (*look and feel*) nativa com uma API muito mais simples[cite: 1].

---

## Comparação de Tratamento de Eventos

Vamos comparar como cada abordagem lida com uma tarefa comum: atualizar um rótulo em tempo real conforme o usuário digita em um campo de texto[cite: 1].

### GTK 3 (C)

```c
static void on_text_changed(GtkEditable *editable, gpointer data) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(editable));
    gtk_label_set_text(GTK_LABEL(data), text);
}

// No código de configuração:
GtkWidget *entry = gtk_entry_new();
GtkWidget *label = gtk_label_new("");
g_signal_connect(entry, "changed", G_CALLBACK(on_text_changed), label);

```

Os nomes dos sinais são textos, a função de *callback* recebe um `gpointer` que precisa de *cast* manual, e o *label* é passado como um ponteiro genérico através do parâmetro `data`[cite: 1].

### gtkmm (C++)

```cpp
m_entry.signal_changed().connect([this]() {
    m_label.set_text(m_entry.get_text());
});

```

Seguro quanto aos tipos (*type-safe*) e conciso[cite: 1]. Porém, você precisa aprender o sistema de sinais `sigc`, e os nomes dos sinais seguem as convenções do GTK[cite: 1].

### wxWidgets

```cpp
textCtrl->Bind(wxEVT_TEXT, [label](wxCommandEvent &event) {
    label->SetLabel(event.GetString());
});

```

Lambda standard em C++[cite: 1]. O próprio objeto de evento carrega os dados[cite: 1].

---

## Suporte Multiplataforma

O código wxWidgets acima funciona no Windows e no macOS sem nenhuma alteração[cite: 1]. Em cada plataforma, o wxWidgets utiliza o toolkit nativo[cite: 1]:

* **Linux:** GTK 3[cite: 1]
* **Windows:** Win32 API[cite: 1]
* **macOS:** Cocoa[cite: 1]

Seus controles têm aparência nativa em cada plataforma[cite: 1]. O mesmo código-fonte, o mesmo sistema de compilação CMake, e visual nativo em qualquer lugar[cite: 1]. Se você é um desenvolvedor Linux que precisa atender a usuários no Windows ou Mac, não precisa reescrever nada[cite: 1].

---

## Quando Usar o GTK Diretamete

O GTK é a escolha certa se você estiver construindo algo profundamente integrado com a área de trabalho do GNOME, como aplicativos que usam GIO/GVfs ou que precisam seguir à risca as diretrizes de interface do GNOME (HIG)[cite: 1]. Ele também é necessário se você precisa de recursos do GTK 4, como a *pipeline* de renderização acelerada por GPU, ou suporte avançado a Wayland[cite: 1].

Para aplicativos desktop de uso geral em C++, especialmente aqueles que precisam rodar em múltiplas plataformas, o wxWidgets é simplesmente mais simples[cite: 1].

---

## Como Começar

A maioria das distribuições Linux inclui os pacotes de desenvolvimento do wxWidgets nos repositórios[cite: 1].

### Ubuntu / Debian

```bash
sudo apt install libwxgtk3.2-dev build-essential cmake

```

*Em versões mais antigas do Ubuntu (20.04 ou anterior), o pacote pode se chamar `libwxgtk3.0-gtk3-dev`.*[cite: 1]

### Fedora

```bash
sudo dnf install wxGTK3-devel gcc-c++ cmake

```

### Arch Linux

```bash
sudo pacman -S wxwidgets-gtk3 base-devel cmake

```

Depois de instalado, o seu arquivo `CMakeLists.txt` só precisa de uma chamada `find_package` para encontrar o wxWidgets do sistema[cite: 1]. Se você preferir que o CMake baixe e compile o wxWidgets automaticamente, pode usar o `FetchContent` do CMake[cite: 1].

De qualquer forma, o próximo passo é configurar e compilar o projeto[cite: 1]:

```bash
cmake -S. -Bbuild
cmake --build build

```

---

## Resumo

O wxWidgets oferece uma API C++ limpa sobre o GTK 3 no Linux, com renderização nativa e sem necessidade de arquivos de layout XML[cite: 1]. O tratamento de eventos utiliza lambdas padrão, e o mesmo código compila no Windows e no macOS usando os toolkits nativos de cada sistema[cite: 1]. Se você está procurando uma maneira direta de construir aplicações desktop em C++, ele é um ótimo ponto de partida[cite: 1].
