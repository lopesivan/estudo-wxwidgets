Aqui está a tradução completa do artigo mantendo todo o código-fonte original em C++ intacto:

---

# O básico de Layout no wxWidgets: Box Sizers — DevMindscape

Vamos falar sobre layout. Este é um conceito fundamental em qualquer tipo de programação de interfaces visuais (UI), e o desenvolvimento de aplicações desktop com o wxWidgets não é diferente.

---

## Posicionamento Padrão

Layout trata de organizar os elementos da sua interface na janela. Embora você possa fazer isso especificando posições absolutas, essa costuma ser uma má ideia. O ideal é que seus controles fiquem visivelmente agradáveis em diferentes tamanhos de janela e que eles se estiquem e se movam quando o usuário redimensionar a aplicação.

Vamos começar com uma aplicação wxWidgets bem básica:

```cpp
#include <wx/wx.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
};

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame("Hello World", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
}

```

O que temos aqui é uma classe de aplicação simples e uma janela principal (*frame*) vazia. Esse é o mínimo necessário para um aplicativo desktop funcional que rode nas principais plataformas (Windows, Linux e Mac).

Vamos adicionar alguns controles, começando por um painel colorido simples. Coloque este código dentro do construtor de `MyFrame`:

```cpp
auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
panel->SetBackgroundColour(wxColour(200, 100, 100));

```

Passar `this` (que aponta para o objeto `MyFrame`) como o primeiro parâmetro do construtor do painel o torna um filho (*child*) da janela principal. Isso também transfere a posse do objeto do painel para o próprio framework wxWidgets. Significa que não precisamos chamar `delete` no objeto do painel, mesmo tendo-o construído usando o operador `new`.

O parâmetro `wxID_ANY` indica que não nos importamos com o ID do painel e deixamos o framework gerá-lo. Os dois parâmetros seguintes especificam o tamanho e a posição do controle. Não colocamos nenhum valor absoluto ali; apenas usamos os identificadores padrões.

Curiosamente, o painel preenche toda a janela e se estica quando ela é redimensionada. Isso acontece porque, com apenas um controle na janela, o framework faz o esticamento automaticamente. Se adicionarmos outro controle, até mesmo um simples botão, os resultados mudam totalmente:

```cpp
auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
panel->SetBackgroundColour(wxColour(200, 100, 100));
auto button = new wxButton(this, wxID_ANY, "Click Me!");

```

Agora não há mais esticamento, e os controles mantêm seus tamanhos e posições padrão. O framework não oferece mais o redimensionamento automático, e precisamos programar as regras de layout por conta própria.

Poderíamos aplicar um posicionamento absoluto apenas definindo valores fixos em pixels:

```cpp
auto panel = new wxPanel(this, wxID_ANY, FromDIP(wxPoint(10, 10)), FromDIP(wxSize(200, 100)));
panel->SetBackgroundColour(wxColour(200, 100, 100));
auto button = new wxButton(this, wxID_ANY, "Click Me!", FromDIP(wxPoint(10, 120)), wxDefaultSize);

```

Duas coisas interessantes aqui. Primeiro, envolvemos os valores de pixel em chamadas `FromDIP`. Isso garante que os tamanhos fiquem corretos em telas de alta densidade (High DPI) em determinados sistemas operacionais.

Segundo, deixamos o tamanho do botão como `wxDefaultSize`. O framework wxWidgets utiliza os controles nativos do sistema, portanto os botões têm tamanhos diferentes dependendo da plataforma. Deixamos ele calcular o tamanho correto de acordo com o sistema operacional de destino.

Não apenas os controles deixam de se esticar quando o usuário redimensiona a janela, mas também tivemos que calcular manualmente as posições para obter, por exemplo, as margens de 10px ao redor do botão. Isso pode ser feito de forma muito mais simples usando **sizers**.

---

## Apresentando o wxBoxSizer

Existe uma família inteira de sizers, incluindo `wxBoxSizer`, `wxGridSizer`, `wxFlexGridSizer`, entre outros. O *box sizer*, sendo o mais simples, é também o mais utilizado, e uma variedade surpreendentemente grande de layouts pode ser desenvolvida usando apenas essa variante básica.

O uso para todas as variantes de sizer é basicamente o mesmo: criamos o objeto sizer, adicionamos os controles a ele e o definimos como o sizer do controle pai.

```cpp
auto sizer = new wxBoxSizer(wxVERTICAL);
auto panel = new wxPanel(this);
panel->SetBackgroundColour(wxColour(200, 100, 100));
auto button = new wxButton(this, wxID_ANY, "Click Me!");
sizer->Add(panel, 1, wxEXPAND | wxALL, FromDIP(10));
sizer->Add(button, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
this->SetSizer(sizer);

```

