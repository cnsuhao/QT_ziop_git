/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ControlStationCorbaImpl.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This receives callbacks from the ManagedProcesses and will allow applications to be launched.
  * It also receives callbacks when the services have been restarted.
  * This implements the IControlStationCorbaDef corba definition which means this
  * object is what the ManagedProcess's get when they resolve the Control Station.
  */
#include "StdAfx.h"
#include "SessionSignal.h"
#include "ScreenPositioning.h"
#include "ProcessManager.h"
#include "app/system_control/control_station/src/ControlStationCorbaImpl.h"
#include "app/system_control/control_station/src/SessionManager.h"
#include "app/system_control/control_station/src/IProcessManager.h"
#include "app/system_control/control_station/src/ControlStationConstants.h"
#include "app/system_control/control_station/src/ControlStationRootDialog.h"
#include "core/exceptions/src/ApplicationException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

using TA_Base_Core::DebugUtil;

ControlStationCorbaImpl::ControlStationCorbaImpl()
{
    setupSignalConnections();
}

void ControlStationCorbaImpl::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&ControlStationCorbaImpl::activate, this));
    SessionSignal::logouted.connect(boost::bind(&ControlStationCorbaImpl::deactivate, this));
}

void ControlStationCorbaImpl::activate()
{
    TA_Base_Core::CorbaUtil::getInstance().setServantKey(this, "ControlStation");
    activateServant();
}

void ControlStationCorbaImpl::deactivate()
{
    deactivateServant();
}

void ControlStationCorbaImpl::launchAnApplication(unsigned long appType,
                                                  const char* extraCommandLine,
                                                  unsigned long posFlag,
                                                  unsigned long alignFlag,
                                                  const TA_Base_Core::IControlStationCorbaDef::RECT& objectDim,
                                                  const TA_Base_Core::IControlStationCorbaDef::RECT& boundaryDim)
{
    FUNCTION_ENTRY("launchAnApplication");

    try
    {
        ProcessManager& processManager = ProcessManager::getInstance();

        RECT objectRect;
        objectRect.left = objectDim.left;
        objectRect.top = objectDim.top;
        objectRect.right = objectDim.right;
        objectRect.bottom = objectDim.bottom;

        RECT boundaryRect;
        boundaryRect.left = boundaryDim.left;
        boundaryRect.top = boundaryDim.top;
        boundaryRect.right = boundaryDim.right;
        boundaryRect.bottom = boundaryDim.bottom;

        // Pass the application type and command line.
        processManager.launchApplication(appType, std::string(extraCommandLine), posFlag, alignFlag, objectRect, boundaryRect);
    }
    catch (TA_Base_Core::ApplicationException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionConstruction, "IControlStationCorbaDef::ApplicationException", "Application could not be launched. Since this is a CORBA method we must throw a non-TransActive exception");
        TA_Base_Core::IControlStationCorbaDef::ApplicationException ex;
        throw ex;
    }

    FUNCTION_EXIT;
}

TA_Base_Core::IControlStationCorbaDef::RECT ControlStationCorbaImpl::getRect(unsigned long targetScreen, unsigned long targetArea, long val)
{
    FUNCTION_ENTRY("getRect");
    RECT rect = ScreenPositioning::getInstance().getRect(static_cast<TA_Base_Bus::TA_GenericGui::EScreen>(targetScreen),
                                                         static_cast<TA_Base_Bus::TA_GenericGui::EArea>(targetArea),
                                                         val);
    TA_Base_Core::IControlStationCorbaDef::RECT retval;
    retval.left = rect.left;
    retval.right = rect.right;
    retval.top = rect.top;
    retval.bottom = rect.bottom;
    return retval;
}

void ControlStationCorbaImpl::launchDisplay(const char* display, const char* asset, long leftPosition)
{
    FUNCTION_ENTRY("launchDisplay");
    LOG_INFO("Trying to launch display %s with asset %s on screen %li", display, asset, leftPosition);

    DisplayInformation* info = new DisplayInformation;

    if (info != NULL)
    {
        info->display = CString(display);
        info->asset = CString(asset);
        info->position = leftPosition;
        ControlStationRootDialog::getInstance().postMessage(WM_LAUNCH_DISPLAY, (WPARAM)info);
    }
    else
    {
        LOG_WARNING("new returned a NULL for display info");
        TA_Base_Core::IControlStationCorbaDef::ApplicationException ex;
        throw ex;
    }

    FUNCTION_EXIT;
}
