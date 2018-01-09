#include "ProcessParamsDlgModelService.h"
#include "SystemControllerService.h"
#include "ProcessParamsDlgModel.h"
#include "Factory.h"


namespace TA_Base_App
{
    const std::vector<std::string>& ProcessParamsDlgModelService::getDebugLevels()
    {
        return m_util.getDebugLevels();
    }


    size_t ProcessParamsDlgModelService::getDebugLevelsIndex()
    {
        std::string debugLevel = m_util.getDebugLevel(*m_selection.begin());
        return m_util.getDebugLevelIndex(debugLevel);
    }


    bool ProcessParamsDlgModelService::isAllSameDebugLevel()
    {
        return m_util.isAllSameDebugLevel();
    }


    void ProcessParamsDlgModelService::onChangeDebugLevel(const std::string& debugLevel)
    {
        TA_Base_Core::RunParam param;
        param.name = static_cast<const char*>("DebugLevel");
        param.value = debugLevel.c_str();
        Factory::instance().getSystemControllerService()->setProcessRunParam(m_util.selectNotDebugLevel(debugLevel), param);
    }


    void ProcessParamsDlgModelService::onMessageProcessDataChanged(const TA_Base_Core::ProcessDataSeq& data)
    {
        m_model->onServiceChanged();
    }
}
