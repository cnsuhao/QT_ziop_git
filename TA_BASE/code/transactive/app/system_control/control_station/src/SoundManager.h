#pragma once
#include "Singleton.h"

namespace TA_Base_Bus
{
    class SoundManagerProxy;
    typedef boost::shared_ptr<SoundManagerProxy> SoundManagerProxyPtr;
}

class SoundManager : public Singleton<SoundManager>
{
protected:

    SoundManager();
    virtual void asyncInitialize() override;
    void setupSignalConnections();
    void onSystemControllerStateChange(bool isGroupOnline, const std::string& groupName);

private:

    TA_Base_Bus::SoundManagerProxyPtr m_soundInterface;
};
