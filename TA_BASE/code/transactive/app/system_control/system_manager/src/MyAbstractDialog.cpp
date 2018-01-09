/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ProcessParamsDlg.cpp $
* @author:  Dominique Beauregard
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* ProcessParamsDlg is associated with the dialog box allowing operators
* to set the processes' runtime parameters. In the current version, it
* only allows the debug level of processes to be changed.
*
*/
#if 0
#include "ProcessParamsDlg.h"
#include "SystemManagerUtilities.h"
#include "app/system_control/system_manager/pmodel/src/ProcessParamsDlgModel.h"
#include <qpushbutton.h>


namespace TA_Base_App
{
    ProcessParamsDlg::ProcessParamsDlg(TA_Base_Bus::ITransActiveApp* app, ProcessParamsDlgModel* model)
        : TA_Base_Bus::AbstractDialog(app, "ProcessParamsDlg::IDD", NULL),
          m_model(model)
    {
        ui.setupUi(this);
        setupSlotConnection();
        m_model->setView(this);
        loadFromModel();
    }


    ProcessParamsDlg::~ProcessParamsDlg()
    {
        m_model->setView(NULL);
    }


    void ProcessParamsDlg::setupSlotConnection()
    {
        connect(ui.debugLevels, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
        connect(this, SIGNAL(set_ok_enabled_signal(bool)), ui.buttonBox->button(QDialogButtonBox::Ok), SLOT(setEnabled(bool)));
        connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    }


    void ProcessParamsDlg::loadFromModel()
    {
        QStringList debuglevels = SystemManagerUtilities::convertTo<QStringList>(m_model->ui.debug_levels);
        ui.debugLevels->addItems(debuglevels);
        ui.debugLevels->setCurrentIndex(m_model->ui.index);
    }


    void ProcessParamsDlg::show()
    {
        QDialog::exec();
    }


    void ProcessParamsDlg::on_model_set_ok_enabled(bool enable)
    {
        emit set_ok_enabled_signal(enable);
    }


    void ProcessParamsDlg::accept()
    {
        QDialog::accept();
        m_model->onAccept();
    }


    void ProcessParamsDlg::reject()
    {
        QDialog::reject();
        m_model->onReject();
    }


    void ProcessParamsDlg::onCurrentIndexChanged(int index)
    {
        m_model->onCurrentIndexChanged(index);
    }
}
#endif
