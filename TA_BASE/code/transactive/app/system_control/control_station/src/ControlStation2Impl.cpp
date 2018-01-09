#include "StdAfx.h"
#include "ControlStation2Impl.h"
#include "ComSignal.h"
#include "LogoutTask.h"
#include "SessionManager.h"
#include "SessionDetails.h"
#include "DataCache.h"
#include "ControlStationConstants.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/src/CommsMessageSender.h"
#include "core/process_management/idl/src/IManagedApplicationCorbaDef.h"
#include "core/message/types/ControlStationComms_MessageTypes.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/src/ResourceAccessFactory.h"
#include "core/data_access_interface/src/IResource.h"
#include "RightsManager.h"
#include "bus/response_plans/plan_agent/src/PlanAgentLibrary.h"
#include "PlanAgentDataCorbaDef.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "GraphWorxManager.h"
#include "ScreenPositioning.h"
#include "ControlStationRootDialog.h"
#include "ServerMonitor.h"
#include "ApplicationSignal.h"
#include "ApplicationManager.h"
#include "RunningApplicationManager.h"

using namespace TA_Base_Core;
using namespace TA_Base_Bus;
using namespace TA_Base_App;

ControlStation2Impl::ControlStation2Impl()
{
    setupSignalConnections();
}

void ControlStation2Impl::setupSignalConnections()
{
    ComSignal::logout.connect(boost::bind(&ControlStation2Impl::logout, this));
    ComSignal::getOperator.connect(boost::bind(&ControlStation2Impl::getOperator, this, _1));
    ComSignal::getSite.connect(boost::bind(&ControlStation2Impl::getSite, this, _1));
    ComSignal::getProfile.connect(boost::bind(&ControlStation2Impl::getProfile, this, _1));
    ComSignal::launchApplicationEx.connect(boost::bind(&ControlStation2Impl::launchApplicationEx, this, _1, _2, _3, _4, _5, _6));
    ComSignal::getAllApplications.connect(boost::bind(&ControlStation2Impl::getAllApplications, this, _1));
    ComSignal::getRunningApplications.connect(boost::bind(&ControlStation2Impl::getRunningApplications, this, _1));
    ComSignal::getLocation.connect(boost::bind(&ControlStation2Impl::getLocation, this, _1));
    ComSignal::setSelectedAlarms.connect(boost::bind(&ControlStation2Impl::setSelectedAlarms, this, _1, _2));
    ComSignal::getProfileDisplay.connect(boost::bind(&ControlStation2Impl::getProfileDisplay, this, _1, _2));
    ComSignal::getProfileDisplayAtLocation.connect(boost::bind(&ControlStation2Impl::getProfileDisplayAtLocation, this, _1, _2, _3));
    ComSignal::changeProfile.connect(boost::bind(&ControlStation2Impl::changeProfile, this));
    ComSignal::changeDisplayMode.connect(boost::bind(&ControlStation2Impl::changeDisplayMode, this));
    ComSignal::changePassword.connect(boost::bind(&ControlStation2Impl::changePassword, this));
    ComSignal::isServerContactable.connect(boost::bind(&ControlStation2Impl::isServerContactable, this, _1));
    ComSignal::displayTaskManager.connect(boost::bind(&ControlStation2Impl::displayTaskManager, this));
    ComSignal::changeOperatorOverride.connect(boost::bind(&ControlStation2Impl::changeOperatorOverride, this));
    ComSignal::isOperatorOverridden.connect(boost::bind(&ControlStation2Impl::isOperatorOverridden, this, _1));
    ComSignal::launchDisplayEx.connect(boost::bind(&ControlStation2Impl::launchDisplayEx, this, _1, _2, _3));
    ComSignal::setDisplayDimension.connect(boost::bind(&ControlStation2Impl::setDisplayDimension, this, _1));
    ComSignal::getRect.connect(boost::bind(&ControlStation2Impl::getRect, this, _1, _2, _3, _4));
    ComSignal::getCurrentDisplayName.connect(boost::bind(&ControlStation2Impl::getCurrentDisplayName, this, _1));
    ComSignal::launchApplication.connect(boost::bind(&ControlStation2Impl::launchApplication, this, _1, _2, _3, _4, _5, _6));
    ComSignal::launchDisplay.connect(boost::bind(&ControlStation2Impl::launchDisplay, this, _1, _2));
    ComSignal::getSessionId.connect(boost::bind(&ControlStation2Impl::getSessionId, this, _1));
    ComSignal::loadComplete.connect(boost::bind(&ControlStation2Impl::loadComplete, this, _1));
    ComSignal::launchHelp.connect(boost::bind(&ControlStation2Impl::launchHelp, this));
    ComSignal::launchPlan.connect(boost::bind(&ControlStation2Impl::launchPlan, this, _1));
    ComSignal::launchPlanByName.connect(boost::bind(&ControlStation2Impl::launchPlanByName, this, _1));
    ComSignal::setRunParam.connect(boost::bind(&ControlStation2Impl::setRunParam, this, _1, _2));
    ComSignal::isActionPermittedOnEntity.connect(boost::bind(&ControlStation2Impl::isActionPermittedOnEntity, this, _1, _2, _3));
    ComSignal::entityCommand.connect(boost::bind(&ControlStation2Impl::entityCommand, this, _1, _2, _3, _4));
    ComSignal::isLoginAllowed.connect(boost::bind(&ControlStation2Impl::isLoginAllowed, this, _1, _2, _3));
    ComSignal::getDisplayMode.connect(boost::bind(&ControlStation2Impl::getDisplayMode, this, _1));
}

