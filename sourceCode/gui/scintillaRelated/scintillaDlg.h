
// Some parts of this code were inspired from Robert Umbehant's personal c compiler
// (http://www.codeproject.com/KB/cpp/Personal_C___Compiler.aspx)

#pragma once

#include "vrepMainHeader.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscistyle.h>
#include <QDialog>

class CScintillaDlg : public QDialog
{
    Q_OBJECT

public:
    CScintillaDlg(int scriptType,bool scriptIsThreaded,QWidget* pParent = NULL, Qt::WindowFlags f= 0);
    virtual ~CScintillaDlg();

    void closeEvent(QCloseEvent *event);

    void setOperational();
    void setShowWindow(bool show);
    void initPhase1(int posAndSize[4]);
    void initPhase2(const char* scriptText,const std::vector<int>* foldingInfo);
    void setWindowTitleText(const std::string& title);

    void setColorsAndMainStyles(int scriptType);
    void setScriptID(int id);
    int _scriptID;
    int _scriptType;
    bool _scriptIsThreaded;
    int _lastSetStyle;

    void _setAStyle(int style,unsigned int fore,unsigned int back=VRGB(0,0,0),int size=-1,const char *face=0);
    void _setKeywords();
    void _prepAutoCompletionList(const std::string& txt);
    std::string _getCallTip(const char* txt);


    bool _operational;
    void InitialiseEditor();
    void _findText(const char* txt,bool caseSensitive);
    std::vector<int> _unfold();
    void _fold(const std::vector<int>& foldingSt);
    int _fold_getHighestIndex(std::vector<int>& foldingState);
    void _fold_overwriteFromIndex(std::vector<int>& foldingState,int index,int newValue);
    std::vector<int> getFoldingInfo();


    void getWindowPosAndSize(int posAndSize[4]);

    std::string _autoCompletionList;

    std::vector<int> _tempFoldingThing;

    QsciScintilla* _scintillaObject;

    static Qt::WindowFlags dialogStyle;

public slots:
    void _onFind();
    void _charAdded(int charAdded);
    void _modified(int, int, const char *, int, int, int, int, int, int, int);
    void _updateUi(int updated);

};

