/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/StopProcessDlg.cpp $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * StopProcessDlg is associated with the dialog box allowing
  * operators to stop a running process.
  *
  */
#include "StopProcessDlg.h"
#include "QtUtilities.h"
#include "app/system_control/system_manager/pmodel/src/StopProcessDlgModel.h"
#include <QPushButton>


namespace TA_Base_App
{

    StopProcessDlg::StopProcessDlg(StopProcessDlgModel* model)
        : AbstructQDialogTemplate(model)
    {
        setupDialog(this);
    }


    void StopProcessDlg::setupSignalSlotConnection()
    {
        qRegisterMetaType< std::set<size_t> >("std::set<size_t>");
        connect(this, SIGNAL(setDisabledSignal(const std::set<size_t>&)), this, SLOT(setDisabledSlot(const std::set<size_t>&)));
    }


    void StopProcessDlg::loadFromModel()
    {
        ui.processes->addItems(QtUtilities::convertTo<QStringList>(m_model->ui.processes));
        QtUtilities::disableItems(ui.processes, m_model->ui.disabled);
    }


    void StopProcessDlg::setDisabledSlot(const std::set<size_t>& indexes)
    {
        QtUtilities::disableItems(ui.processes, indexes);
    }


    void StopProcessDlg::onModelSetDisabled(const std::set<size_t>& indexes)
    {
        emit setDisabledSignal(indexes);
    }
}
