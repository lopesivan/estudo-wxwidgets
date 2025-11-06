#include <wx/string.h>
#include <wx/crt.h>

int main(int argc, char** argv)
{

    int flowers = 21;

    wxString str;
    str.Printf(wxT("There are %d red roses."), flowers);

    wxPuts(str);
}
