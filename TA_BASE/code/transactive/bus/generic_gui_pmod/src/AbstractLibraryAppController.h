/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/AbstractLibraryAppController.h $
  * @author:  Karen Graham
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/03/07 14:59:35 $
  * Last modified by:  $Author: limin.zhu $
  * 
  * This inherits from IApplicationController.
  * This class is used by dynamic/static library components. Its main purpose is to provide PModel factory method
  */


#if !defined(AbstractLibraryApplication_AAA8CF05_F8FC_451c_B03B_791AB8385F14__INCLUDED_)
#define AbstractLibraryApplication_AAA8CF05_F8FC_451c_B03B_791AB8385F14__INCLUDED_

#include "core/threads/src/QueueProcessor.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include "core/utilities/src/RunParams.h"
#include "core/message/public_interfaces/ItaAuditing.h"
#include "core/utilities/src/DebugUtil.h"

#include "core/synchronisation/src/NonReEntrantThreadLockable.h"

#include <map>
#include <string>

namespace TA_Base_Core
{
    class IEntityData;
	class RunParamUser;
}

namespace TA_Base_Bus
{
	  

    
    /** Abstract Library Application Controller
     *
     * _NOTE_: it does not monitor some run param change: 
     * DEBUGLEVEL, DEBUGFILE, DEBUGFILEMAXSIZE, DEBUGMAXFILES, DEBUGPIDFILENAMES, DEBUGLEVELOFF, DEBUGLEVELFILE, SESSIONID
     */
    class AbstractLibraryAppController : public IApplicationController								     
    {

    public:

        /** Constructor function
         * 
         *
         *  @param pWinApp
         *  @param applicatoinName
         *  @return 
         */
        AbstractLibraryAppController(ITransActiveApp * pWinApp, const char * applicatoinName);


        virtual ~AbstractLibraryAppController();

	// ---------------------BEGIN Interface IApplicationController virtual method -------------------------	
	public:
        virtual void invoke(const char * methodName, const NameValueParams & params)  {};

		virtual void initAppController(const std::string& commandLine, bool mustConnectControlStation);

        virtual void cleanup();

        virtual void runController();

        virtual std::string getSessionId(); 

		virtual TA_Base_Bus::SessionInfo getMySessionInfo();

        virtual TA_Base_Core::IEntityData* getEntity() 
        { 
            return m_guiEntity; 
        }

		virtual unsigned long getEntityKey()
		{
			return m_guiEntity->getKey();
		};

		virtual std::string getEntityName()
		{
			return m_guiEntity->getName();
		}

		virtual  void checkEntity(TA_Base_Core::IEntityData* guiEntity);

		virtual std::string getOperatorName();

		virtual void checkCommandLine() {};

        virtual void setMainViewPosition(const unsigned long posFlag,
                               const unsigned long alignFlag,
                               const TA_Rect& objectDim,
                               const TA_Rect& boundaryDim);

        virtual void terminateFromUser();

        virtual void terminateFromControlStation();

        virtual const char * getConsoleName()
        {
            return m_consoleName.c_str();
        }

        virtual const char * getApplicationDisplayName()
        {
            return m_applicationName.c_str();
        }

        ITransActiveApp * getWinApp()
        {
            return m_winApp;
        }

		TA_Base_Core::ItaAuditing * getAuditMessageSender()
		{
			TA_ASSERT(m_auditMessageSender != NULL, "AuditMessageSender should be init successfully");
			return m_auditMessageSender;
		}

		virtual void notifyFatalErrorWhenLaunch();

        virtual void dutyChanged();

		virtual void entityChanged(const std::vector<std::string>& changes) {};


		virtual void serverIsDown() {};

		virtual void serverIsUp() {};

		virtual void getWorkAreaFromControlStation(const TA_Rect& rcNormalPosition, TA_Rect & workingArea);

        virtual ITransActivePModel * getPModel(const std::string& pModelName);


		unsigned long getApplicationType(){return 0;};


	// ---------------------END Interface ITransActiveApp virtual method -------------------------	

       /**************************************************************
	   *                 interface for ICommandGateway               *
	   **************************************************************/
	public:
		virtual void registerObserver(const char * cmdName, ICommandReceiver* observer)
		{
			m_commandReceivers.registerObserver(observer, cmdName);
		}
		virtual void unregisterObserver(const char * cmdName, ICommandReceiver* observer)
		{
			m_commandReceivers.unregisterObserver(observer, cmdName);
		}
		virtual void unregisterObserver(ICommandReceiver* observer)
		{
			m_commandReceivers.unregisterObserver(observer, NULL);
		}
		virtual void transferCommand(const char * cmdName, EventArgs * args)
		{
			m_commandReceivers.notifyObservers(cmdName, args);
		}
 

	protected:
 
		/** notification when sessionID changed
		 * 
		 *
		 *  @return void
		 */
		virtual void sessionIdChanged() {};


        /** allocate application level resource
         * 
         *
         *  @return void
         */
        virtual void allocAppResource() {};

        /** release application level resource which allocated by application controler
         * 
         *
         *  @return void
         */
        virtual void releaseAppResource() {};

        /** process work flow before display main view, do some preprocessing
         * 
         *
         *  @return void
         */
        virtual void processWorkflowDuringAppInit() {};

        /** do some preparation before almost finishing application initialization
         * 
         *
         *  @return void
         */
        virtual void prepareAppControllerRun() {};
        
        /** create presentation model based on name
         *	
         */
        virtual ITransActivePModel * createPModel(const std::string& pModelName) = 0;

   private:
	  
        /**
        * terminate GUI application
        *
        * This is called to actually close the application. This should call the appropriate
        * calls to shutdown the window properly. Its first task will be to call
        * prepareForClose().
        *
        * __NOTE__: override prepareForClose function if necessary 
        */
        void terminateGUI();

        //release the singleton 
        void releaseISCSCommonResource();

	private:

        //
        // Hide the copy constructor and assignment operator as they
        // are not needed.
        //
        AbstractLibraryAppController(const AbstractLibraryAppController& rhs){ };
        AbstractLibraryAppController& operator=(const AbstractLibraryAppController&){ };

		///////////////////////////////////////////
		// Member Variables
		///////////////////////////////////////////

        void cleanupPModels();
	
	protected:

        /// This holds a pointer to the GUI's entity.
	    TA_Base_Core::IEntityData* m_guiEntity;
     
		ITransActiveApp * m_winApp; ///<  pointer to ITransActiveApp

		std::string       m_applicationName; ///<  application name
		std::string       m_consoleName; ///<  host name of current MFT
		std::string       m_operatorName;  ///< operator name

		TA_Base_Core::ItaAuditing * m_auditMessageSender; ///<  pointer to audit message sender

		std::map<std::string, ITransActivePModel*> m_pmodels; ///< map of presentation models 
		TA_Base_Core::NonReEntrantThreadLockable m_pmodelsLock;  ///< lock for presentation models

		//TA_Base_Core::QueueProcessor<UINotificationWorkItem> m_uiNotifyWorker;
		//TA_Base_Core::WorkItemQueueProcessor<UINotificationWorkItem> m_uiNotifyWorker;

	private:

		AbstractStateChangeSubject<ICommandReceiver> m_commandReceivers;  ///< object for command center 
    };
}

#endif // !defined(AbstractLibraryApplication_AAA8CF05_F8FC_451c_B03B_791AB8385F14__INCLUDED_)
