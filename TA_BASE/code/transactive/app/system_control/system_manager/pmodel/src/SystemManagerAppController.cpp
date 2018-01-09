/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemManagerAppController.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  */

#include "SystemManagerAppController.h"
#include "SystemManagerConsts.h"
#include "ChangeOperationModeDlgModel.h"
#include "OptionsDlgModel.h"
#include "ProcessListCtrlModel.h"
#include "ProcessParamsDlgModel.h"
#include "ShutdownSystemControllerDlgModel.h"
#include "StartProcessDlgModel.h"
#include "StopProcessDlgModel.h"
#include "SystemControllerDlgModel.h"
#include "SystemManagerDlgModel.h"
#include "TransActiveLoginDlgModel.h"
#include "bus/common_subsystem_facade/src/SystemControllerFacade.h"
#include "bus/application_types/src/apptypes.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Bus::SettingsMgr;
using namespace TA_Base_Bus;

namespace TA_Base_App
{
    SystemManagerAppController::SystemManagerAppController(TA_Base_Bus::ITransActiveApp* pApp, const char* appName)
        : TA_Base_Bus::AbstractGUIAppController(pApp, appName),
          m_pSystemControllerAccessor(NULL)
    {
    }

    SystemManagerAppController::~SystemManagerAppController()
    {
    }

    unsigned long SystemManagerAppController::getApplicationType()
    {
        return ADMIN_GUI_APPTYPE;
    }

    void SystemManagerAppController::allocAppResource()
    {
        m_pSystemControllerAccessor = new SystemControllerAccessor();
    }

    void SystemManagerAppController::cleanup()
    {
        if (m_pSystemControllerAccessor != NULL)
        {
            delete m_pSystemControllerAccessor;
            m_pSystemControllerAccessor = NULL;
        }
    }

    //raise exception necessary after catch and log
    void SystemManagerAppController::processWorkflowDuringAppInit()
    {
        return; // LIMINTODO

        bool hasConnectedControlStation = isConnectedControlStation();

        if (! hasConnectedControlStation)
        {
            getRunParamFromSystemController();
            doLoginOperation();
        }
    }

    void SystemManagerAppController::getRunParamFromSystemController()
    {
        bool canGetParamFromSC = false;

        TA_Base_Core::RunParams::ParamVector vtRunParam;
        canGetParamFromSC = getRunParamFromSystemController(vtRunParam);

        while (!canGetParamFromSC)
        {
            TA_Base_Bus::NameValueParams params;
            m_winApp->invoke(APP_METHOD_LOCATE_SYSTEM_CONTROLLER, params);
            canGetParamFromSC = getRunParamFromSystemController(vtRunParam);
        }

        // For each RunParam we have retrieved. If it is not already set then set it.
        for (unsigned long i = 0; i < vtRunParam.size(); ++i)
        {
            if (!TA_Base_Core::RunParams::getInstance().isSet(vtRunParam[i].name.c_str()))
            {
                TA_Base_Core::RunParams::getInstance().set(vtRunParam[i].name.c_str(), vtRunParam[i].value.c_str());
            }
        }
    }

    bool SystemManagerAppController::getRunParamFromSystemController(TA_Base_Core::RunParams::ParamVector& vtRunParam)
    {
        vtRunParam.clear();
        std::string hostName = TA_Base_Core::RunParams::getInstance().get(RPARAM_HOSTNAME);

        if (!hostName.empty())
        {
            unsigned portNumber = atoi(TA_Base_Core::RunParams::getInstance().get(RPARAM_PORTNUMBER).c_str());
            m_pSystemControllerAccessor->setEndPointInfo(hostName, portNumber);

            try
            {
                m_pSystemControllerAccessor->getParams(0, m_consoleName.c_str(), vtRunParam);
                return true;
            }
            catch (...)
            {
                LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Faild to get param from system controller according to predefined config, hostname=%s", hostName.c_str());
            }
        }

        return false;
    }

    void SystemManagerAppController::doLoginOperation()
    {
        TA_Base_Bus::NameValueParams params;
        this->m_winApp->invoke(APP_METHOD_LOGIN, params);
    }

    std::string SystemManagerAppController::checkConsoleName()
    {
        // Need to get the console name of this session (if we can)
        try
        {
            boost::shared_ptr<TA_Base_Core::IConsole> theConsole(TA_Base_Core::ConsoleAccessFactory::getInstance().getConsoleFromSession(
                                                                     TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID)));

            std::string consoleName = theConsole->getName().c_str();

            return consoleName;
        }
        catch (...)
        {
            // If there are any problems, then just log that we couldn't get the console name and move on
            LOG_EXCEPTION_CATCH(SourceInfo, "General Exception",
                                "An exception was caught while attempting to retrieve the console details for this session.  Using the console name instead");
            std::string consoleName = getConsoleName();
            return consoleName;
        }
    }

    void SystemManagerAppController::prepareAppControllerRun()
    {
        if (TA_Base_Core::RunParams::getInstance().isSet("LocalTest"))
        {
            return;
        }

        m_consoleName = checkConsoleName();
    }

    void SystemManagerAppController::terminateGUI()
    {
        //TODO
    }
}