Com apenas essas poucas linhas, obtemos um layout organizado com margens e o comportamento de esticamento correto.

No entanto, o framework oferece até liberdade demais para o usuário: ele pode redimensionar a janela para um tamanho ridiculamente pequeno e inutilizável.

Existem duas maneiras de resolver isso. A primeira é usar `SetMinClientSize` no final do construtor do `MyFrame`:

```cpp
this->SetMinClientSize(FromDIP(wxSize(300, 200)));

```

Ou definir as dimensões do painel desde o início e então usar `SetSizerAndFit`. Isso redimensionará a janela para o menor tamanho possível (restrito pelo tamanho do painel no nosso caso) e garantirá que o usuário não possa encolhê-la ainda mais:

```cpp
auto sizer = new wxBoxSizer(wxVERTICAL);
// Aqui definimos o tamanho do painel
auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(400, 250)));
panel->SetBackgroundColour(wxColour(200, 100, 100));
auto button = new wxButton(this, wxID_ANY, "Click Me!");
sizer->Add(panel, 1, wxEXPAND | wxALL, FromDIP(10));
sizer->Add(button, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
// note o "...AndFit"
this->SetSizerAndFit(sizer);

```

Com qualquer uma dessas técnicas, o tamanho mínimo será mantido pelo framework.

---

## Explorando os Parâmetros do Sizer

Mas o que significam todos esses valores que passamos para o método `Add` do sizer? Vamos analisar mais de perto a primeira chamada.

### Item

Obviamente podemos adicionar controles (ou seja, classes derivadas de `wxWindow`) aos nossos sizers, mas não é só isso. Outros sizers também podem ser adicionados. Isso abre mais possibilidades, como centralizar tanto na vertical quanto na horizontal, algo que exploraremos mais adiante.

### Proporção (Proportion)

Este parâmetro é interessante. Especificar `0` indica que não queremos que o item se estique na direção do sizer. Qualquer outro valor significa que o controle se esticará proporcionalmente ao valor total de proporção de todos os itens adicionados àquele sizer.

Se quisermos que um controle ocupe 1/3 da altura disponível e o outro preencha os 2/3 restantes, podemos fazer assim:

```cpp
auto sizer = new wxBoxSizer(wxVERTICAL);
auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(400, 100)));
panel->SetBackgroundColour(wxColour(200, 100, 100));
auto otherPanel = new wxPanel(this);
otherPanel->SetBackgroundColour(wxColour(100, 200, 100));
auto button = new wxButton(this, wxID_ANY, "Click Me!");
sizer->Add(panel, 1, wxEXPAND | wxALL, FromDIP(10));
sizer->Add(otherPanel, 2, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
sizer->Add(button, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
this->SetSizerAndFit(sizer);

```

O primeiro painel é adicionado com `proportion = 1`, e o outro com `proportion = 2`. A soma desses parâmetros é 3, portanto o primeiro preenche 1/3 do espaço vertical disponível, enquanto o segundo se estica para preencher os 2/3 restantes. Como criamos nosso sizer na direção vertical (`auto sizer = new wxBoxSizer(wxVERTICAL)`), todo esse esticamento ocorre ao longo do eixo vertical.

### Flag (Sinalizadores)

As *flags* em um *box sizer* servem para dois propósitos principais:

1. Descrevem o posicionamento do elemento ao longo do eixo perpendicular à direção do sizer.
2. Determinam quais margens (*borda*) devem ser aplicadas.

As flags podem ser combinadas com o operador OR (`|`). Sendo assim, `wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM` no nosso caso significa que queremos que o painel se estique ao longo do eixo horizontal (lembrando que a orientação do sizer é vertical) – essa é a função da flag `wxEXPAND`.

Em seguida, adicionamos as flags para as margens esquerda, direita e inferior. Omitimos a superior porque o controle acima usa a flag `wxALL`, portanto o espaçamento acima do nosso controle já é garantido pela margem inferior do controle anterior.

Da mesma forma, as flags `wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM` para o botão aplicam as margens esquerda, direita e inferior, garantindo que o botão fique centralizado na direção horizontal (`wxALIGN_CENTER`).

A lista completa de flags disponíveis pode ser encontrada na documentação do wxWidgets.

### Borda (Border)

Esse é o tamanho da borda ou margem. No nosso exemplo, usamos 10 pixels independentes de densidade (DIP) em todos os cantos para alcançar um layout proporcional e agradável.

---

## Aninhando Box Sizers

Aninhando *box sizers* de forma inteligente, podemos alcançar excelentes resultados sem nunca precisar recorrer a subclasses mais avançadas de `wxSizer`.

