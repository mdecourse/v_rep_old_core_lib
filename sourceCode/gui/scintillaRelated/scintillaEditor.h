
#pragma once

#include "vrepMainHeader.h"

class CScintillaDlg;

class CScintillaEditor
{
public:
    CScintillaEditor();
    virtual ~CScintillaEditor();


    bool initialize(int scriptID);
    void newSceneProcedure();
    int getScriptID();
    bool updateWindowExceptContentText();
    bool applyChanges(bool getFoldingInfo);
    void requestClosing(void* dialogPointer);

    void showEditorButDontOpenIt();
    void hideEditorButDontCloseIt();

    void getWindowPosAndSize(int posAndSize[4]);

protected:
    void _closeDlgWindow(bool announceChange);
    int _scriptID;
    bool _closeRequest;
    int _hideLevel;
    std::string _currentWindowTitle;
    CScintillaDlg* _dlg;
};