void ControlStation2Impl::entityCommand(EntityMessageType entityType, const std::string& location, const std::string& sourceSchematicID, const std::string& message)
{
    std::string localLocation = SessionDetails::getInstance().getLocationName();

    if (localLocation == location)
    {
        CORBA::Any data;
        IManagedApplicationCorbaDef::GXToTAMessage event;

        static std::map<EntityMessageType, IManagedApplicationCorbaDef::EntityMessageType> theMap =
        {
            { All, IManagedApplicationCorbaDef::All },
            { RadioAgent, IManagedApplicationCorbaDef::RadioAgent },
            { EntityMessageType::ControlStation, IManagedApplicationCorbaDef::ControlStation }
        };

        event.entityType = theMap[entityType];
        event.sourceSchematicID = sourceSchematicID.c_str();
        event.message = message.c_str();
        event.sessionId = SessionDetails::getInstance().getSessionId().c_str();
        data <<= event;
        boost::shared_ptr<CommsMessageSender> commsMessageSender(MessagePublicationManager::getInstance().getCommsMessageSender(ControlStationComms::GXToTAMessageNotification));

        commsMessageSender->sendCommsMessage(ControlStationComms::GXToTAMessageNotification,
                                             DataCache::getInstance().getControlStationEntity()->getKey(),
                                             data,
                                             0,
                                             SessionDetails::getInstance().getLocationKey(),
                                             NULL);
    }
}

void ControlStation2Impl::isLoginAllowed(const std::string& bstrOperator, const std::string& bstrPassword, BoolPromisePtr bLogin)
{
    std::string theOperatorName = SessionDetails::getInstance().getOperatorName();

    bLogin->set_value(theOperatorName.size() &&
                      (theOperatorName == bstrOperator) &&
                      (SessionDetails::getInstance().getSessionInfo().password == bstrPassword));
}

void ControlStation2Impl::isActionPermittedOnEntity(const std::string& entityName, long actionKey, boost::shared_ptr<Promise<ResponseEnum>> response)
{
    TA_Base_App::TA_ControlStation::EPermission permission = TA_ControlStation::GENERAL_ERROR;

    try
    {
        boost::shared_ptr<TA_Base_Core::IEntityData> entity(EntityAccessFactory::getInstance().getEntity(entityName));
        boost::shared_ptr<TA_Base_Core::IResource> resource(ResourceAccessFactory::getInstance().getResourceFromEntity(entity->getKey()));
        permission = RightsManager::getInstance().isActionPermitted(actionKey, resource->getKey());
    }
    catch (...)
    {
    }

    static std::map<TA_ControlStation::EPermission, ResponseEnum> theMap =
    {
        { TA_ControlStation::DENIED, Deny },
        { TA_ControlStation::PERMITTED, Permit },
        { TA_ControlStation::UNKNOWN, Indeterminate },
        { TA_ControlStation::GENERAL_ERROR, TAError }
    };

    response->set_value(theMap[permission]);
}

void ControlStation2Impl::launchPlan(long planId)
{
    TA_Base_Core::ActivePlanId planDetails;
    planDetails.plan = planId;
    TA_Base_Bus::PlanAgentLibrary planLibrary(SessionDetails::getInstance().getLocationKey());
    planLibrary.runPlan(SessionDetails::getInstance().getSessionId().c_str(), planDetails);
}

