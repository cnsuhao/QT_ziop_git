#ifndef IGUIApplicationController_H_
#define IGUIApplicationController_H_



#include "bus/generic_gui_common/src/IRuntimeEnvionmentNotification.h"
#include "bus/generic_gui_common/src/CommandGateway.h"
#include "bus/generic_gui_common/src/CommonDataType.h"


#include "bus/generic_gui_common/src/ITransActiveWinApp.h"
#include "bus/security/authentication_library/src/SessionInfo.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/message/public_interfaces/ItaAuditing.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{
	class ITransActivePModel;
	class IGUIApplicationController;
	class ITransActiveView;
	/** use to store parameter name and value's pointer
	*	
	*/
	 
    /** Interface for GUI application controller.
    *   class AbstractGUI of older GenericGUI framework, used to control and coordinate presentation model behaviors
    *   It needs to allocate and cleanup application level resource, most of the business object should be maintained by this class object
    *   This class is a central class of GenericGUI framework: 
    *
    *   -  Most of application level data will be kept by it, such as entity data, session info, etc.  
    *       Moreover, you can initialize other application-level business object from this class
    *   -	It will monitor server state change, such as GroupOnline and GroupOffline
    *   -	It will monitor runtime parameter change, session id change and duty change
    *   -	It should keep some necessary service object reference, such as AuditMessageSender, which used to send AuditMessage 
    *	
    */
	class IGUIApplicationController: public IRuntimeEnvionmentNotification
                                   , public ICommandGateway
	{

	public:
		virtual ~IGUIApplicationController() {};

        /** Cleanup the application level resource.
         * 
         * 
         *  @return void
         */
        virtual void cleanup() = 0;
		
		/** Process method request based on method name. Used to control and coordinate interaction of different presentation model 
         * 
         *
         * @param methodName
         * @param params
         * @return void
         */
        virtual void invoke(const char * methodName, const NameValueParams & params) = 0;

        /** Initialize application controller.
         * 
         *  Called by TransActiveWinApp during application initialization, ofter create business object during this method
         *  @param cmdLine
         *  @param mustConnectControlStation
         *  @return void
         */
        virtual void initController(const char * cmdLine, bool mustConnectControlStation) = 0;


        /** Run controller after application has been almost initialized.
         *  Called in TransWinApp::InitInstance when main view has been initialized successfully
         *
         *  @return void
         */
        virtual void runController() = 0;


        /** Notify fatal error happened during launching application.
         *  used by TransAciveApp, delegate to GenericGUI to process the notification
         * 
         *  __NOTE__: temporary method when migrating to new GenericGUI Framework
         *  @return void
         */
        virtual void notifyFatalErrorWhenLaunch() = 0;
        
        /** Get the pointer to ITransActiveWinApp.
         * 
         *
         *  @return ITransActiveWinApp *
         */
        virtual ITransActiveWinApp * getWinApp() = 0;



		/**
		* Get Application Type.
		* 
		* This returns the application type for this GUI. GenericGUI will then save
		* this in the RunParams and will use it to pass to the Control Station. In the
		* existing system the application type would need to be hard-coded into here.
		* This method must be implemented by the lowest level class.
		* 
        * __NOTE__: the application type is the key in table GUIApplication
		* @return unsigned long representing the application type
		*
		*/
		virtual unsigned long getApplicationType() = 0;

        /** Get the host name of current MFT
         * 
         *
         *  @return const char *
         */
        virtual const char * getConsoleName() = 0;

        /** Get the application display name
         * 
         *
         *  @return const char *
         */
        virtual const char * getApplicationDisplayName() = 0;

		/** Get the entity data of current GUI application entity 
		 * 
		 *
		 *  @return TA_Base_Core::IEntityData*
		 */
		virtual TA_Base_Core::IEntityData* getEntity() = 0;

		/** Get the entity key of current GUI application entity 
		 * 
		 *
		 *  @return unsigned long
		 */
		virtual unsigned long getEntityKey() = 0;

		/** Get the entity name of current GUI application entity 
		 * 
		 *
		 *  @return std::string
		 */
		virtual std::string getEntityName() = 0;

		/** Get current operator name
		 * 
		 *
		 *  @return std::string
		 */
		virtual std::string getOperatorName() = 0;

		/**
		* getSessionId
		*
		* This will return the current sessionId. This should be called each time the sessionId
		* is needed and it will ensure the application is always using the current one.
		*
		* @return string The current sessionId.
		*/
		virtual std::string getSessionId() = 0;

		/** Get current SessionInfo data
		 * 
		 *
		 *  @return TA_Base_Bus::SessionInfo
		 */
		virtual TA_Base_Bus::SessionInfo getMySessionInfo() = 0;

		/** Get pointer to audit message sender, which used to send audit messages.
		 * 
		 *
		 *  @return TA_Base_Core::ItaAuditing *
		 */
		virtual TA_Base_Core::ItaAuditing * getAuditMessageSender() = 0;

		/**
		* checkEntity
		* 
		* This will check that the entity is valid and then store a local pointer to the
		* entity. This must be implemented in the lowest level class as this will be where
		* the GUI specific configuration details are known.
		*
		* @param guiEntity - A pointer to the gui entity to check and store.
		*
		* \exception GenericGUIException with the type set to ENTITY_CONFIGURATION_INVALID
		*             This should be thrown if the application checks the entity and it is
		*             not configured correctly for the GUI to run.
		*
		*/
		virtual void checkEntity(TA_Base_Core::IEntityData* guiEntity) = 0;
		


		/**
		* checkCommandLine
		*
		* This checks that all the appropriate RunParams have been set from the command line.
		* If anything is invalid or missing then this will throw an exception. This must be
		* implemented in the lowest level class as this will be where the GUI specific
		* command line details are known. All settings SHOULD be in the GUI entity and the
		* only additional command line parameter should be --control-entity.
		*
		* \exception GenericGUIException with the type set to COMMAND_LINE_INVALID
		*             It must throw this if the command line does not contain the correct
		*             information for the GUI to run.
		*/
		virtual void checkCommandLine() = 0;

		/**
		* Called when ControlStation terminate GUI application
		*
		* This is called to actually close the application.
		*/
		virtual void terminateFromControlStation() = 0;


		/**
		* Called when used close the GUI application
		*
		* This is called to actually close the application. All closing will be done from in
		* here and the not the application itself.
		*/
		virtual void terminateFromUser() = 0;

		/**
		* Set main view position
		*
		* This function sets the position of the window. This method does not actually
		* position the window but stores the co-ordinates for later use. (ie the window
		* cannot be moved until it has been created.
		*
		* @param posFlag - How the application is going to be positioned relative to boundary paramter
		* @param alignFlag - How the application is going to be positioned relative to point parameter
		* @param objectDim - The dimension of the object that this application will be launched relative
		*                    to (if POS_RELATIVE or POS_NOOVERLAP is specified).
		* @param boundaryDim - The dimension of the object that this application will be bounded in
		*                      (if POS_BOUNDED is specified)
		*/
		virtual void setMainViewPosition(const unsigned long posFlag,
			const unsigned long alignFlag,
			const RECT& objectDim,
			const RECT& boundaryDim) = 0;

		/** get work area from control station based on corba interface
		 *	
		 */
		virtual void getWorkAreaFromControlStation(const RECT& rcNormalPosition, RECT & workingArea) = 0;

        /** get presentation model based on name
         *	
         */
        virtual ITransActivePModel * getPModel(const std::string & pModelName) = 0;

	};
}

#endif