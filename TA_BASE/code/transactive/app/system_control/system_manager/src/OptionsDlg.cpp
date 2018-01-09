/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/OptionsDlg.cpp $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * OptionsDlg is associated with the dialog that allows
  * operators to set the data refresh rate.
  *
  */
#include "OptionsDlg.h"
#include "QtUtilities.h"
#include "OptionsDlgModel.h"
#include <QPushButton>


namespace TA_Base_App
{
    OptionsDlg::OptionsDlg(OptionsDlgModel* model)
        : AbstructQDialogTemplate(model)
    {
        setupDialog(this);
    }


    void OptionsDlg::setupSignalSlotConnection()
    {
        connect(ui.refreshRates, SIGNAL(currentIndexChanged(int)), this, SLOT(onRefreshRatesIndexChanged(int)));
    }


    void OptionsDlg::loadFromModel()
    {
        ui.refreshRates->addItems(QtUtilities::convertTo<QStringList>(m_model->ui.refreshRates));
        ui.refreshRates->setCurrentIndex(m_model->ui.index);
        ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_model->ui.ok_enabled);
    }


    void OptionsDlg::onRefreshRatesIndexChanged(int index)
    {
        m_model->ui.index = index;
        m_model->onRefreshRatesIndexChanged(index);
    }
}
