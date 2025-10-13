#ifndef EDITOR_H
#define EDITOR_H

#include <wx/wx.h>
#include <wx/textctrl.h>
#include "SyntaxHighlighter.h"

class Editor : public wxTextCtrl
{
public:
    Editor(wxWindow* parent, wxWindowID id,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTE_MULTILINE);
    ~Editor();

    void ApplySyntaxHighlighting();

private:
    SyntaxHighlighter* highlighter;

    void OnTextChange(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif // EDITOR_H
