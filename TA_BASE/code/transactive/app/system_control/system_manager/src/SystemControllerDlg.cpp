/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemControllerDlg.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #2 $ee;khfjhyg
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the dialog used to allow the user to enter the System Controller host name and
  * port number.
  */

#include "SystemControllerDlg.h"
#include "QtUtilities.h"
#include "app/system_control/system_manager/pmodel//src/SystemControllerDlgModel.h"
#include <QPushButton>


namespace TA_Base_App
{
    SystemControllerDlg::SystemControllerDlg(SystemControllerDlgModel* model)
        : AbstructQDialogTemplate(model)
    {
        setupDialog(this);
    }


    void SystemControllerDlg::setupSignalSlotConnection()
    {
        connect(ui.port, SIGNAL(textChanged(const QString&)), this, SLOT(onPortChanged(const QString&)));
        connect(ui.hostnames, SIGNAL(currentIndexChanged(int)), this, SLOT(onHostnameIndexChanged(int)));
    }


    void SystemControllerDlg::loadFromModel()
    {
        ui.hostnames->addItems(QtUtilities::convertTo<QStringList>(m_model->ui.hostnames));
        ui.hostnames->setCurrentIndex(m_model->ui.hostnames_index);
        ui.port->setText(m_model->ui.port.c_str());
        ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_model->ui.ok_enabled);
    }


    void SystemControllerDlg::onPortChanged(const QString& text)
    {
        m_model->ui.port = text.toStdString();
        m_model->onPortChanged(text.toStdString());
    }


    void SystemControllerDlg::onHostnameIndexChanged(int index)
    {
        m_model->ui.hostnames_index = index;
        m_model->onHostnameIndexChanged(index);
    }
}
