/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManagerDlgModel.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class represents the main Rihgts Manager dialog window.
  *
  */


#include "app/security/DutyManager/pmodel/src/DutyManagerDlgModel.h"
#include "app/security/DutyManager/pmodel/src/DutyManagerAppController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using TA_Base_Core::DebugUtil;

using namespace TA_Base_App;



DutyManagerDlgModel::DutyManagerDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
    :TA_Base_Bus::AbstractPModel(appController, pmodelName),
	TA_Base_App::AppControllerUser(appController)
{
    FUNCTION_ENTRY("DutyManagerDlgModel()");

    FUNCTION_EXIT;
}


DutyManagerDlgModel::~DutyManagerDlgModel()
{
    FUNCTION_ENTRY("~DutyManagerDlgModel()");

	unregisterUpdate();
	//TA_Base_Core::MessageSubscriptionManager::getInstance().unsubscribeToMessages(this);
	
	FUNCTION_EXIT;
}


void DutyManagerDlgModel::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
{

	int typeCode = -1;
	if (NULL == args)
	{
		typeCode = -1;
	}
	else
	{
		typeCode = args->getTypeCode();
	}

	if (_stricmp(updateType, PRESENTATION_DATA) == 0)
	{
		switch(typeCode)
		{
			case PRESENTATION_SIGNAL:
				break;
			default:
				LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"notifyUpdate no typecode:%d",typeCode);
				break;
		}
	}
	else if (_stricmp(updateType, VIEW_AND_PRESENTATION_DATA) == 0)
	{
		switch(typeCode)
		{
		case VIEW_AND_PRESENTATION_SIGNAL:
			break;

		default:
			LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"notifyUpdate no typecode:%d",typeCode);
			break;
		}


		notifyViews(updateType, args);
	}
	else // _stricmp(updateType, VIEW_DATA) == 0
	{
		notifyViews(updateType, args);
	}
}


void DutyManagerDlgModel::loadData()
{
	// Register with Admin Manager as an observer
	registerUpdate();


	m_pDmAppController->loadData();
}

void DutyManagerDlgModel::setTitle(std::string& title)
{
	m_pDmAppController->setTitleText(title);
}
//
//void DutyManagerDlgModel::displayHelp(bool display)
//{
//	pDmBridge->displayHelp(display);
//}

void DutyManagerDlgModel::OnTransActiveHelp()
{
	 m_pDmAppController->OnTransActiveHelp();
}

void DutyManagerDlgModel::OnAppAbout()
{
    m_pDmAppController->OnAppAbout();
}


//std::string DutyManagerDlgModel::getLocationNameByLocationKey(const unsigned long locationKey)
//{
//	return m_pDmAppController->getLocationNameByLocationKey(locationKey);
//}

void DutyManagerDlgModel::registerUpdate()
{
	DutyManagerBridge::getInstance()->registerObserver(this);
}

void DutyManagerDlgModel::unregisterUpdate()
{
	DutyManagerBridge::getInstance()->unregisterObserver(this);
}

