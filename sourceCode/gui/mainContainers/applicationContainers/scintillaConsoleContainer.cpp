
#include "vrepMainHeader.h"
#include "scintillaConsoleContainer.h"
#include "v_rep_internal.h"

#include "app.h"

int CScintillaConsoleContainer::_nextConsoleID=0;


CScintillaConsoleContainer::CScintillaConsoleContainer()
{

}

CScintillaConsoleContainer::~CScintillaConsoleContainer()
{
    for (size_t i=0;i<_allConsoles.size();i++)
        delete _allConsoles[i];
    _allConsoles.clear();
    for (size_t i=0;i<_allUninitializedConsoles.size();i++)
        delete _allUninitializedConsoles[i];
    _allUninitializedConsoles.clear();
}

void CScintillaConsoleContainer::executeCommand(SUIThreadCommand* cmdIn,SUIThreadCommand* cmdOut)
{
    if (cmdIn->cmdId==SCINTILLA_CONSOLES_SIMULATION_ENDED_SCUITHREADCMD)
        simulationEnded();
    if (cmdIn->cmdId==SCINTILLA_CONSOLES_HANDLE_CONSOLE_SCUITHREADCMD)
        handleConsoles();
}

void CScintillaConsoleContainer::simulationEnded()
{
    if (VThread::isCurrentThreadTheUiThread())
    { // we are in the UI thread. We execute the command now:
        // Remove handles that were created from a script:
        int ci=App::ct->getCurrentInstanceIndex();
        for (int i=0;i<int(_allConsoles.size());i++)
        {
            if (_allConsoles[i]->info->getDefaultInstance()==ci)
            {
                if (_allConsoles[i]->simulationEnded())
                {
                    delete _allConsoles[i];
                    _allConsoles.erase(_allConsoles.begin()+i);
                    i--; // We need to reprocess this position
                }
            }
        }
        for (int i=0;i<int(_allUninitializedConsoles.size());i++)
        {
            if (_allUninitializedConsoles[i]->_defaultInstance==ci)
            {
                delete _allUninitializedConsoles[i];
                _allUninitializedConsoles.erase(_allUninitializedConsoles.begin()+i);
                i--; // We need to reprocess this position
            }
        }
    }
    else
    { // We are NOT in the UI thread. We execute the command via the UI thread:
        SUIThreadCommand cmdIn;
        SUIThreadCommand cmdOut;
        cmdIn.cmdId=SCINTILLA_CONSOLES_SIMULATION_ENDED_SCUITHREADCMD;
        App::uiThread->executeCommandViaUiThread(&cmdIn,&cmdOut);
    }
}

CConsoleInitInfo* CScintillaConsoleContainer::getConsoleInfoFromHandle(int handle)
{
    for (size_t i=0;i<_allUninitializedConsoles.size();i++)
    {
        if (_allUninitializedConsoles[i]->getHandle()==handle)
            return(_allUninitializedConsoles[i]);
    }
    return(NULL);
}

CScintillaConsoleDlg* CScintillaConsoleContainer::getConsoleFromHandle(int handle)
{
    for (size_t i=0;i<_allConsoles.size();i++)
    {
        if (_allConsoles[i]->info->getHandle()==handle)
            return(_allConsoles[i]);
    }
    return(NULL);
}

bool CScintillaConsoleContainer::isConsoleHandleValid(int handle)
{
    for (size_t i=0;i<_allConsoles.size();i++)
    {
        if (_allConsoles[i]->info->getHandle()==handle)
            return(true);
    }
    for (size_t i=0;i<_allUninitializedConsoles.size();i++)
    {
        if (_allUninitializedConsoles[i]->_consoleHandle==handle)
            return(true);
    }
    return(false);
}

int CScintillaConsoleContainer::addConsoleInfo(CConsoleInitInfo* consoleInfo)
{
    consoleInfo->setHandle(_nextConsoleID++);

    _allUninitializedConsoles.push_back(consoleInfo);
    return(_nextConsoleID-1);
}

bool CScintillaConsoleContainer::consoleSetShowState(int handle,bool show)
{
    for (size_t i=0;i<_allConsoles.size();i++)
    {
        if (_allConsoles[i]->info->getHandle()==handle)
        {
            _allConsoles[i]->info->setShowState(show);
            return(true);
        }
    }
    for (size_t i=0;i<_allUninitializedConsoles.size();i++)
    {
        if (_allUninitializedConsoles[i]->_consoleHandle==handle)
        {
            _allUninitializedConsoles[i]->_manuallySetShowState=show;
            return(true);
        }
    }
    return(false);
}

bool CScintillaConsoleContainer::removeConsole(int handle)
{
    for (size_t i=0;i<_allConsoles.size();i++)
    {
        if (_allConsoles[i]->info->getHandle()==handle)
        {
            _allConsoles[i]->setCloseRequest();
            return(true);
        }
    }
    for (size_t i=0;i<_allUninitializedConsoles.size();i++)
    {
        if (_allUninitializedConsoles[i]->_consoleHandle==handle)
        {
            delete _allUninitializedConsoles[i];
            _allUninitializedConsoles.erase(_allUninitializedConsoles.begin()+i);
            return(true);
        }
    }
    return(false);
}

void CScintillaConsoleContainer::handleConsoles()
{
    if (VThread::isCurrentThreadTheUiThread())
    { // we are in the UI thread. We execute the command now:
        // 1. Handle console opening:
        for (int i=0;i<int(_allUninitializedConsoles.size());i++)
        {
            CScintillaConsoleDlg* c=new CScintillaConsoleDlg(_allUninitializedConsoles[i],App::mainWindow);
            c->qtInitialize();
            c->show();
            _allConsoles.push_back(c);
            _allUninitializedConsoles.erase(_allUninitializedConsoles.begin()+i);
            i--; // reprocess this position
        }

        // 2. Handle console closing:
        for (int i=0;i<int(_allConsoles.size());i++)
        {
            if (_allConsoles[i]->getCloseRequested())
            {
                delete _allConsoles[i];
                _allConsoles.erase(_allConsoles.begin()+i);
                i--; // reprocess this position
            }
        }

        // 3. Handle text update and other (important for threads that printed text):
        for (size_t i=0;i<_allConsoles.size();i++)
            _allConsoles[i]->handleTextUpdate();
    }
    else
    { // We are NOT in the UI thread. We execute the command via the UI thread:
        SUIThreadCommand cmdIn;
        SUIThreadCommand cmdOut;
        cmdIn.cmdId=SCINTILLA_CONSOLES_HANDLE_CONSOLE_SCUITHREADCMD;
        App::uiThread->executeCommandViaUiThread(&cmdIn,&cmdOut);
    }
}

void CScintillaConsoleContainer::hideOrShowAll(bool showState)
{
    for (size_t i=0;i<_allConsoles.size();i++)
        _allConsoles[i]->showOrHideDlg(showState);
}
