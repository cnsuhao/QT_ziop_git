#include "ShutdownSystemControllerDlgModelService.h"
#include "SystemManagerDlgModelService.h"
#include "Factory.h"
#include "SystemControllerService.h"
#include "SendMessage.h"


namespace TA_Base_App
{
    std::string ShutdownSystemControllerDlgModelService::getHostname()
    {
        return Factory::instance().getSystemManagerDlgModelService()->getHostname();
    }


    void ShutdownSystemControllerDlgModelService::onShutdownSystemController(const std::string& hostname)
    {
        Factory::instance().getSystemControllerService()->shutdownSystemController(hostname);
        sendMessage(&IMessage::onMessageShutdownBegin);
    }
}
