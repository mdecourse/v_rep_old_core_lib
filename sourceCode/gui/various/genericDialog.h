
#pragma once

#include "softButton.h"

class CGenericDialog
{
public:
    CGenericDialog(const char* titleText,const char* mainText,int dialogType,bool modal,
        const char* initialText=NULL,const float titleColor[6]=NULL,const float dialogColor[6]=NULL);
    virtual ~CGenericDialog();
    void setGenericDialogID(int theNewID);
    int getGenericDialogID();
    int getDialogBlockHandle();
    void setModal(bool modal);
    bool getModal();
    void setPauseActive(bool pauseActive);
    void buttonPressed(int blockID,int buttonID);
    void textChanged(int blockID,int buttonID,const char* newTxt);
    void setCreatedInMainOrChildScript(bool c);
    bool getCreatedInMainOrChildScript();

    int getDialogResult();
    std::string getDialogString();

protected:
    void _applyColor(CSoftButton* button,float col[6]);
    int _buttonBlockID;
    int _genericDialogID;
    bool _modal;
    int _dialogType;
    std::string _editBoxText;
    int _editBoxButtonID;
    int _button1ID;
    int _button2ID;
    int _pressedButton;
    bool _createdInMainOrChildScript;
};
