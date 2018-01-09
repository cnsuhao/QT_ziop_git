#pragma once
#include "Promise.h"
#include "ControlStationConstants.h"

using namespace TA_Base_App;

class UserExceptionHandler
{
public:

    static UserExceptionHandler& getInstance();
    void handleException(boost::exception_ptr exp, const std::string& actionName);
    void handlePermission(TA_ControlStation::EPermission permission, const std::string& actionName);

private:

    UserExceptionHandler();
    friend class ACE_Singleton<UserExceptionHandler, ACE_Null_Mutex>;
};
