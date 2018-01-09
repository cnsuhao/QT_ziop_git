#include "RunParamUser.h"
#include "RunParams.h"

namespace TA_Base_Core
{
    RunParamUser::~RunParamUser()
    {
        RunParams::getInstance().deregisterRunParamUser(this);
    }
}
