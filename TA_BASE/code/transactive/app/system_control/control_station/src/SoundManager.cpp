#include "StdAfx.h"
#include "SoundManager.h"
#include "Signals.h"
#include "ControlStationConstants.h"
#include "bus/sound/sound_client/src/SoundManagerProxy.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/exceptions/src/SoundPlayerException.h"

using namespace TA_Base_App;

SoundManager::SoundManager()
{
    setupSignalConnections();
}

void SoundManager::setupSignalConnections()
{
    Signals::systemControllerStateChanged.connect(boost::bind(&SoundManager::onSystemControllerStateChange, this, _1, _2));
}

void SoundManager::asyncInitialize()
{
    try
    {
        m_soundInterface.reset(new TA_Base_Bus::SoundManagerProxy);
    }
    catch (TA_Base_Core::SoundPlayerException& ex)
    {
        std::string error = "Unable to initialise SoundManager: ";
        error += ex.what();
        LOG_EXCEPTION("SoundPlayerException", error.c_str());
    }
}

void SoundManager::onSystemControllerStateChange(bool isGroupOnline, const std::string& groupName)
{
    if (m_soundInterface)
    {
        try
        {
            m_soundInterface->setPriority(TA_ControlStation::GROUP_STATUS_SOUND_CATEGORY,
                                          isGroupOnline ? TA_ControlStation::GROUP_ONLINE_SOUND_CATEGORY : TA_ControlStation::GROUP_OFFLINE_SOUND_CATEGORY);
        }
        // If there are any exceptions, just log and continue
        catch (TA_Base_Core::SoundPlayerException& ex)
        {
            LOG_EXCEPTION("SoundPlayerException", str(boost::format("Caught while attempting to play sound for group online, with error: %s") % ex.what()));
        }
        catch (...)
        {
            LOG_EXCEPTION("Unknown Exception", "Caught an exception while attempting to play sound for group online.");
        }
    }
}
