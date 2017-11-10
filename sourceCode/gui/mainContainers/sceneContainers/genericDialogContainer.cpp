
#include "vrepMainHeader.h"
#include "genericDialogContainer.h" 

CGenericDialogContainer::CGenericDialogContainer()
{
}


CGenericDialogContainer::~CGenericDialogContainer()
{
    removeAll(false);
}

void CGenericDialogContainer::emptySceneProcedure()
{ // don't do anything here!!
//  removeAll(true);
}

void CGenericDialogContainer::simulationAboutToStart()
{
    removeAll(true);
}

void CGenericDialogContainer::simulationEnded()
{
    removeAll(true);
}

void CGenericDialogContainer::renderYour3DStuff(CViewableBase* renderingObject,int displayAttrib)
{

}

void CGenericDialogContainer::removeAll(bool onlyDialogsCreatedInMainOrChilds)
{
    if (!onlyDialogsCreatedInMainOrChilds)
    {
        for (int i=0;i<int(_allDialogs.size());i++)
            delete _allDialogs[i];
        _allDialogs.clear();
    }
    else
    {
        for (int i=0;i<int(_allDialogs.size());i++)
        {
            if (_allDialogs[i]->getCreatedInMainOrChildScript())
            {
                removeDialogFromID(_allDialogs[i]->getGenericDialogID());
                i=-1; // ordering might have changed
            }
        }
    }
}

int CGenericDialogContainer::addDialog(CGenericDialog* theDialog)
{
    int theID=0;
    while (getDialogFromID(theID)!=NULL)
        theID++;
    theDialog->setGenericDialogID(theID);
    _allDialogs.push_back(theDialog);
    return(theID);
}

CGenericDialog* CGenericDialogContainer::getDialogFromID(int theID)
{
    for (int i=0;i<int(_allDialogs.size());i++)
    {
        if (_allDialogs[i]->getGenericDialogID()==theID)
            return(_allDialogs[i]);
    }
    return(NULL);   
}

void CGenericDialogContainer::buttonPressed(int blockID,int buttonID)
{
    for (int i=0;i<int(_allDialogs.size());i++)
        _allDialogs[i]->buttonPressed(blockID,buttonID);
}

void CGenericDialogContainer::textChanged(int blockID,int buttonID,const char* newTxt)
{
    for (int i=0;i<int(_allDialogs.size());i++)
        _allDialogs[i]->textChanged(blockID,buttonID,newTxt);
}

int CGenericDialogContainer::getDialogResult(int dlgHandle)
{
    CGenericDialog* it=getDialogFromID(dlgHandle);
    if (it==NULL)
        return(-1);
    return(it->getDialogResult());
}

bool CGenericDialogContainer::getDialogString(int dlgHandle,std::string& theString)
{
    CGenericDialog* it=getDialogFromID(dlgHandle);
    if (it==NULL)
        return(false);
    theString=it->getDialogString();
    return(true);
}

bool CGenericDialogContainer::removeDialogFromID(int theID)
{
    for (int i=0;i<int(_allDialogs.size());i++)
    {
        if (_allDialogs[i]->getGenericDialogID()==theID)
        {
            delete _allDialogs[i];
            _allDialogs.erase(_allDialogs.begin()+i);
            return(true);
        }
    }
    return(false);
}
