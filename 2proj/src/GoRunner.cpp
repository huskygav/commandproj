#include "GoRunner.h"
#include <wx/filename.h>
#include <wx/txtstrm.h>

wxBEGIN_EVENT_TABLE(GoRunner, wxEvtHandler)
    EVT_TIMER(wxID_ANY, GoRunner::OnOutputTimer)
    EVT_END_PROCESS(wxID_ANY, GoRunner::OnProcessEnd)
wxEND_EVENT_TABLE()

GoRunner::GoRunner()
    : goProcess(nullptr), isRunning(false)
{
    outputTimer = new wxTimer(this);
}

GoRunner::~GoRunner()
{
    if (outputTimer && outputTimer->IsRunning()) {
        outputTimer->Stop();
    }
    delete outputTimer;

    if (goProcess) {
        goProcess->Detach();
    }
}

void GoRunner::RunCode(const wxString& code, OutputCallback callback)
{
    outputCallback = callback;

    // Создаем временный файл
    wxString tempDir = wxFileName::GetTempDir() + wxFILE_SEP_PATH + "temp_go_project";
    wxString tempFile = tempDir + wxFILE_SEP_PATH + "main.go";

    // Создаем директорию
    if (!wxDirExists(tempDir)) {
        wxMkdir(tempDir);
    }

    // Создаем go.mod
    wxFile goMod(tempDir + wxFILE_SEP_PATH + "go.mod", wxFile::write);
    if (goMod.IsOpened()) {
        goMod.Write("module temp_go_project\n\ngo 1.21\n");
        goMod.Close();
    }

    // Сохраняем код
    wxFile file(tempFile, wxFile::write);
    if (file.IsOpened()) {
        file.Write(code);
        file.Close();
    } else {
        if (outputCallback) {
            outputCallback("Error: Could not create temporary file\n");
        }
        return;
    }

    // Запускаем выполнение
    if (goProcess) {
        goProcess->Detach();
        delete goProcess;
    }

    goProcess = new wxProcess(this);
    goProcess->Redirect();

    wxString oldCwd = wxGetCwd();
    wxSetWorkingDirectory(tempDir);

    long pid = wxExecute("go run main.go", wxEXEC_ASYNC, goProcess);

    wxSetWorkingDirectory(oldCwd);

    if (pid == 0) {
        if (outputCallback) {
            outputCallback("Error: Failed to execute Go command\n");
        }
    } else {
        isRunning = true;
        outputTimer->Start(100);
    }
}

void GoRunner::StopExecution()
{
    if (goProcess && isRunning) {
        goProcess->Detach();
        delete goProcess;
        goProcess = nullptr;
        isRunning = false;
        outputTimer->Stop();
    }
}

void GoRunner::OnOutputTimer(wxTimerEvent& event)
{
    if (goProcess && isRunning) {
        wxInputStream* inputStream = goProcess->GetInputStream();
        if (inputStream && inputStream->CanRead()) {
            wxTextInputStream textStream(*inputStream);
            while (inputStream->CanRead()) {
                wxString line = textStream.ReadLine();
                if (!line.IsEmpty() && outputCallback) {
                    outputCallback(line + "\n");
                }
            }
        }

        wxInputStream* errorStream = goProcess->GetErrorStream();
        if (errorStream && errorStream->CanRead()) {
            wxTextInputStream errorTextStream(*errorStream);
            while (errorStream->CanRead()) {
                wxString line = errorTextStream.ReadLine();
                if (!line.IsEmpty() && outputCallback) {
                    outputCallback("ERROR: " + line + "\n");
                }
            }
        }
    }
}

void GoRunner::OnProcessEnd(wxProcessEvent& event)
{
    outputTimer->Stop();
    isRunning = false;

    // Читаем оставшийся вывод
    wxInputStream* inputStream = goProcess->GetInputStream();
    if (inputStream) {
        wxTextInputStream textStream(*inputStream);
        while (inputStream->CanRead()) {
            wxString line = textStream.ReadLine();
            if (!line.IsEmpty() && outputCallback) {
                outputCallback(line + "\n");
            }
        }
    }

    wxInputStream* errorStream = goProcess->GetErrorStream();
    if (errorStream) {
        wxTextInputStream errorTextStream(*errorStream);
        while (errorStream->CanRead()) {
            wxString line = errorTextStream.ReadLine();
            if (!line.IsEmpty() && outputCallback) {
                outputCallback("ERROR: " + line + "\n");
            }
        }
    }

    goProcess->Detach();
    delete goProcess;
    goProcess = nullptr;
}
