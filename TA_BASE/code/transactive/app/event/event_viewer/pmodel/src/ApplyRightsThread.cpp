// ApplyRights.cpp: implementation of the CApplyRights class.
//
//////////////////////////////////////////////////////////////////////

//#include "StdAfx.h"
//#include "app\event\event_viewer\src\EventViewer.h"
#include "app/event/event_viewer/pmodel/src/ApplyRightsThread.h"
//#include "app\event\event_viewer\EventListComponent\src\EventListComponent.h"
#include "app/event/event_viewer/pmodel/src/CombinedEventFilter.h"
#include "app/event/event_viewer/pmodel/src/NotifyGUIPostMessage.h"

//#include "bus\generic_gui\src\IGUIAccess.h"
#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"

#include "core\data_access_interface\src\IResource.h"
#include "core\data_access_interface\src\ResourceAccessFactory.h"

#include "core\utilities\src\FunctionEntry.h"

CApplyRightsThread::CApplyRightsThread(TA_Base_Bus::IGUIApplicationController *pControlClass) : m_pControlClass(pControlClass)
{
}

CApplyRightsThread::~CApplyRightsThread()
{

}

void CApplyRightsThread::run()
{
	FUNCTION_ENTRY("CApplyRightsThread::run");
	
	SrtActionPermittedInfo *pSrtActionPermittedInfo = new SrtActionPermittedInfo();
	
	std::auto_ptr<TA_Base_Core::IResource> resource(
        TA_Base_Core::ResourceAccessFactory::getInstance().getResourceFromEntity(m_pControlClass->getEntity()->getKey()) );
	
	TA_Base_Core::IEntityData* entityData = TA_Base_Core::EntityAccessFactory::getInstance().getEntity( resource->getKey()); 
	pSrtActionPermittedInfo->locationKey = entityData->getLocation();
	pSrtActionPermittedInfo->subsystemKey = entityData->getSubsystem();
	delete entityData;
	pSrtActionPermittedInfo->sessionId = m_pControlClass->getSessionId();
	pSrtActionPermittedInfo->resourceId = resource->getKey();
	
	NotifyGUIPostMessage::getInstance().PostMessageToMainDlg(WM_USER_APPLY_RIGHTS, reinterpret_cast<unsigned long>(pSrtActionPermittedInfo), 0);
	
	FUNCTION_EXIT;
}

void CApplyRightsThread::terminate()
{
}