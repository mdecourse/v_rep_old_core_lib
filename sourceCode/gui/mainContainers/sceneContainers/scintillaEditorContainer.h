
#pragma once

#include "vrepMainHeader.h"
#include "mainCont.h"
#include "scintillaEditor.h"
#include "uiThread.h"

class CScintillaEditorContainer
{
public:
    CScintillaEditorContainer();
    virtual ~CScintillaEditorContainer();

    // From UI thread:
    void executeCommand(SUIThreadCommand* cmdIn,SUIThreadCommand* cmdOut);

    // From any thread;
    bool openEditorForScript(int scriptID);
    void closeAllEditors();
    void showOrHideAllEditors(bool show);
    bool closeEditor(int scriptID);
    void updateWindowsExceptContentText();
    void applyChanges(bool getFoldingInfo);
    void requestClosing(void* dialogPointer);
    bool areWindowsOpen(); // needed by undo/redo

protected:
    std::vector<CScintillaEditor*> _allEditors;
};