Os controles dentro do nosso painel podem ter seus próprios sizers — essa seria a forma mais simples de aninhá-los:

```cpp
auto sizer = new wxBoxSizer(wxVERTICAL);
auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(400, 250)));
auto list = new wxListView(panel);
list->InsertColumn(0, "Column 1");
list->SetColumnWidth(0, FromDIP(100));
for (int i = 0; i < 5; i++) {
    list->InsertItem(i, wxString::Format("Item %d", i));
}
auto itemDesc = new wxStaticText(panel, wxID_ANY, "Here is a description of the selected item. \n\nIt can be as long as you want it to be, \neven spanning multiple lines.");
auto panelSizer = new wxBoxSizer(wxHORIZONTAL);
panelSizer->Add(list, 1, wxEXPAND | wxRIGHT, FromDIP(10));
panelSizer->Add(itemDesc, 2, wxEXPAND);
panel->SetSizerAndFit(panelSizer);
auto button = new wxButton(this, wxID_ANY, "Click Me!");
sizer->Add(panel, 1, wxEXPAND | wxALL, FromDIP(10));
sizer->Add(button, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
this->SetSizerAndFit(sizer);

```

Este exemplo adiciona um controle de lista (lembre-se de incluir `#include <wx/listctrl.h>`) e uma descrição ao painel. Desta vez, adicionamos nosso novo sizer ao painel (e não ao *frame* principal), aninhando os sizers e garantindo o comportamento de redimensionamento automático.

---

## Adicionando Sizers a Outros Sizers

Na verdade, se o único propósito do painel for gerenciar o redimensionamento automático dos controles, nem precisamos dele. Podemos adicionar os controles diretamente ao *frame* principal, criar um sizer separado para eles e adicionar esse sizer ao sizer principal (em vez de adicionar o painel ao sizer principal).

```cpp
auto sizer = new wxBoxSizer(wxVERTICAL);
auto list = new wxListView(this);
list->InsertColumn(0, "Column 1");
list->SetColumnWidth(0, FromDIP(100));
for (int i = 0; i < 5; i++) {
    list->InsertItem(i, wxString::Format("Item %d", i));
}
auto itemDesc = new wxStaticText(this, wxID_ANY, "Here is a description of the selected item. \n\nIt can be as long as you want it to be, \neven spanning multiple lines.");
auto listPickerSizer = new wxBoxSizer(wxHORIZONTAL);
listPickerSizer->Add(list, 1, wxEXPAND | wxRIGHT, FromDIP(10));
listPickerSizer->Add(itemDesc, 2, wxEXPAND);
auto button = new wxButton(this, wxID_ANY, "Click Me!");
// adicionando um sizer diretamente a outro sizer
sizer->Add(listPickerSizer, 1, wxEXPAND | wxALL, FromDIP(10));
sizer->Add(button, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
this->SetSizerAndFit(sizer);

```

---

## Centralizando Controles em Ambas as Direções

Uma das tarefas comuns, porém ligeiramente desafiadoras envolvendo sizers aninhados, é centralizar um controle tanto na horizontal quanto na vertical. Isso pode ser alcançado usando dois *Box Sizers* como neste exemplo:

```cpp
auto verticalSizer = new wxBoxSizer(wxVERTICAL);
auto horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(400, 300)));
panel->SetBackgroundColour(wxColour(200, 100, 100));
horizontalSizer->Add(panel, 0, wxALIGN_CENTER);
verticalSizer->Add(horizontalSizer, 1, wxALIGN_CENTER); // Note a proporção!
this->SetSizerAndFit(verticalSizer);

```

Primeiro, adicionamos o painel ao sizer horizontal. A proporção é definida como zero, o que significa que o painel não se esticará horizontalmente. No entanto, ele ficará centralizado na direção perpendicular graças à flag `wxALIGN_CENTER`.

Analisando a estrutura, isso significa que o sizer horizontal cuidará de centralizar o item na vertical. Agora precisamos de um sizer vertical para centralizar o painel horizontalmente.

O detalhe interessante aqui é o parâmetro de proporção ser definido como 1. Se o definíssemos como 0, não haveria esticamento ao longo do eixo do `verticalSizer` e o painel ficaria preso no topo. Mas, ao definir a proporção para 1, "esticamos" o `horizontalSizer` verticalmente, permitindo que ele centralize o painel na direção vertical.

---

## Conclusão

Neste post, exploramos os conceitos básicos de layout de interface no wxWidgets. Apresentamos o layout absoluto, evoluímos para o redimensionamento automático e exploramos as particularidades do `wxBoxSizer`.

Essa classe simples pode ser bastante útil tanto para interfaces de usuário simples quanto para as mais avançadas, graças à capacidade de aninhar sizers.
