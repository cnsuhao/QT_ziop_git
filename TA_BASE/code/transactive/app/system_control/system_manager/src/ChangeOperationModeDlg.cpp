/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ChangeOperationModeDlg.cpp $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * ChangeOperationModeDlg is the class associated with the
  * Change Operation Mode dialog.
  *
  */
#include "ChangeOperationModeDlg.h"
#include "ChangeOperationModeDlgModel.h"
#include "QtUtilities.h"
#include <QPushButton>


namespace TA_Base_App
{
    ChangeOperationModeDlg::ChangeOperationModeDlg(ChangeOperationModeDlgModel* model)
        : AbstructQDialogTemplate(model)
    {
        setupDialog(this);
    }


    void ChangeOperationModeDlg::setupSignalSlotConnection()
    {
        qRegisterMetaType< std::set<size_t> >("std::set<size_t>");
        connect(ui.control, SIGNAL(toggled(bool)), this, SLOT(onControlRadioToggled(bool)));
        connect(ui.monitor, SIGNAL(toggled(bool)), this, SLOT(onMonitorRadioToggled(bool)));
        connect(this, SIGNAL(setDisabledSignal(const std::set<size_t>&)), this, SLOT(setDisabledSlot(const std::set<size_t>&)));
        connect(this, SIGNAL(setRadioEnabledSignal(bool)), this, SLOT(setRadioEnabledSlot(bool)));
        connect(this, SIGNAL(setRadioCheckedSingnal(bool, bool)), this, SLOT(setRadioCheckedSlot(bool, bool)));
        connect(this, SIGNAL(setAutoExclusiveSignal(bool)), this, SLOT(setAutoExclusiveSlot(bool)));
    }


    void ChangeOperationModeDlg::loadFromModel()
    {
        ui.processes->addItems(QtUtilities::convertTo<QStringList>(m_model->ui.processes));
        QtUtilities::disableItems(ui.processes, m_model->ui.disabled);
        ui.control->setChecked(m_model->ui.control_checked);
        ui.monitor->setChecked(m_model->ui.monitor_checked);
        ui.control->setEnabled(m_model->ui.radio_enabled);
        ui.monitor->setEnabled(m_model->ui.radio_enabled);
        ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_model->ui.ok_enabled);
    }


    void ChangeOperationModeDlg::onControlRadioToggled(bool checked)
    {
        m_model->ui.control_checked = checked;
        m_model->onControlRadioToggled(checked);
    }


    void ChangeOperationModeDlg::onMonitorRadioToggled(bool checked)
    {
        m_model->ui.monitor_checked = checked;
        m_model->onMonitorRadioToggled(checked);
    }


    void ChangeOperationModeDlg::setDisabledSlot(const std::set<size_t>& indexes)
    {
        QtUtilities::disableItems(ui.processes, indexes);
    }


    void ChangeOperationModeDlg::setAutoExclusiveSlot(bool auto_exclusive)
    {
        ui.control->setAutoExclusive(auto_exclusive);
        ui.monitor->setAutoExclusive(auto_exclusive);
    }


    void ChangeOperationModeDlg::setRadioEnabledSlot(bool enabled)
    {
        ui.control->setEnabled(enabled);
        ui.monitor->setEnabled(enabled);
    }


    void ChangeOperationModeDlg::setRadioCheckedSlot(bool control_checked, bool monitor_checked)
    {
        ui.control->setChecked(control_checked);
        ui.monitor->setChecked(monitor_checked);
    }


    void ChangeOperationModeDlg::onModelSetDisabled(const std::set<size_t>& indexes)
    {
        emit setDisabledSignal(indexes);
    }


    void ChangeOperationModeDlg::onModelSetAutoExclusive(bool auto_exclusive)
    {
        emit setAutoExclusiveSignal(auto_exclusive);
    }


    void ChangeOperationModeDlg::onModelSetRadioEnabled(bool enabled)
    {
        emit setRadioEnabledSignal(enabled);
    }


    void ChangeOperationModeDlg::omModelSetRadioChecked(bool control_checked, bool monitor_checked)
    {
        emit setRadioCheckedSingnal(control_checked, monitor_checked);
    }
}
