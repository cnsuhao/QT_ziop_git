#include "StdAfx.h"
#include "ApplicationManager.h"
#include "Application.h"
#include "SessionSignal.h"
#include "ControlStationSignal.h"
#include "ApplicationSignal.h"
#include "RunningApplicationManager.h"
#include "BannerApplication.h"
#include "SystemManagerApplication.h"
#include "TADllHostApplication.h"
#include "DataCache.h"
#include "NamedRunningApplicationTerminator.h"
#include "bus/application_types/src/apptypes.h"
#include "core/data_access_interface/src/IGui.h"
#include "core/exceptions/src/ApplicationException.h"
#include "core/data_access_interface/src/GuiAccessFactory.h"
#include "core/configuration_updates/src/OnlineUpdateListener.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/configuration_updates/src/ConfigUpdateDetails.h"

using namespace TA_Base_Core;
using namespace boost::assign;

ApplicationManager::ApplicationManager()
{
    setupSignalConnections();
    setupApplicationCreators();
}

void ApplicationManager::setupSignalConnections()
{
    ApplicationSignal::launchApplication.connect(boost::bind(&ApplicationManager::launchApplication, this, _1, _2, _3, _4, _5, _6, _7));
    ApplicationSignal::registerManagedProcess.connect(boost::bind(&ApplicationManager::registerManagedProcess, this, _1, _2, _3, _4));
    //ControlStationSignal::exit.connect();
}

void ApplicationManager::asyncInitialize()
{
    loadApplications();
    loadTerminators();
    OnlineUpdateListener::getInstance().registerAllInterests(TA_Base_Core::APPLICATION, *this);
}

void ApplicationManager::loadApplications()
{
    for (IGui* gui : GuiAccessFactory::getInstance().getAllGuis())
    {
        unsigned long key = gui->getKey();
        m_applications.insert(std::make_pair(key, getApplicationCreator(key)(gui)));
    }
}

void ApplicationManager::loadTerminators()
{
    TA_Base_Core::ControlStationPtr controlStation = DataCache::getInstance().getControlStationEntity();

    NamedRunningApplicationTerminatorPtr terminateOnLogout(new NamedRunningApplicationTerminator(controlStation->getApplicationsToTerminateOnLogout()));
    terminateOnLogout->addSignal(SessionSignal::logouted);
    terminateOnLogout->addApplicationName("Gwx32.exe");
    m_terminators += terminateOnLogout;

    NamedRunningApplicationTerminatorPtr terminateOnControlStationExit(new NamedRunningApplicationTerminator(controlStation->getApplicationsToTerminate()));
    terminateOnControlStationExit->addSignal(ControlStationSignal::exit);
    m_terminators += terminateOnControlStationExit;
}

void ApplicationManager::setupApplicationCreators()
{
    m_creators[BANNER_GUI_APPTYPE] = &BannerApplication::create;
    m_creators[DLL_HOST_GUI_APPTYPE] = &TADllHostApplication::create;
    m_creators[ADMIN_GUI_APPTYPE] = &SystemManagerApplication::create;
}

void ApplicationManager::launchApplication(unsigned long appType,
                                           const std::string& additionalCommands,
                                           unsigned long posFlag,
                                           unsigned long alignFlag,
                                           const RECT& objectDim,
                                           const RECT& boundaryDim,
                                           bool isVisible)
{
    waitAsyncInitialize();
    checkAppType(appType);
    ApplicationSignal::launchApplicationByAppType[appType](additionalCommands, posFlag, alignFlag, objectDim, boundaryDim, isVisible);
}

bool ApplicationManager::isValidAppType(unsigned long appType)
{
    return m_applications.find(appType) != m_applications.end();
}

void ApplicationManager::registerManagedProcess(TA_Base_Core::IManagedProcessCorbaDef_var managedProcess,
                                                TA_Base_Core::ProcessId processId,
                                                CORBA::ULong applicationType,
                                                const std::string& entity)
{
    waitAsyncInitialize();
    checkAppType(applicationType);
    ApplicationSignal::registerManagedProcessByAppType[applicationType](processId, managedProcess, entity);
}

void ApplicationManager::checkAppType(unsigned long appType)
{
    if (!isValidAppType(appType))
    {
        TA_THROW(ApplicationException(str(boost::format("Application type not found - %lu") % appType)));
    }
}

void ApplicationManager::processUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent)
{
    FUNCTION_ENTRY("processUpdate");

    if (updateEvent.getType() != TA_Base_Core::APPLICATION)
    {
        LOG_WARNING("Type of configuration update does not match the update we are interested in. Ignoring");
        FUNCTION_EXIT;
        return;
    }

    unsigned long key = updateEvent.getKey();
    ApplicationMap::iterator appIter = m_applications.find(key);

    if ((updateEvent.getModifications() != TA_Base_Core::Create) && (appIter == m_applications.end()))
    {
        LOG_WARNING("Key of configuration update does not match the update we are interested in. Ignoring");
        FUNCTION_EXIT;
        return;
    }

    switch (updateEvent.getModifications())
    {
    case TA_Base_Core::Create:
    {
        LOG_INFO("Received a 'Create' update");
        TA_Base_Core::IGui* gui = TA_Base_Core::GuiAccessFactory::getInstance().getGui(key);
        m_applications.insert(ApplicationMap::value_type(key, new Application(gui)));
        break;
    }

    case TA_Base_Core::Delete:
    {
        LOG_INFO("Received a 'Delete' update");
        m_applications.erase(appIter);
        break;
    }

    case TA_Base_Core::Update:
    {
        LOG_INFO("Received a 'Update' update");

        if (updateEvent.getChangedParams().empty())
        {
            LOG_WARNING("Received a configuration update with an empty list of changes. Ignoring");
        }
        else
        {
            // mark the Gui as dirty so that it'll fetch from database next time it's called
            appIter->second->getGui().invalidate();
        }

        break;
    }

    default:
    {
        LOG_INFO("Received unknown update");
        break;
    }
    }

    FUNCTION_EXIT;
}

std::vector<IProcessManager::ApplicationProperties> ApplicationManager::getApplications()
{
    FUNCTION_ENTRY("getApplications");

    std::vector<IProcessManager::ApplicationProperties> applist;

    for (ApplicationMap::value_type& v : m_applications)
    {
        Application* app = v.second;

        try
        {
            if (app->getGui().isIconDisplayed())
            {
                IProcessManager::ApplicationProperties prop;
                prop.key = app->getGui().getKey();
                prop.name = app->getGui().getFullName();
                prop.isAccessible = app->isAccessible();
                applist.push_back(prop);
            }
        }
        catch (...)
        {
            LOG_EXCEPTION("Unknown", "Error occurred while retrieving the list of applications. Ignore this application since we cannot retrieve it's details");
        }
    }

    return applist;
}

ApplicationCreator ApplicationManager::getApplicationCreator(unsigned long appType)
{
    ApplicationCreatorMap::iterator it = m_creators.find(appType);

    if (it != m_creators.end())
    {
        return it->second;
    }

    struct DefaultApplicationCreator
    {
        Application* operator()(TA_Base_Core::IGui* gui)
        {
            return Application::create(gui);
        }
    };

    return DefaultApplicationCreator();
}
