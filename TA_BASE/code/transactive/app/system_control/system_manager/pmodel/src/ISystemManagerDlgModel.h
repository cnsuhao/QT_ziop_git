#pragma once
#include "IModel.h"
#include <string>
#include <vector>
#include <map>
#include <set>

typedef std::vector<std::string> Process;
typedef std::vector<Process> ProcessList;
typedef std::map<std::pair<size_t, size_t>, std::string> ProcessUpdates;

namespace TA_Base_App
{
    struct ISystemManagerDlgModel
    {
        // Service ==> Model
        virtual void onServiceProcessList(const ProcessList& processes) = 0;
        virtual void onServiceUpdateProcessList(const ProcessUpdates& updates) = 0;
        virtual void onServiceClearProcessList() = 0;
        virtual void onServiceLogin(const std::string& hostname) = 0;
        virtual void onServiceLogout() = 0;
        virtual void onServiceSystemControllerError(bool error) = 0;
        virtual void onServiceShutdownSystemControllerStarted() = 0;
        virtual void onServiceShutdownSystemControllerFinished() = 0;
        // Service <==> Model
        virtual const std::set<size_t>& getSelection() = 0;
    };
}