void ControlStation2Impl::launchPlanByName(const std::string& planPath)
{
    TA_Base_Bus::PlanAgentLibrary planLibrary(SessionDetails::getInstance().getLocationKey());
    planLibrary.runPlanByName(SessionDetails::getInstance().getSessionId().c_str(), planPath.c_str());
}

void ControlStation2Impl::launchHelp()
{
    //TODO:
    //TA_Base_Bus::HelpLauncher::getInstance().displayHelp(SessionManager::getInstance().getGraphWorxHelpfile(), true);
}

void ControlStation2Impl::loadComplete(long leftPosition)
{
    GraphWorxManager::getInstance().loadComplete(leftPosition);
}

void ControlStation2Impl::getSessionId(StringPromisePtr sessionId)
{
    sessionId->set_value(SessionDetails::getInstance().getSessionId());
}

void ControlStation2Impl::getCurrentDisplayName(StringPromisePtr display)
{
    display->set_value(GraphWorxManager::getInstance().getCurrentDisplay(1));
}

void ControlStation2Impl::getRect(EScreen targetScreen, EArea targetArea, long val, boost::shared_ptr<Promise<RECT>> dim)
{
    dim->set_value(ScreenPositioning::getInstance().getRect(static_cast<TA_GenericGui::EScreen>(targetScreen),
                                                            static_cast<TA_GenericGui::EArea>(targetArea),
                                                            val));
}

void ControlStation2Impl::setDisplayDimension(RECT dim)
{
    ScreenPositioning::getInstance().setDisplayDimension(dim);
}

void ControlStation2Impl::launchDisplayEx(const std::string& display, const std::string& asset, long leftPosition)
{
    LOG_INFO("Trying to launch display %s with asset %s on screen %li", display, asset, leftPosition);

    DisplayInformation* info = new DisplayInformation;
    *info = { display.c_str(), asset.c_str(), leftPosition };
    //TODO:
    ControlStationRootDialog::getInstance().postMessage(WM_LAUNCH_DISPLAY, (WPARAM)info);
}

void ControlStation2Impl::changePassword()
{
    //TODO: change-password-task
    //boost::async();
}

void ControlStation2Impl::isOperatorOverridden(BoolPromisePtr isOverridden)
{
    isOverridden->set_value(SessionDetails::getInstance().isOperatorOverridden());
}

void ControlStation2Impl::changeOperatorOverride()
{
    //TODO: changeOperatorOverride-task
    //boost::async();
}

void ControlStation2Impl::displayTaskManager()
{
    //TODO: displayTaskManager-task
    //boost::async();
}

void ControlStation2Impl::isServerContactable(BoolPromisePtr isContactable)
{
    isContactable->set_value(ServerMonitor::getInstance().isContactable());
}

void ControlStation2Impl::logout()
{
    boost::async(LogoutTask());
}

void ControlStation2Impl::giveApplicationFocus(long processId)
{
    //TODO: do this in application manager
}

void ControlStation2Impl::getOperator(StringPromisePtr operatorName)
{
    std::string theOperatorName = SessionDetails::getInstance().getOperatorName();

    if (SessionDetails::getInstance().isOperatorOverridden())
    {
        theOperatorName += TA_ControlStation::NAME_SEPARATOR;
        theOperatorName += SessionDetails::getInstance().getOverrideOperatorName();
    }

    operatorName->set_value(theOperatorName);
}

void ControlStation2Impl::getSite(StringPromisePtr site)
{
    // Site is the physical location of the operator. That is, where is their console sitting.
    site->set_value(SessionDetails::getInstance().getConsoleLocationName());
}

void ControlStation2Impl::getProfile(StringPromisePtr profile)
{
    std::string theProfile = SessionDetails::getInstance().getProfileAndLocationDisplayString();

    if (SessionDetails::getInstance().isOperatorOverridden())
    {
        theProfile += TA_ControlStation::NAME_SEPARATOR;
        theProfile += SessionDetails::getInstance().getOverrideProfileAndLocationDisplayString();
    }

    profile->set_value(theProfile);
}

