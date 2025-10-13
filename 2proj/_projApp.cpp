#include "_projApp.h"
#include "_projMain.h"
#include <wx/msgdlg.h>

wxIMPLEMENT_APP(_projApp);

bool _projApp::OnInit()
{
    try {
        _projMain* frame = new _projMain(NULL);
        frame->Show(true);
        return true;
    }
    catch (const std::exception& e) {
        wxMessageBox(wxString::Format("Failed to start: %s", e.what()), "Error", wxOK | wxICON_ERROR);
        return false;
    }
    catch (...) {
        wxMessageBox("Unknown error during startup", "Error", wxOK | wxICON_ERROR);
        return false;
    }
}
