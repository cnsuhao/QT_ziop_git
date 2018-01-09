#include "StdAfx.h"
#include "TADllHostApplication.h"
#include "SessionSignal.h"
#include "ControlStationSignal.h"
#include "DataCache.h"
#include "RunningApplicationManager.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/utilities/src/RunParams.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"

TADllHostApplication::TADllHostApplication(TA_Base_Core::IGui* gui)
    : Application(gui)
{
    setupSignalConnections();
}

TADllHostApplication::~TADllHostApplication()
{
}

void TADllHostApplication::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&TADllHostApplication::onLogin, this));
}

TADllHostApplication* TADllHostApplication::create(TA_Base_Core::IGui* gui)
{
    return new TADllHostApplication(gui);
}

void TADllHostApplication::onLogin()
{
    if (!m_dllHostEntity)
    {
        m_dllHostEntity = DataCache::getInstance().getDllHost();
    }

    static RECT defaultRect = { -1, -1, -1, -1 };
    std::string commandline = str(boost::format("--%s=%s") % RPARAM_CORBA_NAMING_PORT % m_dllHostEntity->getAddress());

    RunningApplicationManager::getInstance().createRunningApplication(this,
                                                                      commandline,
                                                                      TA_Base_Bus::TA_GenericGui::POS_BOUNDED,
                                                                      TA_Base_Bus::TA_GenericGui::ALIGN_FIT,
                                                                      defaultRect,
                                                                      defaultRect,
                                                                      false);
}
