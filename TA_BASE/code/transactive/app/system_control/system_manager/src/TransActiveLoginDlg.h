/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/TransActiveLoginDlg.h $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the dialog used to allow the user to enter their login details. IT will then attempt to authenticate
  */
#pragma once
#include "ui_TransActiveLoginDlg.h"
#include "AbstructQDialogTemplate.h"
#include "app/system_control/system_manager/pmodel/src/ITransActiveLoginDlg.h"

namespace TA_Base_App
{
    struct TransActiveLoginDlgModel;

    struct TransActiveLoginDlg : AbstructQDialogTemplate<ITransActiveLoginDlg, TransActiveLoginDlgModel, Ui::TransActiveLoginDlg>
    {
        Q_OBJECT

    public:

        TransActiveLoginDlg(TransActiveLoginDlgModel* model);
        virtual void loadFromModel();
        virtual void setupSignalSlotConnection();
        virtual void onModelSetProfileEnabled(bool enabled);
        virtual void onModelSetProfiles(const std::vector<std::string>& profiles);
        virtual void onModelSetProfileIndex(int index);
        virtual void onModelSetLocationEnabled(bool enabled);
        virtual void onModelSetLocations(const std::vector<std::string>& locations);
        virtual void onModelSetLocationIndex(int index);
        virtual void onModelSetPasswordEnabled(bool enabled);

    private slots:

        void onNameIndexChanged(int);
        void onProfileIndexChanged(int);
        void onLocationIndexChanged(int);
        void onPasswordChanged(const QString& text);
        void setProfileEnabledSlot(bool enabled);
        void setProfilesSlot(const std::vector<std::string>& profiles);
        void setProfileIndexSlot(int index);
        void setLocationEnabledSlot(bool enabled);
        void setLocationsSlot(const std::vector<std::string>& locations);
        void setLocationIndexSlot(int index);
        void setPasswordEnabledSlot(bool enabled);

    signals:

        void setProfileEnabledSignal(bool enabled);
        void setProfilesSignal(const std::vector<std::string>& profiles);
        void setProfileIndexSignal(int index);
        void setLocationEnabledSignal(bool enabled);
        void setLocationsSignal(const std::vector<std::string>& locations);
        void setLocationIndexSignal(int index);
        void setPasswordEnabledSignal(bool enabled);
    };
}
