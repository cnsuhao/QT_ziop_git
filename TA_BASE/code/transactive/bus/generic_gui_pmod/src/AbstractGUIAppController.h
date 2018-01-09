/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/AbstractGUIAppController.h $
  * @author:  Karen Graham
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/03/07 14:59:35 $
  * Last modified by:  $Author: limin.zhu $
  *
  * This inherits from IApplicationController and IGUIApplicationConfig and implements all methods that are common
  * between dialogs, property sheets and documents. It also introduces some protected methods to
  * be implemented by child classes to aid in consistency.
  */


#if !defined(AbstractGUIApplication_AAA8CF05_F8FC_451c_B03B_791AB8385F14__INCLUDED_)
#define AbstractGUIApplication_AAA8CF05_F8FC_451c_B03B_791AB8385F14__INCLUDED_

#include "core/threads/src/QueueProcessor.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include "core/utilities/src/RunParamUser.h"
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
    class SettingsMgr;
    class GenericGUI;
    class ISecuritySubsystemAccessor;

    class UINotificationWorkItem
    {
    };

    ///** Simple Implementation for Interface ICommandGateway. It is thread-safe to register/unregister command receiver and transfer command
    // *
    // *  __NOTE__: It does not maintain a queue to receive commands from ICommandReceiver and the process is synchronous, the sender will free the content of memory when finished sending command
    // *  all ICommandReciever in method transferCommand
    // */
    //class SimpleCommandGateway : public  ICommandGateway
    //{
    //public:
    //    ~SimpleCommandGateway() {};

    //    /// data structure to hold command name
    //    typedef std::set<std::string> CommandSet;

    //};


    /** Abstract GUI Application Controller
      *
      * _NOTE_: it will monitor some run param change:
      * DEBUGLEVEL, DEBUGFILE, DEBUGFILEMAXSIZE, DEBUGMAXFILES, DEBUGPIDFILENAMES, DEBUGLEVELOFF, DEBUGLEVELFILE, SESSIONID
      */
    class AbstractGUIAppController : public IApplicationController, public TA_Base_Core::RunParamUser
    {
    public:

        AbstractGUIAppController(ITransActiveApp* pWinApp, const char* applicatoinName);
        virtual ~AbstractGUIAppController();

        // ---------------------BEGIN Interface IApplicationController virtual method -------------------------

    public:

        virtual void invoke(const char* methodName, const NameValueParams& params)  {};
        virtual void initAppController(const std::string& commandLine, bool mustConnectControlStation);
        virtual void cleanup();
        virtual void runController();
        virtual std::string getSessionId();
        virtual TA_Base_Bus::SessionInfo getMySessionInfo();
        virtual TA_Base_Core::IEntityData* getEntity() { return m_guiEntity; }
        virtual unsigned long getEntityKey() { return m_guiEntity->getKey(); }
        virtual std::string getEntityName() { return m_guiEntity->getName(); }
        virtual  void checkEntity(TA_Base_Core::IEntityData* guiEntity);
        virtual std::string getOperatorName();
        virtual void checkCommandLine() {};
        virtual void setMainViewPosition(const unsigned long posFlag,
                                         const unsigned long alignFlag,
                                         const TA_Rect& objectDim,
                                         const TA_Rect& boundaryDim);
        virtual void terminateFromUser();
        virtual void terminateFromControlStation();
        virtual const char* getConsoleName() { return m_consoleName.c_str(); }
        virtual const char* getApplicationDisplayName() { return m_applicationName.c_str(); }
        ITransActiveApp* getWinApp() { return m_winApp; }
        TA_Base_Core::ItaAuditing* getAuditMessageSender() { return m_auditMessageSender; }
        virtual void notifyFatalErrorWhenLaunch();
        virtual void dutyChanged();
        virtual void entityChanged(const std::vector<std::string>& changes) {};
        virtual void serverIsDown() {};
        virtual void serverIsUp() {};
        virtual void getWorkAreaFromControlStation(const TA_Rect& rcNormalPosition, TA_Rect& workingArea);
        virtual ITransActivePModel* getPModel(const std::string& pModelName);

        // ---------------------END Interface ITransActiveApp virtual method -------------------------

        /**************************************************************
        *                 interface for ICommandGateway               *
        **************************************************************/

    public:

        virtual void registerObserver(const char* cmdName, ICommandReceiver* observer)
        {
            m_commandReceivers.registerObserver(observer, cmdName);
        }
        virtual void unregisterObserver(const char* cmdName, ICommandReceiver* observer)
        {
            m_commandReceivers.unregisterObserver(observer, cmdName);
        }
        virtual void unregisterObserver(ICommandReceiver* observer)
        {
            m_commandReceivers.unregisterObserver(observer, NULL);
        }
        virtual void transferCommand(const char* cmdName, EventArgs* args)
        {
            m_commandReceivers.notifyObservers(cmdName, args);
        }

    public:

        bool isConnectedControlStation();

    protected:

        /**
          * do action when close main view
          *
          * This method is used to prepare the GUI for closing. It should save the user settings
          * and perform any other tidying up. eg Stop any processing threads, ask the user if they
          * wish to save anything etc.
          * This should be called before any close is done on the GUI whether initiated by the
          * GUI itself or the Control Station. This means it should be the first call in the
          * implementation of terminateGUI().
          */
        virtual void prepareForClose();
        virtual void saveAllUserSettings();
        virtual void loadAllUserSettings();
        virtual void sessionIdChanged() {};
        virtual void allocAppResource() {};
        virtual void releaseAppResource() {};
        virtual void processWorkflowDuringAppInit() {};
        virtual void prepareAppControllerRun() {};
        virtual ITransActivePModel* createPModel(const std::string& pModelName) = 0;
        virtual void onRunParamChange(const std::string& name, const std::string& value);
        virtual void registerRunParamChange();

    private:

        TA_Rect getMainViewRect();

        /**
          * terminate GUI application
          *
          * This is called to actually close the application. This should call the appropriate
          * MFC calls to shutdown the window properly. Its first task will be to call
          * prepareForClose().
          *
          * __NOTE__: override prepareForClose function if necessary
          */
        void terminateGUI();

        //release the singleton
        void releaseISCSCommonResource();

        /**
          * saveMainViewPositionSettings()
          *
          * This saves the size and position settings of the application and should be called from
          * saveAllUserSettings(). The position stuff has been separated out as it is generally
          * the same for all applications.
          *
          * __NOTE__: This should only save size and position settings if it is configured in the
          * system that it should do so.
          *
          * @exception UserSettingsException - Thrown for a variety of reasons but it basically means
          *                                    that the window position cannot be saved.
          */
        void saveMainViewPositionSettings();

        TA_Base_Bus::SettingsMgr* getUserSetting() { return m_userSettings; }
        void setUserSetting(TA_Base_Bus::SettingsMgr& userSettings);

    private:

        //
        // Hide the copy constructor and assignment operator as they
        // are not needed.
        //
        AbstractGUIAppController(const AbstractGUIAppController& rhs) { };
        AbstractGUIAppController& operator=(const AbstractGUIAppController&) { };

        /**
          * setMainViewPosition
          *
          * This will move the position of the window to the position specified in m_leftWindowPosition
          * and m_topWindowPosition. If these values are negative then no setting will be applied.
          *
          */
        void setMainViewPosition();

        /** choose the correct application name
          *
          *
          *  @return void
          */
        void processApplicationName();

        /** move main view based on user setting when launching application
          *
          *
          *  @return void
          */
        void moveMainViewWhenStartedBasedOnUserSetting();

        void configureHelpFile();

        ///////////////////////////////////////////
        // Member Variables
        ///////////////////////////////////////////

        void cleanupPModels();

    protected:

        /// This holds a pointer to the GUI's entity.
        TA_Base_Core::IEntityData* m_guiEntity;

        /// The user settings manager is created and saved here
        TA_Base_Bus::SettingsMgr* m_userSettings;


        /// The information required to move the correct application into the correct position.
        WindowPositionInfo m_positionInfo;

        /// True if it is the first time the window position is calculate. This is needed as applications that are calculated not to move
        bool m_isFirstCalculatePosition;

        GenericGUI* m_genericGUI; ///< pointer to GenericGUI

        ITransActiveApp* m_winApp; ///<  pointer to ITransActiveApp

        std::string m_applicationName; ///<  application name
        std::string m_consoleName; ///<  host name of current MFT
        std::string m_operatorName;  ///< operator name

        TA_Base_Core::ItaAuditing* m_auditMessageSender;  ///<  pointer to audit message sender

        std::map<std::string, ITransActivePModel*> m_pmodels; ///< map of presentation models
        TA_Base_Core::NonReEntrantThreadLockable m_pmodelsLock;  ///< lock for presentation models

        //TA_Base_Core::QueueProcessor<UINotificationWorkItem> m_uiNotifyWorker;
        //TA_Base_Core::WorkItemQueueProcessor<UINotificationWorkItem> m_uiNotifyWorker;

    private:

        AbstractStateChangeSubject<ICommandReceiver> m_commandReceivers;  ///< object for command center
    };
}

#endif // !defined(AbstractGUIApplication_AAA8CF05_F8FC_451c_B03B_791AB8385F14__INCLUDED_)
