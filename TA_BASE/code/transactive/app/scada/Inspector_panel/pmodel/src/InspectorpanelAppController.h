/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemManagerAppController.h $
* @author:  Justin Ebedes
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* This is the System Manager implementation of the 
* GenericGUI IGUIApplicationController interfaces.
*
*/

#ifndef INSPECTORPANEL_APP_CONTROLLER_H
#define INSPECTORPANEL_APP_CONTROLLER_H
#include <string>

#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"
#include "bus/scada/proxy_library/src/ScadaProxyFactory.h"
#include "bus/security/rights_library/src/RightsLibrary.h"

#include "InspectorPanelConsts.h"
#include "AccessControlHelper.h"
#include "Equipment.h"
#include "DisplayLogic.h"
#include "OutputLogic.h"


using namespace TA_Base_Bus;

namespace TA_Base_App
{
	//class SessionHandler;

	class InspectorpanelAppController : public TA_Base_Bus::AbstractGUIAppController
	{

	public:

		/**
		* Constructor
		*/
		InspectorpanelAppController(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName);

		/**
		* Destructor
		*/
		~InspectorpanelAppController();


		/**
		* getApplicationType
		* 
		* This returns the application type for this GUI. GenericGUI will then
		* save this in the RunParams and will use it to pass to the Control Station. 
		* In the existing system the application type would need to be hard-coded 
		* into here. This method must be implemented by the lowest level class.
		*
		* @return unsigned long representing the application type
		*
		*/
		virtual unsigned long getApplicationType();

		virtual TA_Base_Bus::ITransActivePModel * createPModel(const std::string & pModelName);

		//bool					requestUserReply(unsigned int messageId, const std::vector<std::string>& values);
	public:
		CEquipment*				getEquipment();
		DisplayLogic*			getDisplayLogic();
		OutputLogic*			getOutputLogic();

		AccessControlHelper*	getAccessControlHelper();

		void					showStatusMessage(const std::string& msg);
		void					showStatusMessage(int MsgId, const std::vector<std::string>	&parameters);
		bool					isActionPermitted(unsigned long aEntityKey, AccessControlActions aAccessControlType);

	protected:
		virtual void	onRunParamChange(const std::string& name, const std::string& value);
		virtual void	sessionIdChanged();
		void cleanup();

		void allocAppResource();

		void processWorkflowDuringAppInit();
		void prepareAppControllerRun();
	
	private:
		// Copy constructor and operator= made private to prevent them being used.
		InspectorpanelAppController(const InspectorpanelAppController&);
		InspectorpanelAppController& operator=(const InspectorpanelAppController&);

	private:
		std::string			m_equipmentName;
		TA_Base_Bus::RightsLibrary*		m_pRightsLib;
		ScadaProxyFactory*	m_scadaProxyFactory;

		TA_Base_App::AccessControlHelper*	m_pAccessControlHelper;
		TA_Base_App::CEquipment*				m_pEquipment;
		TA_Base_App::DisplayLogic*			m_displayLogic;
		TA_Base_App::OutputLogic*			m_outputLogic;
	};

} // namespace TA_Base_App

#endif // SYSTEMMANAGERGUI_H
