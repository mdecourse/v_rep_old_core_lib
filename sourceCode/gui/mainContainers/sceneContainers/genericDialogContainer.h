
#pragma once

#include "vrepMainHeader.h"
#include "mainCont.h"
#include "genericDialog.h"

class CGenericDialogContainer : public CMainCont
{
public:

    CGenericDialogContainer();
    virtual ~CGenericDialogContainer();
    void simulationAboutToStart();
    void simulationEnded();
    void renderYour3DStuff(CViewableBase* renderingObject,int displayAttrib);

    void emptySceneProcedure();
    void removeAll(bool onlyDialogsCreatedInMainOrChilds);
    int addDialog(CGenericDialog* theDialog);
    CGenericDialog* getDialogFromID(int theID);
    bool removeDialogFromID(int theID);
    void buttonPressed(int blockID,int buttonID);
    void textChanged(int blockID,int buttonID,const char* newTxt);

    int getDialogResult(int dlgHandle);
    bool getDialogString(int dlgHandle,std::string& theString);

protected:
    std::vector<CGenericDialog*> _allDialogs;
};
