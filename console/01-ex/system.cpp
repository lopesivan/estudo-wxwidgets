#include <wx/string.h>
#include <wx/utils.h>
#include <wx/crt.h>

int main(int argc, char** argv)
{
    wxPuts(wxGetHomeDir());
    wxPuts(wxGetOsDescription());
    wxPuts(wxGetUserName());
    wxPuts(wxGetFullHostName());

    long mem = wxGetFreeMemory().ToLong();

    wxPrintf(wxT("Memory: %ld\n"), mem);
}
