#pragma once
#include "IService.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct IDatabaseService;
    struct TransActiveLoginDlgModel;

    struct TransActiveLoginDlgModelService : IService<TransActiveLoginDlgModel>
    {
    public:

        TransActiveLoginDlgModelService();
        std::vector<std::string> getOperators();
        std::vector<std::string> getProfiles(const std::string& operatorName);
        std::vector<std::string> getLocations(const std::string& profileName);
        bool requiresLocationToBeSelected(const std::string& profileName);
        const std::string& getCurrentLocation();
        size_t getCurrentConsole();
        void onLogin(const std::string& user, const std::string& profile, const std::string& location, const std::string& password);

    private:

        IDatabaseService* m_dbservice;
    };
}
