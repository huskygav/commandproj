#include "SyntaxHighlighter.h"
#include <wx/log.h>

SyntaxHighlighter::SyntaxHighlighter(wxTextCtrl* editor)
    : codeEditor(editor)
{
    SetupDefaultStyles();
}

void SyntaxHighlighter::SetupDefaultStyles()
{
    if (!codeEditor) return;

    // Устанавливаем моноширинный шрифт для редактора
    codeEditor->SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}

void SyntaxHighlighter::ApplyHighlighting()
{
    if (!codeEditor) return;

    wxString code = codeEditor->GetValue();
    if (code.IsEmpty()) return;

    // Сохраняем позицию курсора и выделение
    long insertionPoint = codeEditor->GetInsertionPoint();
    long from, to;
    codeEditor->GetSelection(&from, &to);

    // Сбрасываем все стили к стандартным
    codeEditor->SetStyle(0, code.Length(), wxTextAttr(*wxBLACK));

    // Подсветка ключевых слов Go - СИНИЙ
    ApplyStyleForPattern("\\b(package|import|func|var|const|type|struct|interface|map|chan|go|defer|select|range|if|else|for|switch|case|default|break|continue|return)\\b",
                        wxColour(0, 0, 200), true);

    // Подсветка типов - ТЕМНО-СИНИЙ
    ApplyStyleForPattern("\\b(int|int8|int16|int32|int64|uint|uint8|uint16|uint32|uint64|uintptr|float32|float64|complex64|complex128|byte|rune|string|bool|error)\\b",
                        wxColour(0, 100, 200));

    // Подсветка значений - ФИОЛЕТОВЫЙ
    ApplyStyleForPattern("\\b(true|false|nil|iota)\\b",
                        wxColour(150, 0, 200));

    // Подсветка встроенных функций - ОРАНЖЕВЫЙ
    ApplyStyleForPattern("\\b(append|cap|close|complex|copy|delete|imag|len|make|new|panic|print|Println|real|recover)\\b",
                        wxColour(200, 100, 0));

    // Подсветка строк - КРАСНЫЙ
    ApplyStyleForPattern("\"[^\"]*\"", wxColour(200, 0, 0));
    ApplyStyleForPattern("`[^`]*`", wxColour(200, 0, 0));

    // Подсветка комментариев - ЗЕЛЕНЫЙ
    ApplyStyleForPattern("//[^\n]*", wxColour(0, 150, 0));
    ApplyStyleForPattern("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/", wxColour(0, 150, 0));

    // Подсветка чисел - ФИОЛЕТОВЫЙ
    ApplyStyleForPattern("\\b[0-9]+\\b", wxColour(150, 0, 200));
    ApplyStyleForPattern("\\b0x[0-9a-fA-F]+\\b", wxColour(150, 0, 200));
    ApplyStyleForPattern("\\b[0-9]+\\.[0-9]+\\b", wxColour(150, 0, 200));

    // Восстанавливаем позицию курсора и выделение
    codeEditor->SetInsertionPoint(insertionPoint);
    if (from != to) {
        codeEditor->SetSelection(from, to);
    }
}

void SyntaxHighlighter::ApplyStyleForPattern(const wxString& pattern, const wxColour& color, bool bold)
{
    if (!codeEditor) return;

    wxString code = codeEditor->GetValue();
    if (code.IsEmpty()) return;

    wxRegEx regex(pattern);
    if (!regex.IsValid()) {
        return;
    }

    size_t start = 0;
    wxString text = code;

    while (regex.Matches(text)) {
        size_t startPos, len;
        if (!regex.GetMatch(&startPos, &len, 0)) {
            break;
        }

        if (len > 0) {
            wxTextAttr style;
            style.SetTextColour(color);
            if (bold) {
                style.SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            } else {
                style.SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
            codeEditor->SetStyle(start + startPos, start + startPos + len, style);
        }

        // Перемещаемся дальше по тексту
        if (startPos + len >= text.Length()) {
            break;
        }
        start += startPos + len;
        text = text.Mid(startPos + len);

        // Защита от бесконечного цикла
        if (text.IsEmpty()) {
            break;
        }
    }
}
