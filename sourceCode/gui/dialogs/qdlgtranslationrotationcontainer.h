
#ifndef QDLGTRANSLATIONROTATIONCONTAINER_H
#define QDLGTRANSLATIONROTATIONCONTAINER_H

#include "dlgEx.h"
#include <QVBoxLayout>

namespace Ui {
    class CQDlgTranslationRotationContainer;
}

class CQDlgTranslationRotationContainer : public CDlgEx
{
    Q_OBJECT

public:
    explicit CQDlgTranslationRotationContainer(QWidget *parent = 0);
    ~CQDlgTranslationRotationContainer();

    void refresh();
    void cancelEvent();
    void okEvent();

    QVBoxLayout* bl;

    int currentPage;
    int desiredPage;

    CDlgEx* pageDlgs[2];
    int originalHeights[2];

private slots:
    void on_qqTranslationMode_clicked();

    void on_qqRotationMode_clicked();

private:
    Ui::CQDlgTranslationRotationContainer *ui;
};

#endif // QDLGTRANSLATIONROTATIONCONTAINER_H
