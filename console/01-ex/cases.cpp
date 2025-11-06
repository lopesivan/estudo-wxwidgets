#include <wx/string.h>
#include <wx/crt.h>

int main(int argc, char** argv)
{
    wxString str = wxT("The history of my life");

    wxPuts(str.MakeLower());
    wxPuts(str.MakeUpper());
}
