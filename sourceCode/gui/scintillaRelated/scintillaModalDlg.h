
// Some parts of this code were inspired from Robert Umbehant's personal c compiler
// (http://www.codeproject.com/KB/cpp/Personal_C___Compiler.aspx)

#pragma once

#include "vrepMainHeader.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscistyle.h>
#include <QDialog>

class CScintillaModalDlg : public QDialog
{
    Q_OBJECT

public:
    CScintillaModalDlg(int scriptType,bool scriptIsThreaded,QWidget* pParent = NULL, Qt::WindowFlags f= 0);
    virtual ~CScintillaModalDlg();

    bool initialize(int scriptId,const char* titleText,bool readOnly,bool ignoreSceneLock);
    void makeDialogModal();

protected:
    std::vector<int> _unfold();
    void _fold(const std::vector<int>& foldingState);
    int _fold_getHighestIndex(std::vector<int>& foldingState);
    void _fold_overwriteFromIndex(std::vector<int>& foldingState,int index,int newValue);

    void _findText(const char* txt,bool caseSensitive);
    void _setColorsAndMainStyles();
    int _scriptID;
    int _scriptType;
    bool _scriptIsThreaded;

    void _setAStyle(int style,unsigned int fore,unsigned int back=VRGB(0,0,0),int size=-1,const char *face=0);
    void _setKeywords();
    void _prepAutoCompletionList(const std::string& txt);
    std::string _getCallTip(const char* txt);

    std::string _autoCompletionList;

    QsciScintilla* _scintillaObject;

    std::vector<int> _tempFoldingThing;
public slots:
    void _onFind();
    void _charAdded(int charAdded);
    void _updateUi(int updated);
};

