
#pragma once

#include "vrepMainHeader.h"
#include "scintillaConsoleDlg.h"
#include "uiThread.h"

class CScintillaConsoleContainer 
{
public:
    CScintillaConsoleContainer();
    virtual ~CScintillaConsoleContainer();

    // From UI thread:
    void executeCommand(SUIThreadCommand* cmdIn,SUIThreadCommand* cmdOut);

    // From any thread;
    void simulationEnded();
    void handleConsoles();
    void hideOrShowAll(bool showState);

    // From sim thread:
    bool consoleSetShowState(int handle,bool show);
    bool removeConsole(int handle);
    int addConsoleInfo(CConsoleInitInfo* consoleInfo);
    CConsoleInitInfo* getConsoleInfoFromHandle(int handle);
    CScintillaConsoleDlg* getConsoleFromHandle(int handle);
    bool isConsoleHandleValid(int handle);

protected:
    std::vector<CScintillaConsoleDlg*> _allConsoles;
    std::vector<CConsoleInitInfo*> _allUninitializedConsoles;

    static int _nextConsoleID;
};
