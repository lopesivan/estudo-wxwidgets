#include "app.h"
#include "frame1.h"
#include "frame2.h"

#include <wx/xrc/xmlres.h>

wxIMPLEMENT_APP (MyApp);

bool MyApp::OnInit()
{
    wxXmlResource::Get()->InitAllHandlers();

    if (!wxXmlResource::Get()->Load ("resources.xrc"))
    {
        wxLogError ("Falha ao carregar resources.xrc");
        return false;
    }

    f1_ = new MyFrame1();
    f2_ = new MyFrame2();

    // Começa no Frame 1
    f1_->Show (true);
    return true;
}

void MyApp::ShowFrame1()
{
    if (f2_) f2_->Hide();
    if (f1_) f1_->Show (true), f1_->Raise();
}

void MyApp::ShowFrame2()
{
    if (f1_) f1_->Hide();
    if (f2_) f2_->Show (true), f2_->Raise();
}
