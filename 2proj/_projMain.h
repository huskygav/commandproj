#ifndef _PROJMAIN_H
#define _PROJMAIN_H

#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include "GoRunner.h"
#include "Editor.h"

enum {
    ID_RUN_BUTTON = 1001,
    ID_SAVE_BUTTON,
    ID_NEW_BUTTON,
    ID_CLEAR_BUTTON
};

class _projMain : public wxFrame
{
public:
    _projMain(wxWindow* parent);
    ~_projMain();

private:
    Editor* editor;
    wxTextCtrl* outputArea;
    wxButton* runButton;
    wxButton* saveButton;
    wxButton* newButton;
    wxButton* clearOutputButton;

    GoRunner* goRunner;

    void OnRunCode(wxCommandEvent& event);
    void OnSaveCode(wxCommandEvent& event);
    void OnNewFile(wxCommandEvent& event);
    void OnClearOutput(wxCommandEvent& event);
    void OnOutput(const wxString& output);

    wxDECLARE_EVENT_TABLE();
};

#endif // _PROJMAIN_H
