#ifndef GORUNNER_H
#define GORUNNER_H

#include <wx/wx.h>
#include <wx/process.h>
#include <wx/timer.h>
#include <functional>

class GoRunner : public wxEvtHandler
{
public:
    using OutputCallback = std::function<void(const wxString&)>;

    GoRunner();
    ~GoRunner();

    void RunCode(const wxString& code, OutputCallback callback);
    void StopExecution();
    bool IsRunning() const { return isRunning; }

private:
    wxProcess* goProcess;
    wxTimer* outputTimer;
    bool isRunning;
    OutputCallback outputCallback;

    void OnOutputTimer(wxTimerEvent& event);
    void OnProcessEnd(wxProcessEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif // GORUNNER_H
