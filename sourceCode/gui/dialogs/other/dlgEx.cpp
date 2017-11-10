
#include "vrepMainHeader.h"
#include "dlgEx.h"
#include "app.h"
#include <QLineEdit>

int CDlgEx::doTransparencyCounter=0;

CDlgEx::CDlgEx(QWidget* pParent) : VDialog(pParent)
{
    _wasTransparent=false;
    _markedForDestruction=false;
    doTransparencyCounter++;
    _transparencyCounter=doTransparencyCounter;
    initializationEvent();
}
CDlgEx::CDlgEx(QWidget* pParent,Qt::WindowFlags specialFlags) : VDialog(pParent,specialFlags)
{
    _wasTransparent=false;
    _markedForDestruction=false;
    doTransparencyCounter++;
    _transparencyCounter=doTransparencyCounter;
    initializationEvent();
}

bool CDlgEx::event(QEvent* event)
{
    if (App::userSettings->allowTransparentDialogs)
    {
        if (_transparencyCounter!=doTransparencyCounter)
        {
            setWindowOpacity(App::userSettings->dialogTransparencyFactor);
            _transparencyCounter=doTransparencyCounter;
            _wasTransparent=true;
        }
        else
        {
            if (event->type()==QEvent::WindowActivate)//FocusIn)
            {
                setWindowOpacity(1.0f);
                _wasTransparent=false;
            }
            if (event->type()==QEvent::WindowDeactivate)//FocusOut)
            {
                setWindowOpacity(App::userSettings->dialogTransparencyFactor);
                _wasTransparent=true;
            }
        }
    }
    else
    {
        if (_wasTransparent)
            setWindowOpacity(1.0f);
        _wasTransparent=false;
    }
    return(QDialog::event(event));
}

CDlgEx::~CDlgEx()
{
}

void CDlgEx::refresh()
{
}

void CDlgEx::dialogCallbackFunc(const SUIThreadCommand* cmdIn,SUIThreadCommand* cmdOut)
{
}

bool CDlgEx::needsDestruction()
{
    return(_markedForDestruction);
}

bool CDlgEx::doesInstanceSwitchRequireDestruction()
{
    return(false); // default behaviour
}


void CDlgEx::cancelEvent()
{ // We just hide the dialog and destroy it at next rendering pass
#ifndef LIN_VREP
    showDialog(false); // on Linux it seems that once a window is hidden, its position becomes (0,0)!!! So we don't hide it on Linux, we just destroy it later!
#endif
    _markedForDestruction=true;
    App::setToolbarRefreshFlag();
}

void CDlgEx::okEvent()
{
}

void CDlgEx::initializationEvent()
{
    defaultDialogInitializationRoutine();
    refresh();
}

QLineEdit* CDlgEx::getSelectedLineEdit()
{
    QList<QLineEdit*> wl=((QWidget*)this)->findChildren<QLineEdit*>(QString());
    for (int i=0;i<wl.size();i++)
    {
        if (wl[i]->selectedText().size()>0)
            return(wl[i]);
    }
    return(NULL);
}

void CDlgEx::selectLineEdit(QLineEdit* edit)
{
    if (edit!=NULL)
    {
        edit->setFocus();
        edit->selectAll();
    }
}
