#include "StartProcessDlgModelService.h"
#include "SystemControllerService.h"
#include "StartProcessDlgModel.h"
#include "Factory.h"


namespace TA_Base_App
{
    StartProcessDlgModelService::StartProcessDlgModelService()
    {
        m_selection = m_util.selectStartAbleIndexes();
        m_util.set(m_selection);
    }


    std::vector<std::string> StartProcessDlgModelService::getProcesses()
    {
        return m_util.selectStartAble(true);
    }


    std::vector<std::string> StartProcessDlgModelService::getNotStartAbleProcesses()
    {
        return m_util.selectNotStartAble(true);
    }


    void StartProcessDlgModelService::onStartProcess()
    {
        Factory::instance().getSystemControllerService()->startProcess(m_util.selectStartAble());
    }


    void StartProcessDlgModelService::onMessageProcessDataChanged(const TA_Base_Core::ProcessDataSeq& data)
    {
        m_model->onServiceChanged();
    }
}
