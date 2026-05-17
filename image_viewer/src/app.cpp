#include <wx/wx.h>
#include "main_frame.h"

class ImageViewerApp : public wxApp
{
public:
    bool OnInit() override
    {
        wxInitAllImageHandlers();

        wxString imagePath = "assets/imagem.png";

        if (argc > 1)
            imagePath = argv[1];

        auto* frame = new MainFrame(imagePath);
        frame->Show();

        return true;
    }
};

wxIMPLEMENT_APP(ImageViewerApp);
