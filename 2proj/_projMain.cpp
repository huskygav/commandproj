#include "_projMain.h"
#include <wx/filedlg.h>
#include <wx/sizer.h>
#include <wx/file.h>

wxBEGIN_EVENT_TABLE(_projMain, wxFrame)
    EVT_BUTTON(ID_RUN_BUTTON, _projMain::OnRunCode)
    EVT_BUTTON(ID_SAVE_BUTTON, _projMain::OnSaveCode)
    EVT_BUTTON(ID_NEW_BUTTON, _projMain::OnNewFile)
    EVT_BUTTON(ID_CLEAR_BUTTON, _projMain::OnClearOutput)
wxEND_EVENT_TABLE()

_projMain::_projMain(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "Go Editor", wxDefaultPosition, wxSize(1200, 800))
{
    // Создаем GoRunner первым
    goRunner = new GoRunner();

    // Простая панель
    wxPanel* panel = new wxPanel(this);

    // Основной sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // Левая часть - редактор и вывод
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    // Редактор кода
    editor = new Editor(panel, wxID_ANY,
        "package main\n\nimport \"fmt\"\n\nfunc main() {\n    fmt.Println(\"Hello, World!\")\n}",
        wxDefaultPosition, wxSize(800, 500));

    // Область вывода
    outputArea = new wxTextCtrl(panel, wxID_ANY, "Output will appear here...",
                               wxDefaultPosition, wxSize(800, 200),
                               wxTE_MULTILINE | wxTE_READONLY);

    // Правая часть - кнопки
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    // Кнопки с уникальными ID
    runButton = new wxButton(panel, ID_RUN_BUTTON, "Run Code");
    saveButton = new wxButton(panel, ID_SAVE_BUTTON, "Save File");
    newButton = new wxButton(panel, ID_NEW_BUTTON, "New File");
    clearOutputButton = new wxButton(panel, ID_CLEAR_BUTTON, "Clear Output");

    // Устанавливаем размеры кнопок
    runButton->SetMinSize(wxSize(150, 35));
    saveButton->SetMinSize(wxSize(150, 35));
    newButton->SetMinSize(wxSize(150, 35));
    clearOutputButton->SetMinSize(wxSize(150, 35));

    // Добавляем кнопки в правый sizer
    rightSizer->Add(runButton, 0, wxALL, 5);
    rightSizer->Add(saveButton, 0, wxALL, 5);
    rightSizer->Add(newButton, 0, wxALL, 5);
    rightSizer->Add(clearOutputButton, 0, wxALL, 5);
    rightSizer->AddStretchSpacer();

    // Добавляем в левый sizer
    leftSizer->Add(editor, 1, wxEXPAND | wxALL, 5);
    leftSizer->Add(outputArea, 0, wxEXPAND | wxALL, 5);

    // Добавляем в основной sizer
    mainSizer->Add(leftSizer, 1, wxEXPAND);
    mainSizer->Add(rightSizer, 0, wxEXPAND | wxALL, 10);

    panel->SetSizer(mainSizer);
    Centre();
}

_projMain::~_projMain()
{
    delete goRunner;
}

void _projMain::OnRunCode(wxCommandEvent& event)
{
    wxString code = editor->GetValue();
    if (code.IsEmpty()) {
        outputArea->SetValue("No code to run\n");
        return;
    }

    outputArea->SetValue("");

    goRunner->RunCode(code, [this](const wxString& output) {
        this->OnOutput(output);
    });
}

void _projMain::OnSaveCode(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, "Save Go File", "", "main.go",
                               "Go files (*.go)|*.go", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxFile file(saveFileDialog.GetPath(), wxFile::write);
    if (file.IsOpened()) {
        file.Write(editor->GetValue());
        file.Close();
        outputArea->SetValue("File saved: " + saveFileDialog.GetPath() + "\n");
    } else {
        outputArea->SetValue("Error: Could not save file\n");
    }
}

void _projMain::OnNewFile(wxCommandEvent& event)
{
    editor->SetValue(
        "package main\n\n"
        "import \"fmt\"\n\n"
        "func main() {\n"
        "    \n"
        "}"
    );
    outputArea->SetValue("New file created\n");
}

void _projMain::OnClearOutput(wxCommandEvent& event)
{
    outputArea->SetValue("");
}

void _projMain::OnOutput(const wxString& output)
{
    outputArea->AppendText(output);
}
