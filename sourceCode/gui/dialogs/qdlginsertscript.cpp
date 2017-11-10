
#include "vrepMainHeader.h"
#include "qdlginsertscript.h"
#include "ui_qdlginsertscript.h"
#include "v_rep_internal.h"
#include "v_repStrings.h"
#include "app.h"

CQDlgInsertScript::CQDlgInsertScript(QWidget *parent) :
    VDialog(parent,QT_MODAL_DLG_STYLE),
    ui(new Ui::CQDlgInsertScript)
{
    ui->setupUi(this);
}

CQDlgInsertScript::~CQDlgInsertScript()
{
    delete ui;
}

void CQDlgInsertScript::cancelEvent()
{
    defaultModalDialogEndRoutine(false);
}

void CQDlgInsertScript::okEvent()
{
    scriptType=ui->qqCombo->itemData(ui->qqCombo->currentIndex()).toInt();
    defaultModalDialogEndRoutine(true);
}

void CQDlgInsertScript::initialize()
{
    ui->qqCombo->clear();
    ui->qqCombo->addItem(strTranslate(IDS_MAIN_SCRIPT),QVariant(sim_scripttype_mainscript));
    ui->qqCombo->addItem(strTranslate(IDS_CHILD_SCRIPT_UNTHREADED),QVariant(sim_scripttype_childscript));
    ui->qqCombo->addItem(strTranslate(IDS_CHILD_SCRIPT_THREADED),QVariant(sim_scripttype_childscript|sim_scripttype_threaded));
    if (App::userSettings->enableOldCustomContactHandlingEdition)
        ui->qqCombo->addItem(strTranslate(IDS_CONTACT_CALLBACK_SCRIPT),QVariant(sim_scripttype_contactcallback));
    if (App::userSettings->enableOldJointCallbackScriptEdition)
        ui->qqCombo->addItem(strTranslate(IDS_JOINT_CTRL_CALLBACK_SCRIPT),QVariant(sim_scripttype_jointctrlcallback));
    if (App::userSettings->enableOldGeneralCallbackScriptEdition)
        ui->qqCombo->addItem(strTranslate(IDS_GENERAL_CALLBACK_SCRIPT),QVariant(sim_scripttype_generalcallback));
    ui->qqCombo->addItem(strTranslate(IDS_CUSTOMIZATION_SCRIPT),QVariant(sim_scripttype_customizationscript));
}

void CQDlgInsertScript::on_qqOkCancel_accepted()
{
    scriptType=ui->qqCombo->itemData(ui->qqCombo->currentIndex()).toInt();
    defaultModalDialogEndRoutine(true);
}

void CQDlgInsertScript::on_qqOkCancel_rejected()
{
    defaultModalDialogEndRoutine(false);
}
