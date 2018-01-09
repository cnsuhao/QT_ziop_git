#include "ProcessParamsDlgModel.h"
#include "IProcessParamsDlg.h"
#include "ProcessParamsDlgModelService.h"
#include "ForeachCall.h"


namespace TA_Base_App
{
    std::vector<std::string> ProcessParamsSynData::debug_levels;


    ProcessParamsDlgModel::ProcessParamsDlgModel(ProcessParamsDlgModelService* service)
        : AbstractModel(service)
    {
        setupModel(this);
    }


    void ProcessParamsDlgModel::initSyncData()
    {
        if (ui.debug_levels.empty())
        {
            ui.debug_levels = pm.debug_levels = m_service->getDebugLevels();
        }
    }


    void ProcessParamsDlgModel::onDebugLevelsIndexChanged(int index)
    {
        onViewChanged();
    }


    void ProcessParamsDlgModel::calculateStatusOnViewChanged()
    {
        ui.ok_enabled = m_mixed_debug_levels || (ui.index != pm.index);
    }


    void ProcessParamsDlgModel::calculateStatusOnModelChanged()
    {
        m_mixed_debug_levels = (false == m_service->isAllSameDebugLevel());
        pm.index = m_service->getDebugLevelsIndex();
        pm.ok_enabled = m_mixed_debug_levels || (ui.index != pm.index);
    }


    void ProcessParamsDlgModel::notifyChangesToView(const SyncData& o, const SyncData& n)
    {
        NOTIFY(ok_enabled, &IProcessParamsDlg::onModelEnableOkButton);
        NOTIFY(index, &IProcessParamsDlg::onModelSetIndex);
    }


    void ProcessParamsDlgModel::beforeAttachView()
    {
        m_mixed_debug_levels = (false == m_service->isAllSameDebugLevel());
        ui.index = pm.index = m_service->getDebugLevelsIndex();
        ui.ok_enabled = pm.ok_enabled = m_mixed_debug_levels;
    }


    void ProcessParamsDlgModel::onAccept()
    {
        AbstractModel::onAccept();
        m_service->onChangeDebugLevel(pm.debug_levels[pm.index]);
    }
}
