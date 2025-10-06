#include "AptManagerApp.h"

#include "AptManagerFrame.h"

bool AptManagerApp::OnInit()
{
    // Criar a janela principal
    auto* frame = new AptManagerFrame();

    // Mostrar a janela na tela
    frame->Show(true);

    return true;  // Inicialização bem-sucedida
}
