
#ifndef QDLGROTATIONS_H
#define QDLGROTATIONS_H

#include "dlgEx.h"
#include "7Vector.h"

namespace Ui {
    class CQDlgRotations;
}

class QComboBox;

class CQDlgRotations : public CDlgEx
{
    Q_OBJECT

public:
    explicit CQDlgRotations(QWidget *parent = 0);
    ~CQDlgRotations();

    void refresh();

    void cancelEvent();


protected:
    void _enableCoordinatePart(bool enableState,bool anglePart,bool enableButtons,bool alsoRadioButtons);
    void _enableTransformationPart(bool enableState,int partIndex,bool enableButtons,bool alsoRadioButtons);
    void _setDefaultValuesCoordinatePart(bool anglePart,bool alsoRadioButtons);
    void _setDefaultValuesTransformationPart(int partIndex,bool alsoRadioButtons);
    void _setValuesTransformationPart(int partIndex,bool alsoRadioButtons);

    // Coord part
    bool _setCoord_userUnit(float newValueInUserUnit,int index);
    C7Vector _getNewTransf(const C7Vector& transf,float newValueInUserUnit,int index);
    bool _applyCoord();

    // Transf part
    bool _applyTransformation();
    void _transform(C7Vector& tr,bool self);

    static int coordMode; //0=abs,1=rel to parent
    static float rotAngles[3];
    static float translationValues[3];
    static float scalingValues[3];
    static int transfMode; //0=abs,1=rel to parent,2=rel to self

    int lastLastSelectionID;

private slots:
    void on_qqCoordWorld_clicked();

    void on_qqCoordParent_clicked();

    void on_qqCoordAlpha_editingFinished();

    void on_qqCoordBeta_editingFinished();

    void on_qqCoordGamma_editingFinished();

    void on_qqCoordApplyOr_clicked();

    void on_qqTransfWorld_clicked();

    void on_qqTransfParent_clicked();

    void on_qqTransfOwn_clicked();

    void on_qqTransfAlpha_editingFinished();

    void on_qqTransfBeta_editingFinished();

    void on_qqTransfGamma_editingFinished();

    void on_qqTransfApplyOr_clicked();

private:
    Ui::CQDlgRotations *ui;
};

#endif // QDLGROTATIONS_H
