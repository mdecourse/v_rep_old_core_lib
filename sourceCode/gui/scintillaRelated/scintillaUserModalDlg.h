
// Some parts of this code were inspired from Robert Umbehant's personal c compiler
// (http://www.codeproject.com/KB/cpp/Personal_C___Compiler.aspx)

#pragma once

#include "vrepMainHeader.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerlua.h>
//#include <Qsci/qscilexerjava.h>
//#include <Qsci/qscilexerpython.h>
//#include <Qsci/qscilexercpp.h>
//#include <Qsci/qscilexerhtml.h>
//#include <Qsci/qscilexeroctave.h>
#include <Qsci/qscistyle.h>
#include <QDialog>

struct SScintillaUserKeyword {
    std::string keyword;
    std::string callTip;
    bool autocomplete;
};


class CScintillaUserModalDlg : public QDialog
{
    Q_OBJECT

public:
    CScintillaUserModalDlg(const std::string& xmlInfo,QWidget* pParent = NULL, Qt::WindowFlags f= 0);
    virtual ~CScintillaUserModalDlg();

    bool initialize(const char* text);
    std::string makeDialogModal();

    void getSizeAndPosition(int s[2],int p[2]);

protected:
    void _getColorFromString(const char* txt,unsigned int& col);
    void _findText(const char* txt,bool caseSensitive);
    void _setColorsAndMainStyles();

    void _setAStyle(int style,unsigned int fore,unsigned int back=VRGB(0,0,0),int size=-1,const char *face=0);
    void _prepAutoCompletionList(const std::string& txt);
    std::string _getCallTip(const char* txt);

    std::vector<SScintillaUserKeyword> _keywords1;
    std::vector<SScintillaUserKeyword> _keywords2;
    std::string _autoCompletionList;
    std::string _allKeywords1;
    std::string _allKeywords2;
    bool _editable;
    bool _searchable;
    int _tabWidth;
    int _size[2];
    int _position[2];
    std::string _title;
    bool _isLua;
    bool _useVrepKeywords;
    unsigned int _textColor;
    unsigned int _backgroundColor;
    unsigned int _selectionColor;
    unsigned int _keywords1Color;
    unsigned int _keywords2Color;

    unsigned int _commentColor;
    unsigned int _numberColor;
    unsigned int _stringColor;
    unsigned int _characterColor;
    unsigned int _operatorColor;
    unsigned int _preprocessorColor;
    unsigned int _identifierColor;
    unsigned int _wordColor;
//    unsigned int _word2Color;
//    unsigned int _word3Color;
    unsigned int _word4Color;

    QsciScintilla* _scintillaObject;

public slots:
    void _onFind();
    void _charAdded(int charAdded);
    void _updateUi(int updated);
};

