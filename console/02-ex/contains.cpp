#include <wx/string.h>
#include <wx/crt.h>

int main(int argc, char** argv)
{

    wxString str = wxT("The history of my life");

    if(str.Contains(wxT("history")))
    {
        wxPuts(wxT("Contains!"));
    }


    if(!str.Contains(wxT("plain")))
    {
        wxPuts(wxT("Does not contain!"));
    }
}
