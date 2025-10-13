#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/regex.h>

class SyntaxHighlighter
{
public:
    SyntaxHighlighter(wxTextCtrl* editor);
    void ApplyHighlighting();

private:
    wxTextCtrl* codeEditor;

    void ApplyStyleForPattern(const wxString& pattern, const wxColour& color, bool bold = false);
    void SetupDefaultStyles();
};

#endif // SYNTAXHIGHLIGHTER_H
