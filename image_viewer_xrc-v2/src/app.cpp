#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "main_frame.h"

class ImageViewerApp : public wxApp
{
public:
    bool OnInit() override
    {
        wxInitAllImageHandlers();

        wxXmlResource::Get()->InitAllHandlers();

        if (!wxXmlResource::Get()->Load("resources/main_frame.xrc"))
        {
            wxLogError("Não foi possível carregar resources/main_frame.xrc");
            return false;
        }

        wxString imagePath = "assets/imagem.png";

        if (argc > 1)
            imagePath = argv[1];

        auto* frame = new MainFrame(imagePath);
        frame->Show();

        return true;
    }
};

wxIMPLEMENT_APP(ImageViewerApp);

