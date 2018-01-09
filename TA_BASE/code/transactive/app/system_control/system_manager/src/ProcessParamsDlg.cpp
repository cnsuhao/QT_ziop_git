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
#include "ProcessParamsDlg.h"
#include "QtUtilities.h"
#include "ProcessParamsDlgModel.h"
#include <QPushButton>
#include <boost/foreach.hpp>


namespace TA_Base_App
{
    ProcessParamsDlg::ProcessParamsDlg(ProcessParamsDlgModel* model)
        : AbstructQDialogTemplate(model)
    {
        setupDialog(this);
    }


    void ProcessParamsDlg::setupSignalSlotConnection()
    {
        connect(this, SIGNAL(setIndexSignal(int)), this, SLOT(setIndexSlot(int)));
        connect(ui.debugLevels, SIGNAL(currentIndexChanged(int)), this, SLOT(onDebugLevelsIndexChanged(int)));
    }


    void ProcessParamsDlg::onSwitchLanguage()
    {
        AbstructQDialogTemplate::onSwitchLanguage();
        retranslateDebugLevels();
    }


    void ProcessParamsDlg::loadFromModel()
    {
        ui.debugLevels->addItems(m_tr.tr(m_model->ui.debug_levels));
        ui.debugLevels->setCurrentIndex(m_model->ui.index);
        ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_model->ui.ok_enabled);
    }


    void ProcessParamsDlg::onDebugLevelsIndexChanged(int index)
    {
        m_model->ui.index = index;
        m_model->onDebugLevelsIndexChanged(index);
    }


    void ProcessParamsDlg::retranslateDebugLevels()
    {
        for (size_t i = 0; i < ui.debugLevels->count(); ++i)
        {
            ui.debugLevels->setItemText(i, m_tr.tr(m_model->ui.debug_levels[i]));
        }
    }


    void ProcessParamsDlg::setIndexSlot(int index)
    {
        ui.debugLevels->setCurrentIndex(index);
    }


    void ProcessParamsDlg::onModelSetIndex(int index)
    {
        emit setIndexSignal(index);
    }
}
