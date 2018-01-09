#pragma once
#include <string>

const int ERR_AUTH_NO_ERROR              = 0;
const int ERR_AUTH_SOCKET_ERROR          = 1;
const int ERR_AUTH_SERVICE_ERROR         = 2;
const int ERR_AUTH_ACCESS_DENIED         = 3;
const int ERR_GET_ADMIN_ERROR            = 4;
const int ERR_SESSION_RUNNING            = 5;
const int ERR_AUTH_NO_PERMISSION         = 6;
const int ERR_AUTH_INVALID_KEYS          = 7;
const int ERR_NO_SYSTEM_CONTROLLER_ERROR = 8;
const int ERR_RETRIEVING_PROCESS_DATA    = 9;

#define DOMAIN_DATA_CHANGE_NEW_PROCESS_DATA "new_process_data"

namespace TA_Base_App
{
    struct AppServiceConsts
    {
        static const std::string RUNNING_CONTROL;
        static const std::string RUNNING_MONITOR;
        static const std::string GOING_TO_CONTROL;
        static const std::string GOING_TO_MONITOR;
        static const std::string STARTUP;
        static const std::string STOPPED;
        static const std::string NOT_RUNNING;
        static const std::string TERMINATING;
        static const std::string NOT_STARTED;

        static const std::string RUNNING; // For non managed processes

        static const std::string NOT_APPLICABLE;
        static const std::string CONTROL;
        static const std::string MONITOR;
    };
}
