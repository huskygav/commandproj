#include "Editor.h"
#include <wx/log.h>

wxBEGIN_EVENT_TABLE(Editor, wxTextCtrl)
    EVT_TEXT(wxID_ANY, Editor::OnTextChange)
wxEND_EVENT_TABLE()

Editor::Editor(wxWindow* parent, wxWindowID id,
               const wxString& value,
               const wxPoint& pos,
               const wxSize& size,
               long style)
    : wxTextCtrl(parent, id, value, pos, size, style | wxTE_RICH2 | wxTE_DONTWRAP)
{
    // Создаем подсветку синтаксиса
    highlighter = new SyntaxHighlighter(this);

    // Применяем начальную подсветку
    ApplySyntaxHighlighting();
}

Editor::~Editor()
{
    delete highlighter;
}

void Editor::ApplySyntaxHighlighting()
{
    if (highlighter) {
        highlighter->ApplyHighlighting();
    }
}

void Editor::OnTextChange(wxCommandEvent& event)
{
    // Автоматически применяем подсветку при изменении текста
    ApplySyntaxHighlighting();
    event.Skip();
}
