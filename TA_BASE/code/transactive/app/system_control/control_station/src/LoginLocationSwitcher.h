#pragma once
#include "Singleton.h"
#include "Promise.h"

namespace TA_Base_Core
{
    class IOperator; class IProfile; class ILocation;
}

class LoginLocationSwitcher : public Singleton<LoginLocationSwitcher>
{
protected:

    LoginLocationSwitcher();
    void setupSignalConnections();
    void handleOperatorRequestLoginInfoSignal(TA_Base_Core::ILocation* location);
};
