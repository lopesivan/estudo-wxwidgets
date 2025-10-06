#include "AptManagerApp.h"

#include <clocale>

#include "AptManagerFrame.h"

bool AptManagerApp::OnInit()
{
    std::setlocale(LC_ALL, "");  // usa locale do sistema (ex.: pt_BR.UTF-8)

    if (!wxApp::OnInit())
        return false;
    auto* frame = new AptManagerFrame();
    frame->Show(true);
    return true;
}
