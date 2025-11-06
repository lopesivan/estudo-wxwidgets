#include <wx/file.h>
#include <wx/crt.h>

int main(int argc, char** argv)
{

    wxString str = wxT("You make me want to be a better man.\n");

    wxFile file;
    file.Create(wxT("quote"), true);

    if(file.IsOpened())
        wxPuts(wxT("the file is opened"));

    file.Write(str);

    file.Close();

    if(!file.IsOpened())
        wxPuts(wxT("the file is not opened"));
}