void ControlStation2Impl::launchApplicationEx(long appType, std::string extraCommandLine, long posType, long alignType, RECT objectDim, RECT boundaryDim)
{
    LOG_INFO("Graphworx has requested an application be launched");
    //LOGMORE(SourceInfo, "Application Type: %d PosType,AlignType: (%d,%d) ObjectDim,BoundaryDim: (%p,%p) Additional commandline: %s", appType, posType, alignType, objectDim, boundaryDim, commandStdString.c_str());
    //LOGMORE(SourceInfo, "ObjectRect(%d,%d,%d,%d), BoundaryRect(%d,%d,%d,%d)", objectRect.left, objectRect.top, objectRect.right, objectRect.bottom, boundaryRect.left, boundaryRect.top, boundaryRect.right, boundaryRect.bottom);
    ApplicationSignal::launchApplication(appType, extraCommandLine, posType, alignType, objectDim, boundaryDim, false);
}

void ControlStation2Impl::getAllApplications(boost::shared_ptr<Promise<std::vector<ApplicationProperties>>> applications)
{
    applications->set_value(ApplicationManager::getInstance().getApplications());
}

void ControlStation2Impl::getRunningApplications(boost::shared_ptr<Promise<std::map<unsigned long, std::string>>> runningApplications)
{
    runningApplications->set_value(RunningApplicationManager::getInstance().getRunningApplications());
}

void ControlStation2Impl::getLocation(StringPromisePtr location)
{
    // Location is the conceptual location of the operator.
    location->set_value(SessionDetails::getInstance().getLocationDisplayName());
}

void ControlStation2Impl::setSelectedAlarms(const std::string& asset, long leftPosition)
{
    //TODO:
}

void ControlStation2Impl::getProfileDisplay(long leftPosition, StringPromisePtr profileDisplay)
{
    unsigned long screen = ScreenPositioning::getInstance().getWhichScreenCoordinateIsOn(leftPosition);
    profileDisplay->set_value(SessionDetails::getInstance().getProfileDisplay(screen));
}

void ControlStation2Impl::getProfileDisplayAtLocation(long leftPosition, const std::string& locationName, StringPromisePtr profileDisplay)
{
    unsigned long screen = ScreenPositioning::getInstance().getWhichScreenCoordinateIsOn(leftPosition);
    profileDisplay->set_value(SessionDetails::getInstance().getProfileDisplayAtLocation(screen, locationName));
    LOG_INFO("Get the default display for location %s is %s", locationName.c_str(), profileDisplay->get().c_str());
}

void ControlStation2Impl::changeProfile()
{
    //TODO:
    // change-profile-task
}

void ControlStation2Impl::launchDisplay(const std::string& display, long leftPosition)
{
    LOG_INFO("Trying to launch display %s on screen %li", display, leftPosition);

    DisplayInformation* info = new DisplayInformation;
    *info = { display.c_str(), "", leftPosition };
    //TODO:
    ControlStationRootDialog::getInstance().postMessage(WM_LAUNCH_DISPLAY, (WPARAM)info);
}

void ControlStation2Impl::launchApplication(long appType, long leftPosition, long topPosition, long width, long height, const std::string& addtionalCommands)
{
    //TODO:
    //LOGMORE(SourceInfo, "Application Type: %d PosType,AlignType: (%d,%d) ObjectDim,BoundaryDim: (%p,%p) Additional commandline: %s", appType, posType, alignType, objectDim, boundaryDim, commandStdString.c_str());
    //LOGMORE(SourceInfo, "ObjectRect(%d,%d,%d,%d), BoundaryRect(%d,%d,%d,%d)", objectRect.left, objectRect.top, objectRect.right, objectRect.bottom, boundaryRect.left, boundaryRect.top, boundaryRect.right, boundaryRect.bottom);
    //ApplicationSignal::launchApplication(appType, addtionalCommands, posType, alignType, objectDim, boundaryDim, false);
}

void ControlStation2Impl::changeDisplayMode()
{
    //TODO:
    // task
}

void ControlStation2Impl::getDisplayMode(StringPromisePtr displayMode)
{
    displayMode->set_value(SessionDetails::getInstance().getDisplayMode() ? "DisplayOnly" : "DisplayNormal");
}

void ControlStation2Impl::setRunParam(const std::string& name, const std::string& value)
{
    RunParams::getInstance().set(name, value);

    if (boost::iequals(name, "DebugLevel"))
    {
        DebugUtil::getInstance().setLevel(DebugUtil::getInstance().getDebugLevelFromString(value));
    }
}
