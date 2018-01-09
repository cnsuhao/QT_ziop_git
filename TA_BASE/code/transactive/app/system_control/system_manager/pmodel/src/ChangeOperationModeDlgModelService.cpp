#include "ChangeOperationModeDlgModelService.h"
#include "SystemControllerService.h"
#include "Factory.h"
#include "ChangeOperationModeDlgModel.h"


namespace TA_Base_App
{
    ChangeOperationModeDlgModelService::ChangeOperationModeDlgModelService()
    {
        m_selection = m_util.selectCanChangeModeIndexes();
        m_util.set(m_selection);
    }


    std::vector<std::string> ChangeOperationModeDlgModelService::getProcesses()
    {
        return m_util.selectCanChangeMode(true);
    }


    bool ChangeOperationModeDlgModelService::isAllCannotChangeMode()
    {
        return m_util.isAllCannotChangeMode();
    }


    bool ChangeOperationModeDlgModelService::isAllCannotChangeToControl()
    {
        return m_util.isAllCannotChangeToControl();
    }


    bool ChangeOperationModeDlgModelService::isAllCannotChangeToMonitor()
    {
        return m_util.isAllCannotChangeToMonitor();
    }


    bool ChangeOperationModeDlgModelService::isCanChangeToBothMode()
    {
        return m_util.isCanChangeToBothMode();
    }


    void ChangeOperationModeDlgModelService::onChangeOperationMode(bool isControl)
    {
        std::vector<std::string> processes = isControl ? m_util.selectCanChangeToControl() : m_util.selectCanChangeToMonitor();
        Factory::instance().getSystemControllerService()->changeProcessOperationMode(processes, isControl ? TA_Base_Core::Control : TA_Base_Core::Monitor);
    }


    std::vector<std::string> ChangeOperationModeDlgModelService::getCannotChangeModeProcesses()
    {
        return m_util.selectCannotChangeMode(true);
    }


    std::vector<std::string> ChangeOperationModeDlgModelService::getCannotChangeToControlProcesses()
    {
        return m_util.selectCannotChangeToControl(true);
    }


    std::vector<std::string> ChangeOperationModeDlgModelService::getCannotChangeToMonitorProcesses()
    {
        return m_util.selectCannotChangeToMonitor(true);
    }


    void ChangeOperationModeDlgModelService::onMessageProcessDataChanged(const TA_Base_Core::ProcessDataSeq& data)
    {
        m_model->onServiceChanged();
    }
}
