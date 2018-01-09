#include "StdAfx.h"
#include "Application.h"
#include "RunningApplication.h"
#include "RightsManager.h"
#include "RunningApplicationManager.h"
#include "ExternalRunningApplication.h"
#include "Application.h"
#include "resourceQt.h"
#include "SessionManager.h"
#include "ApplicationSignal.h"
#include "bus/resources/resource.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "core/data_access_interface/src/IGui.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/src/ResourceAccessFactory.h"
#include "core/data_access_interface/src/IResource.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include "core/utilities/src/DebugUtil.h"

using namespace TA_Base_Core;

const int Application::APPLICATION_NAME_LENGTH = 9;

Application::Application(TA_Base_Core::IGui* gui)
    : m_gui(gui)
{
    m_resourceKey.set_wait_callback(boost::bind(&Application::getResourceKeyCallback, this, _1));
    m_type = m_gui->getKey();
    setupSignalConnections();
}

Application::~Application()
{
    disconnectSignals();
}

Application* Application::create(TA_Base_Core::IGui* gui)
{
    return new Application(gui);
}

void Application::setupSignalConnections()
{
    m_connections.push_back(ApplicationSignal::launchApplicationByAppType[m_type].connect(boost::bind(&Application::launchApplicationByType, this, _1, _2, _3, _4, _5, _6)));
    m_connections.push_back(ApplicationSignal::registerManagedProcessByAppType[m_type].connect(boost::bind(&Application::registerManagedProcessByType, this, _1, _2, _3)));
}

void Application::disconnectSignals()
{
    for (auto& c : m_connections)
    {
        c.disconnect();
    }
}

bool Application::isAccessible()
{
    return RightsManager::getInstance().isActionPermitted(TA_Base_Bus::aca_RUN_APPLICATION, m_resourceKey) == TA_Base_App::TA_ControlStation::PERMITTED;
}

void Application::launchApplicationByType(const std::string& additionalCommands,
                                          unsigned long posFlag,
                                          unsigned long alignFlag,
                                          const RECT& objectDim,
                                          const RECT& boundaryDim,
                                          bool isVisible)
{
    size_t count = RunningApplicationManager::getInstance().getRunningApplicationCount(m_type);

    if (count && m_gui->areInstancesLimited())
    {
        return;
    }

    RunningApplicationManager::getInstance().createRunningApplication(this, additionalCommands, posFlag, alignFlag, objectDim, boundaryDim, isVisible);
}

void Application::registerManagedProcessByType(TA_Base_Core::ProcessId processId,
                                               TA_Base_Core::IManagedProcessCorbaDef_var managedProcess,
                                               const std::string& entity)
{
    if (!RunningApplicationManager::getInstance().isRunning(processId))
    {
        size_t count = RunningApplicationManager::getInstance().getRunningApplicationCount(m_type);

        if (count && m_gui->areInstancesLimited())
        {
            return;
        }

        RunningApplicationManager::getInstance().createRunningApplication(this, managedProcess, processId, entity);
    }

    ApplicationSignal::registerManagedProcessByPid[processId](managedProcess, entity);
}

void Application::getResourceKeyCallback(boost::promise<unsigned int>& promise)
{
    unsigned int resourceKey = -1;

    try
    {
        std::string executable = m_gui->getExecutable();
        boost::smatch m;
        boost::regex e("(--entity-name=|--EntityName=)([^ ]+)");

        if (boost::regex_search(executable, m, e))
        {
            std::string entityName = m.str(1);
            IEntityDataPtr entity(EntityAccessFactory::getInstance().getEntity(executable));

            if (entity)
            {
                IResourcePtr resource(TA_Base_Core::ResourceAccessFactory::getInstance().getResourceFromEntity(entity->getKey()));

                if (resource)
                {
                    resourceKey = resource->getKey();
                    LOG_DEBUG("Resource of %s is %d", entityName.c_str(), resourceKey);
                }
            }
        }
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown exception", "Some unknown exception has occurred when attempting to get the entity key or resource key of the application");
    }

    promise.set_value(resourceKey);
}
