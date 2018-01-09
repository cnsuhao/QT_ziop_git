  /*
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

#include "InspectorpanelAppController.h"

#include "bus/application_types/src/apptypes.h"


#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"
#include "inspectorpaneldlgmodel.h"
#include "maintabcontrolmodel.h"
#include "informationlistctrlmodel.h"
#include "controllistctrlmodel.h"
#include "inhibitlistctrlmodel.h"
#include "overridelistctrlmodel.h"
#include "meterlistctrlmodel.h"
#include "tagdialogmodel.h"
#include "notesdialogmodel.h"
#include "limitlistctrlmodel.h"
#include "limitdetailllistctrlmodel.h"
#include "statusdialogmodel.h"
#include "directcontroldialogmodel.h"


using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Bus::SettingsMgr;
using namespace TA_Base_Bus;

namespace TA_Base_App
{

    InspectorpanelAppController::InspectorpanelAppController(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName)
       : TA_Base_Bus::AbstractGUIAppController(pApp, appName),
	   m_equipmentName(""),
	   m_pRightsLib(NULL),
	   m_scadaProxyFactory(NULL),
	   m_pAccessControlHelper(NULL),
	   m_pEquipment(NULL),
	   m_displayLogic(NULL),
	   m_outputLogic(NULL)
    {
    }

    InspectorpanelAppController::~InspectorpanelAppController()
    {

    }

	CEquipment* InspectorpanelAppController::getEquipment()
	{
		return m_pEquipment;
	}

	DisplayLogic*			InspectorpanelAppController::getDisplayLogic()
	{
		return m_displayLogic;
	}


	OutputLogic*			InspectorpanelAppController::getOutputLogic()
	{
		return m_outputLogic;
	}

	AccessControlHelper*	InspectorpanelAppController::getAccessControlHelper()
	{
		return m_pAccessControlHelper;
	}


	void	InspectorpanelAppController::showStatusMessage(const std::string& msg)
	{
		TA_Base_Bus::NameValueParams params;
		params["Message"] = (void*)msg.c_str();
		//params[msg] = NULL;
		m_winApp->invoke(APP_METHOD_SHOW_STATUS_MESSAGE, params);
	}

	void	InspectorpanelAppController::showStatusMessage(int MsgId, const std::vector<std::string>	&parameters)
	{
		TA_Base_Bus::NameValueParams params;
		params["MessageId"] = (void*)MsgId;
		params["Parameters"] = (void*)&parameters;
		m_winApp->invoke(APP_METHOD_SHOW_STATUS_MESSAGE, params);
	}

	bool	InspectorpanelAppController::isActionPermitted(unsigned long aEntityKey, AccessControlActions aAccessControlType)
	{
		return m_pAccessControlHelper->IsActionPermitted(aEntityKey, aAccessControlType);
	}

    unsigned long InspectorpanelAppController::getApplicationType()
    {
	    return ADMIN_GUI_APPTYPE;
    }


    void InspectorpanelAppController::allocAppResource()
    {
		m_equipmentName = TA_Base_Core::RunParams::getInstance().get(EQUIPMENT_RPARAM);
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, EQUIPMENT_RPARAM);
		TA_ASSERT (!m_equipmentName.empty(),"No equipment name passed");

		m_pAccessControlHelper = AccessControlHelper::getInstance();
		m_pEquipment		   = new CEquipment(m_equipmentName);
		m_displayLogic		   = new DisplayLogic(m_pEquipment);
		m_outputLogic		   = new OutputLogic(m_pEquipment);

    }


	void	InspectorpanelAppController::onRunParamChange(const std::string& name, const std::string& value)
	{
		AbstractGUIAppController::onRunParamChange(name,value);
		if(name == EQUIPMENT_RPARAM)
		{
			std::string newEquipment = TA_Base_Core::RunParams::getInstance().get(EQUIPMENT_RPARAM);
			m_pEquipment->updateEquipmentName(newEquipment);
		}
	}

	void	InspectorpanelAppController::sessionIdChanged()
	{
		std::string newSession = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		m_pAccessControlHelper->updateSession(newSession);
	}

    void InspectorpanelAppController::cleanup()
    {
		m_equipmentName ="";

		if(m_pRightsLib)
		{
			delete m_pRightsLib;
			m_pRightsLib = NULL;
		}
		
		if(m_scadaProxyFactory)
		{
			ScadaProxyFactory::removeInstance();
			m_scadaProxyFactory = NULL;
		}
	}

	//raise exception necessary after catch and log 
    void InspectorpanelAppController::processWorkflowDuringAppInit()
    {
		
		//TA_Base_Bus::NameValueParams params;
    }

	void InspectorpanelAppController::prepareAppControllerRun()
	{
		m_pEquipment->startProcess();
	}

    TA_Base_Bus::ITransActivePModel * InspectorpanelAppController::createPModel( const std::string & pModelName )
    {
        ITransActivePModel * pModel = NULL;
		if(pModelName == PMODEL_INSPECTOR_PANEL_DLG)
		{
			pModel = new InspectorPanelDlgModel(this,pModelName.c_str());
		}

		else if(pModelName == PMODEL_MAIN_TAB_CONTROL)
		{
			pModel = new MainTabControlModel(this,pModelName.c_str());
		}

		else if(pModelName == PMODEL_INFORMATION_LISTCTRL)
		{
			pModel = new InformationListCtrlModel(this,pModelName.c_str());
		}
		else if(pModelName == PMODEL_CONTROL_LISTCTRL)
		{
			pModel = new ControlListCtrlModel(this,pModelName.c_str());
		}
		else if(pModelName == PMODEL_INHIBIT_LISTCTRL)
		{
			pModel = new InhibitListCtrlModel(this,pModelName.c_str());
		}
		else if(pModelName == PMODEL_OVERRIDE_LISTCTRL)
		{
			pModel = new OverrideListCtrlModel(this, pModelName.c_str());
		}
		else if(pModelName == PMODEL_METER_LISTCTRL)
		{
			pModel = new MeterListCtrlModel(this, pModelName.c_str());
		}
		else if(pModelName == PMODEL_NOTES_DLG)
		{
			pModel = new NotesDialogModel(this, pModelName.c_str());
		}
		else if(pModelName == PMODEL_TAG_DLG)
		{
			pModel = new TagDialogModel(this, pModelName.c_str());
		}
		else if(pModelName == PMODEL_LIMITS_LISTCTRL)
		{
			pModel = new LimitListCtrlModel(this, pModelName.c_str());
		}
		else if(pModelName == PMODEL_LIMIT_DETAIL_LISTCTRL)
		{
			pModel = new LimitDetaillListCtrlModel(this,pModelName.c_str());
		}
		else if(pModelName == PMODEL_STATUS_DIALOG)
		{
			pModel = new StatusDialogModel(this, pModelName.c_str());
		}
		else if(pModelName == PMODEL_DIRECT_CONTROL_DIALOG)
		{
			pModel = new DirectControlDialogModel(this, pModelName.c_str());
		}

// 		AbstractEquipmentPModel* pModelCast = dynamic_cast<AbstractEquipmentPModel*>(pModel);
// 		if(pModelCast)
// 		{
// 			pModelCast->initialize(m_displayLogic,m_outputLogic);
// 		}
        return pModel;
    }


	/*
	bool	InspectorpanelAppController::requestUserReply(unsigned int messageId, const std::vector<std::string>& values)
	{
		TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(TA_Base_Bus::PARAM_ENCODE_STR_1));
		
		for(unsigned int index=0; index < values.size(); ++index)
		{
			dataContextPtr->pushParamValue(values[index]);
		}
		

		TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, messageId) ) ;
		this->getWinApp()->requestUserReply(eventArgs);

		return true;
	}*/
} 