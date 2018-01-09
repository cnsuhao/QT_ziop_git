#include "TransActiveLoginDlgModelService.h"
#include "Factory.h"
#include "IDatabaseService.h"


namespace TA_Base_App
{
    TransActiveLoginDlgModelService::TransActiveLoginDlgModelService()
    {
        m_dbservice = TA_Base_App::Factory::instance().getDatabaseService();
    }


    std::vector<std::string> TransActiveLoginDlgModelService::getOperators()
    {
        return m_dbservice->getAllOperatorNames();
    }


    std::vector<std::string> TransActiveLoginDlgModelService::getProfiles(const std::string& operatorName)
    {
        return m_dbservice->getProfileNamesForOperator(operatorName);
    }


    std::vector<std::string> TransActiveLoginDlgModelService::getLocations(const std::string& profileName)
    {
        return m_dbservice->getLocationDescriptionsForProfile(profileName);
    }


    void TransActiveLoginDlgModelService::onLogin(const std::string& user, const std::string& profile, const std::string& location, const std::string& password)
    {
        //TODO
    }


    bool TransActiveLoginDlgModelService::requiresLocationToBeSelected(const std::string& profileName)
    {
        return m_dbservice->requiresLocationToBeSelected(profileName);
    }


    const std::string& TransActiveLoginDlgModelService::getCurrentLocation()
    {
        return m_dbservice->getCurrentLocationDescription();
    }


    size_t TransActiveLoginDlgModelService::getCurrentConsole()
    {
        return m_dbservice->getCurrentConsoleKey();
    }
}
