#include "StopProcessDlgModelService.h"
#include "SystemControllerService.h"
#include "StopProcessDlgModel.h"
#include "Factory.h"


namespace TA_Base_App
{
    StopProcessDlgModelService::StopProcessDlgModelService()
    {
        m_selection = m_util.selectStopAbleIndexes();
        m_util.set(m_selection);
    }


    std::vector<std::string> StopProcessDlgModelService::getProcesses()
    {
        return m_util.selectStopAble(true);
    }


    std::vector<std::string> StopProcessDlgModelService::getNotStopAbleProcesses()
    {
        return m_util.selectNotStopAble(true);
    }


    void StopProcessDlgModelService::onStopProcess()
    {
        Factory::instance().getSystemControllerService()->stopProcess(m_util.selectStopAble());
    }


    void StopProcessDlgModelService::onMessageProcessDataChanged(const TA_Base_Core::ProcessDataSeq& data)
    {
        m_model->onServiceChanged();
    }
}
