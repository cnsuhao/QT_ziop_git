/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/SchematicDisplay.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
// SchematicDisplay.cpp : Implementation of CSchematicDisplay
#include "StdAfx.h"
#include "SessionDetails.h"
#include "app/system_control/control_station/src/RippleControlStation_i.h"
#include "app/system_control/control_station/src/SchematicDisplay.h"
#include "app/system_control/control_station/src/SessionManager.h"
#include "app/system_control/control_station/src/ControlStationConstants.h"
#include "app/system_control/control_station/src/ControlStationRootDialog.h"
#include "app/system_control/control_station/src/IProcessManager.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Core::EntityAccessFactory;

const std::string CSchematicDisplay::REPLACEMENT_STR = "###";

/////////////////////////////////////////////////////////////////////////////
// CSchematicDisplay
CSchematicDisplay::CSchematicDisplay() : m_templateName("")
{
    // Set ourselves in the root dialog so it can perform callbacks.
    ControlStationRootDialog::getInstance().setRippleControlStationComObject(this);
}

void CSchematicDisplay::initialise()
{
    static bool initialised = false;

    if (initialised)
    {
        return;
    }

    try
    {
        // Load the Control Station entity and retrieve all the data we need
        std::string entityName = RunParams::getInstance().get(RPARAM_ENTITYNAME);

        if (entityName.empty())
        {
            return;
        }

        // get the name of the schematic that will be used by ATS (specific to 4669)
        using TA_Base_Core::ControlStation;
        ControlStation* controlStationEntity = dynamic_cast<ControlStation*>(EntityAccessFactory::getInstance().getEntity(entityName));
        m_templateName = controlStationEntity->getExtSchematic();
        initialised = true;
    }
    catch (...)
    {
        LOG_ERROR("Unable to get the control station entity from the database");
    }
}

STDMETHODIMP CSchematicDisplay::displayAtsSchematic(BSTR filePath, long leftPosition)
{
    FUNCTION_ENTRY("displayAtsSchematic");

    LOG_INFO("Trying to launch display %s on screen %li", filePath, leftPosition);

    initialise();

    DisplayInformation* info = new struct DisplayInformation;

    if (info != NULL)
    {
        info->display = CString(filePath);

        // the format is going to be something like ".\SIG\###_ATS_AXC.gdf@..."
        // we need to substitute the ### with the location name that we get from filePath
        CString tmp = m_templateName.c_str();

        if (!tmp.IsEmpty())
        {
            tmp.Replace(REPLACEMENT_STR.c_str(), info->display.Left(REPLACEMENT_STR.size()));
            info->display = tmp + info->display;
        }

        info->asset = "";
        info->position = leftPosition;
        ControlStationRootDialog::getInstance().postMessage(WM_LAUNCH_DISPLAY, (WPARAM)info);
    }
    else
    {
        LOG_WARNING("new returned a NULL for display info");
    }

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CSchematicDisplay::getCurrentSessionId(BSTR* sessionId)
{
    FUNCTION_ENTRY("getCurrentSessionId");
    CString strResult = SessionDetails::getInstance().getSessionId().c_str();
    *sessionId = strResult.AllocSysString();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CSchematicDisplay::getCurrentProfile(BSTR* profile)
{
    FUNCTION_ENTRY("getCurrentProfile");
    SessionDetails& details = SessionDetails::getInstance();

    CString strResult = details.getProfileAndLocationDisplayString().c_str();

    if (details.isOperatorOverridden())
    {
        strResult += TA_Base_App::TA_ControlStation::NAME_SEPARATOR.c_str();
        strResult += details.getOverrideProfileAndLocationDisplayString().c_str();
    }

    *profile = strResult.AllocSysString();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CSchematicDisplay::getCurrentLocation(BSTR* location)
{
    FUNCTION_ENTRY("getCurrentLocation");
    // location is the physical location of the operator. That is, where is their console
    // sitting.
    SessionDetails& details = SessionDetails::getInstance();
    CString strResult = details.getConsoleLocationName().c_str();
    *location = strResult.AllocSysString();

    FUNCTION_EXIT;
    return S_OK;
}

STDMETHODIMP CSchematicDisplay::getCurrentUserName(BSTR* userName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FUNCTION_ENTRY("getCurrentUserName");

    CString strResult = SessionDetails::getInstance().getOperatorName().c_str();

    if (SessionDetails::getInstance().isOperatorOverridden())
    {
        strResult += TA_Base_App::TA_ControlStation::NAME_SEPARATOR.c_str();
        strResult += SessionDetails::getInstance().getOverrideOperatorName().c_str();
    }

    *userName = strResult.AllocSysString();

    FUNCTION_EXIT;
    return S_OK;
}
