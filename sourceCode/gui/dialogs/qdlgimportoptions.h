
#ifndef QDLGIMPORTOPTIONS_H
#define QDLGIMPORTOPTIONS_H

#include "vDialog.h"

namespace Ui {
    class CQDlgImportOptions;
}

class CQDlgImportOptions : public VDialog
{
    Q_OBJECT

public:
    explicit CQDlgImportOptions(QWidget *parent = 0);
    ~CQDlgImportOptions();

    void cancelEvent();
    void okEvent();

    void refresh();

    int sizeIndex; // 0=m, 1=ft, 2=10cm, 3=in, 4=cm, 5=mm
    float scaleF;
    bool zIsUp;
    float rotX;

    float totalScale;

private slots:
    void on_qqOk_accepted();

    void on_qqM_clicked();

    void on_qqIn_clicked();

    void on_qqCm_clicked();

    void on_qqMm_clicked();

    void on_qqY_clicked();

    void on_qqZ_clicked();

    void on_qqFt_clicked();

    void on_qq10Cm_clicked();

private:
    Ui::CQDlgImportOptions *ui;
};

#endif // QDLGIMPORTOPTIONS_H
