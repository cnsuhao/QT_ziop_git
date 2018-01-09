#include "OptionsDlgModel.h"
#include "IOptionsDlg.h"
#include "OptionsDlgModelService.h"
#include "ForeachCall.h"
#include <boost/lexical_cast.hpp>


namespace TA_Base_App
{
    std::vector<std::string> OptionsSynData::refreshRates;


    OptionsDlgModel::OptionsDlgModel(OptionsDlgModelService* service)
        : AbstractModel(service)
    {
        setupModel(this);
    }


    void OptionsDlgModel::initSyncData()
    {
        if (ui.refreshRates.empty())
        {
            ui.refreshRates = pm.refreshRates = m_service->getRefereshRates();
        }

        ui.ok_enabled = pm.ok_enabled = false;
        ui.index = pm.index = 1;
    }


    void OptionsDlgModel::onRefreshRatesIndexChanged(int index)
    {
        onViewChanged();
    }


    void OptionsDlgModel::calculateStatus(SyncData& data)
    {
        data.ok_enabled = (ui.index != pm.index);
    }


    void OptionsDlgModel::notifyChangesToView(const SyncData& o, const SyncData& n)
    {
        NOTIFY(ok_enabled, &IOptionsDlg::onModelEnableOkButton);
    }


    void OptionsDlgModel::onAccept()
    {
        AbstractModel::onAccept();
        m_service->onSetRefreshRate(boost::lexical_cast<size_t>(pm.refreshRates[pm.index]));
    }
}
