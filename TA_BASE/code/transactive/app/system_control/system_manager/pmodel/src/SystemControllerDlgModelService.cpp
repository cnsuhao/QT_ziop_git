#include "SystemControllerDlgModelService.h"
#include "SendMessage.h"
#include "SystemControllerService.h"
#include "IDatabaseService.h"
#include "Factory.h"
#include <boost/lexical_cast.hpp>


namespace TA_Base_App
{
    std::vector<std::string>& SystemControllerDlgModelService::getHostnames()
    {
        return Factory::instance().getDatabaseService()->getHostnames();
    }


    std::string& SystemControllerDlgModelService::getPort()
    {
        static std::string port = boost::lexical_cast<std::string>(TA_Base_Core::DEFAULT_MGR_PORT);
        return port;
    }


    void SystemControllerDlgModelService::onLogin(const std::string& hostname, const std::string& port)
    {
        Factory::instance().getSystemControllerService()->setCurrentSystemController(hostname, boost::lexical_cast<unsigned long>(port));
        sendMessage(&IMessage::onMessageLogin, hostname);
    }
}
