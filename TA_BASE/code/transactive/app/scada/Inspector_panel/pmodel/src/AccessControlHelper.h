/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3002_TIP/TA_BASE/transactive/app/scada/inspector_panel/src/AccessControlHelper.h $
  * @author:  Rod Rolirad
  * @version: $Revision: #6 $
  *
  * Last modification: $DateTime: 2013/07/10 15:49:51 $
  * Last modified by:  $Author: jinmin.huang $
  * 
  * This singleton creates a map of permissions for InspectorPanel.
  */

#pragma warning ( disable:4786 )

#if !defined(AFX_ACCESSCONTROLHELPER_H__D7F32719_5D89_477E_8272_A62BC014A053__INCLUDED_)
#define AFX_ACCESSCONTROLHELPER_H__D7F32719_5D89_477E_8272_A62BC014A053__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
#include <set>
#include <list>

#include "bus/security/access_control/actions/src/AccessControlledActions.h"

#include "bus/security/rights_library/src/RightsLibrary.h"
#include "core/data_access_interface/src/ResourceAccessFactory.h"
#include "core/data_access_interface/src/IResource.h"
#include "core/threads/src/Thread.h"
#include "core/synchronisation/src/Condition.h"
#include "inspectorpanelconsts.h"

using namespace TA_Base_Core;
using namespace TA_Base_Bus;

namespace TA_Base_App
{
    struct AccessEntityInfo
    {
        unsigned long entityKey;
        unsigned long locationKey;
        unsigned long subSystemKey;
    };
    
	class IPermissionnNotification
	{
	public:
		virtual ~IPermissionnNotification(){};
	public:
		virtual void processPermissionnChange() = 0;
	};

	class AccessControlHelper  : public virtual TA_Base_Core::Thread
	{
	public:		
		virtual ~AccessControlHelper();
	public:
		static	AccessControlHelper*	getInstance();
		bool	IsActionPermitted(unsigned long aEntityKey, AccessControlActions aAccessControlType);
		void	InitAccessControlMap(const std::vector<AccessEntityInfo>& entityInfo);
		void	updateSession(const std::string& newSession);
		void	registerObserver(IPermissionnNotification* observer);
		void	unRegisterObserver(IPermissionnNotification*observer);
	protected:
		
		void	AssortSubsystem();
		void	InitEntitiesRights();
		virtual void	run();
		virtual void	terminate();
	private:
		void	notifyObservers();
		void	areActionsPermittedOnSubsystemAndLocSub(const std::string& sessionId, unsigned long resourceKey, unsigned long locationKey, unsigned long subsystemKey,
			const std::list<unsigned long> & actionKeys,std::list<unsigned long> & returnActionList );
	private:
		void InitActionsMap();
		AccessControlHelper(const AccessControlHelper& obj) {};			
		AccessControlHelper&operator=(const AccessControlHelper& rhs) {};		
		AccessControlHelper();

		static AccessControlHelper*	m_singleton;				

		//TA_Base_Core::ReEntrantThreadLockable					m_ActionsListLock;
		std::list<unsigned long>								m_ActionsList;

		// use the same lock: m_entitySubsystemMap,m_subsystemActionsMap,m_EntityInfoVector
		TA_Base_Core::ReEntrantThreadLockable					m_entitySubsystemActionLock;

		// entity -> subsystem		
		std::map<unsigned long, unsigned long >					m_entitySubsystemMap;
		// subsystem -> actionsSet
		typedef std::map<unsigned long, std::set<unsigned long> > SubsystemActionsMap;
		SubsystemActionsMap										m_subsystemActionsMap;

		std::vector<AccessEntityInfo>							m_EntityInfoVector;	
		

		volatile bool											m_isRunning;
		TA_Base_Bus::RightsLibrary*								m_pRightsLib;
		std::string												m_SessionId;

		TA_Base_Core::Condition									m_condition;

		TA_Base_Core::ReEntrantThreadLockable					m_observerLock;
		std::vector<IPermissionnNotification*>						m_observers;

	};

} // namespace

#endif // !defined(AFX_ACCESSCONTROLHELPER_H__D7F32719_5D89_477E_8272_A62BC014A053__INCLUDED_)
