/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/TransActiveLoginDlg.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the dialog used to allow the user to enter their login details. IT will then attempt to authenticate
  */

#include "TransActiveLoginDlg.h"
#include "QtUtilities.h"
#include "app/system_control/system_manager/pmodel/src/TransActiveLoginDlgModel.h"
#include <QPushButton>


namespace TA_Base_App
{
    TransActiveLoginDlg::TransActiveLoginDlg(TransActiveLoginDlgModel* model)
        : AbstructQDialogTemplate(model)
    {
        setupDialog(this);
    }


    void TransActiveLoginDlg::setupSignalSlotConnection()
    {
        connect(ui.names, SIGNAL(currentIndexChanged(int)), this, SLOT(onNameIndexChanged(int)));
        connect(ui.profiles, SIGNAL(currentIndexChanged(int)), this, SLOT(onProfileIndexChanged(int)));
        connect(ui.locations, SIGNAL(currentIndexChanged(int)), this, SLOT(onLocationIndexChanged(int)));
        connect(ui.password, SIGNAL(textChanged(const QString&)), this, SLOT(onPasswordChanged(const QString)));
        connect(this, SIGNAL(setProfileEnabledSignal(bool)), this, SLOT(setProfileEnabledSlot(bool)));
        connect(this, SIGNAL(setProfilesSignal(const std::vector<std::string>&)), this, SLOT(setProfilesSlot(const std::vector<std::string>&)));
        connect(this, SIGNAL(setProfileIndexSignal(int)), this, SLOT(setProfileIndexSlot(int)));
        connect(this, SIGNAL(setLocationEnabledSignal(bool)), this, SLOT(setLocationEnabledSlot(bool)));
        connect(this, SIGNAL(setLocationsSignal(const std::vector<std::string>&)), this, SLOT(setLocationsSlot(const std::vector<std::string>&)));
        connect(this, SIGNAL(setLocationIndexSignal(int)), this, SLOT(setLocationIndexSlot(int)));
        connect(this, SIGNAL(setPasswordEnabledSignal(bool)), this, SLOT(setPasswordEnabledSlot(bool)));
    }


    void TransActiveLoginDlg::loadFromModel()
    {
        ui.names->addItems(QtUtilities::convertToUnicode<QStringList>(m_model->ui.names));
        ui.names->setCurrentIndex(m_model->ui.name_index);
        ui.profiles->addItems(QtUtilities::convertToUnicode<QStringList>(m_model->ui.profiles));
        ui.profiles->setCurrentIndex(m_model->ui.profile_index);
        ui.profiles->setEnabled(m_model->ui.profile_enabled);
        ui.locations->addItems(QtUtilities::convertToUnicode<QStringList>(m_model->ui.locations));
        ui.locations->setCurrentIndex(m_model->ui.location_index);
        ui.locations->setEnabled(m_model->ui.location_enabled);
        ui.password->setText(m_model->ui.password.c_str());
        ui.password->setEnabled(m_model->ui.password_enabled);
        ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_model->ui.ok_enabled);
    }


    void TransActiveLoginDlg::onNameIndexChanged(int index)
    {
        m_model->ui.name_index = index;
        m_model->onNameIndexChanged(index);
    }


    void TransActiveLoginDlg::onProfileIndexChanged(int index)
    {
        m_model->ui.profile_index = index;
        m_model->onProfileIndexChanged(index);
    }


    void TransActiveLoginDlg::onLocationIndexChanged(int index)
    {
        m_model->ui.location_index = index;
        m_model->onLocationIndexChanged(index);
    }


    void TransActiveLoginDlg::onPasswordChanged(const QString& text)
    {
        m_model->ui.password = text.toStdString();
        m_model->onPasswordChanged(text.toStdString());
    }


    void TransActiveLoginDlg::setProfileEnabledSlot(bool enabled)
    {
        ui.profiles->setEnabled(enabled);
    }


    void TransActiveLoginDlg::setProfilesSlot(const std::vector<std::string>& profiles)
    {
        BLOCK_SIGNALS(ui.profiles);
        ui.profiles->clear();
        ui.profiles->addItems(QtUtilities::convertToUnicode<QStringList>(profiles));
    }


    void TransActiveLoginDlg::setProfileIndexSlot(int index)
    {
        BLOCK_SIGNALS(ui.profiles);
        ui.profiles->setCurrentIndex(index);
    }


    void TransActiveLoginDlg::setLocationEnabledSlot(bool enabled)
    {
        ui.locations->setEnabled(enabled);
    }


    void TransActiveLoginDlg::setLocationsSlot(const std::vector<std::string>& locations)
    {
        BLOCK_SIGNALS(ui.locations);
        ui.locations->clear();
        ui.locations->addItems(QtUtilities::convertToUnicode<QStringList>(locations));
    }


    void TransActiveLoginDlg::setLocationIndexSlot(int index)
    {
        BLOCK_SIGNALS(ui.locations);
        ui.locations->setCurrentIndex(index);
    }


    void TransActiveLoginDlg::setPasswordEnabledSlot(bool enabled)
    {
        ui.password->setEnabled(enabled);
    }


    void TransActiveLoginDlg::onModelSetProfileEnabled(bool enabled)
    {
        emit setProfileEnabledSignal(enabled);
    }


    void TransActiveLoginDlg::onModelSetProfiles(const std::vector<std::string>& profiles)
    {
        emit setProfilesSignal(profiles);
    }


    void TransActiveLoginDlg::onModelSetProfileIndex(int index)
    {
        emit setProfileIndexSignal(index);
    }


    void TransActiveLoginDlg::onModelSetLocationEnabled(bool enabled)
    {
        emit setLocationEnabledSignal(enabled);
    }


    void TransActiveLoginDlg::onModelSetLocations(const std::vector<std::string>& locations)
    {
        emit setLocationsSignal(locations);
    }


    void TransActiveLoginDlg::onModelSetLocationIndex(int index)
    {
        emit setLocationIndexSignal(index);
    }


    void TransActiveLoginDlg::onModelSetPasswordEnabled(bool enabled)
    {
        emit setPasswordEnabledSignal(enabled);
    }
}
