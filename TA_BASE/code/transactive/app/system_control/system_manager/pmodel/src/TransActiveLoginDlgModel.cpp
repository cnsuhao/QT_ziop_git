#include "TransActiveLoginDlgModel.h"
#include "ITransActiveLoginDlg.h"
#include "TransActiveLoginDlgModelService.h"
#include "ForeachCall.h"
#include "StdUtilities.h"


namespace TA_Base_App
{
    std::vector<std::string> TransActiveLoginSynData::names;
    std::string TransActiveLoginDlgModel::m_currentLocationDescription;


    TransActiveLoginDlgModel::TransActiveLoginDlgModel(TransActiveLoginDlgModelService* service)
        : AbstractModel(service)
    {
        setupModel(this);
    }


    void TransActiveLoginDlgModel::initSyncData()
    {
        if (ui.names.empty())
        {
            ui.names = pm.names = m_service->getOperators();
        }

        if (m_currentLocationDescription.empty())
        {
            m_currentLocationDescription = m_service->getCurrentLocation();
        }

        ui.name_index = pm.name_index = (1 == ui.names.size()) ? 0 : -1;
        ui.profile_enabled = pm.profile_enabled = false;
        ui.profile_index = pm.profile_index = -1;
        ui.location_enabled = pm.location_enabled = false;
        ui.location_index = pm.location_index = -1;
        ui.password_enabled = pm.password_enabled = false;
        ui.ok_enabled = pm.ok_enabled = false;
    }


    void TransActiveLoginDlgModel::onNameIndexChanged(int index)
    {
        if (index != pm.name_index)
        {
            ui.profile_enabled = true;
            ui.profiles = m_service->getProfiles(ui.names[index]);
            ui.profile_index = (1 == ui.profiles.size()) ? 0 : -1;

            if (ui.profile_index != -1)
            {
                notifyChangesToView(pm, ui);
                pm = ui;
                pm.profile_index = -1;
                onProfileIndexChanged(ui.profile_index);
            }
            else
            {
                ui.location_enabled = false;
                ui.locations.clear();
                ui.password_enabled = false;
                ui.ok_enabled = false;
                notifyChangesToView(pm, ui);
                pm = ui;
            }
        }
    }


    void TransActiveLoginDlgModel::onProfileIndexChanged(int index)
    {
        if (index != pm.profile_index)
        {
            const std::string& profile = ui.profiles[index];
            ui.location_enabled = m_service->requiresLocationToBeSelected(profile);
            bool shouldEnablePassword = false;

            if (ui.location_enabled)
            {
                ui.locations = m_service->getLocations(profile);
                ui.location_index = -1;

                if (ui.locations.empty())
                {
                    ui.location_enabled = true;
                    shouldEnablePassword = true;
                }
                else
                {
                    ui.location_index = (1 == ui.locations.size()) ? 0 : -1;

                    if (-1 == ui.location_index && !m_currentLocationDescription.empty())
                    {
                        ui.location_index = StdUtilities::findIndex(ui.locations, m_currentLocationDescription);
                    }

                    if (ui.location_index != -1)
                    {
                        shouldEnablePassword = true;
                    }
                }
            }
            else
            {
                ui.locations.clear();
                ui.location_index = -1;
                shouldEnablePassword = true;
            }

            if (shouldEnablePassword)
            {
                ui.password_enabled = true;
                ui.ok_enabled = calculateOkEnabled(ui);;
            }
            else
            {
                ui.ok_enabled = ui.password_enabled = false;
            }

            notifyChangesToView(pm, ui);
            pm = ui;
        }
    }


    void TransActiveLoginDlgModel::onLocationIndexChanged(int index)
    {
        if (index != pm.location_index)
        {
            ui.password_enabled = true;
            ui.ok_enabled = calculateOkEnabled(ui);;
            notifyChangesToView(pm, ui);
            pm = ui;
        }
    }


    void TransActiveLoginDlgModel::onPasswordChanged(const std::string& password)
    {
        ui.ok_enabled = calculateOkEnabled(ui);
        notifyChangesToView(pm, ui);
        pm = ui;
    }


    bool TransActiveLoginDlgModel::calculateLocationEnabled(SyncData& data)
    {
        if (data.profile_index != -1)
        {
            return m_service->requiresLocationToBeSelected(data.profiles[data.profile_index]);
        }

        return false;
    }


    bool TransActiveLoginDlgModel::calculateOkEnabled(SyncData& data)
    {
        return (8 <= data.password.size());
    }


    void TransActiveLoginDlgModel::notifyChangesToView(const SyncData& o, const SyncData& n)
    {
        NOTIFY(profile_enabled, &ITransActiveLoginDlg::onModelSetProfileEnabled);
        NOTIFY(profile_index, &ITransActiveLoginDlg::onModelSetProfileIndex);
        NOTIFY(location_enabled, &ITransActiveLoginDlg::onModelSetLocationEnabled);
        NOTIFY(location_index, &ITransActiveLoginDlg::onModelSetLocationIndex);
        NOTIFY(password_enabled, &ITransActiveLoginDlg::onModelSetPasswordEnabled);
        NOTIFY(ok_enabled, &ITransActiveLoginDlg::onModelEnableOkButton);

        if (o.profiles != n.profiles)
        {
            foreachCall(m_views, &ITransActiveLoginDlg::onModelSetProfiles, n.profiles);
            foreachCall(m_views, &ITransActiveLoginDlg::onModelSetProfileIndex, n.profile_index);
        }

        if (o.locations != n.locations)
        {
            foreachCall(m_views, &ITransActiveLoginDlg::onModelSetLocations, n.locations);
            foreachCall(m_views, &ITransActiveLoginDlg::onModelSetLocationIndex, n.location_index);
        }
    }


    void TransActiveLoginDlgModel::onAccept()
    {
        AbstractModel::onAccept();
        m_service->onLogin("", "", "", "");
    }
}
