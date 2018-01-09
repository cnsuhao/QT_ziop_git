#pragma once
#include "bus/generic_gui_pmod/src/UserInteraction.h"
#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "bus/security/authentication_library/src/SessionInfo.h"
//#include "bus/generic_gui_pmod/src/ItemRepository.h"
#include "bus/generic_gui_pmod/src/CommandGateway.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/message/public_interfaces/ItaAuditing.h"
#include "core/utilities/src/DebugUtil.h"
#include "boost/shared_ptr.hpp"
#include <vector>
#include <string>
#include <map>

namespace TA_Base_Bus_Test
{
    class AbstractStateChangeSubjectTest;
}

namespace TA_Base_Bus
{
    class ITransActivePModel;
    class IApplicationController;
    class ITransActiveView;
    class ITransActiveApp;

    typedef UINT_PTR            TRA_WPARAM;
    typedef LONG_PTR            TRA_LPARAM;
    /** use to store parameter name and value's pointer
      *
      */
    typedef std::map<std::string, void*> NameValueParams;


    /** Interface for subject that send domain data change to IStateChangeObserver
      *
      */
    template<class ObserverType, class EventArgsType>
    class IStateChangeSubject
    {
    public:

        virtual ~IStateChangeSubject() {};

        /** Register observer
          *
          */
        virtual void registerObserver(ObserverType*, const char* updateType = NULL) = 0;

        /** Unregister observer
          *
          */
        virtual void unregisterObserver(ObserverType*, const char* updateType = NULL) = 0;

        /** Notify observers when domain data changed
          *
          */
        virtual void notifyObservers(const char* updateType, EventArgsType* args = NULL) = 0;
    };

    /** Simple Implementation for Interface IStateChangeSubject
      *
      */
    template<class ObserverType>
    class AbstractStateChangeSubject : public IStateChangeSubject<ObserverType, TA_Base_Bus::EventArgs>
    {
    public:

        friend TA_Base_Bus_Test::AbstractStateChangeSubjectTest;  ///< for unit test

        AbstractStateChangeSubject()
        {
        }

        virtual ~AbstractStateChangeSubject()
        {
        }

        void registerObserver(ObserverType* observer, const char* updateType = NULL)
        {
            TA_Base_Core::ThreadGuard guard(m_observerLock);

            if (NULL == updateType)
            {
                m_observersForAllUpdate.insert(observer);

                //remove unnecessary special update registration
                removeObserverFromSpecialUpdate(observer);
            }
            else
            {
                //has it been already registered for all update type
                if (m_observersForAllUpdate.find(observer) != m_observersForAllUpdate.end())
                {
                    return;
                }

                if (! exists(observer, updateType))
                {
                    m_observersForOneUpdate.insert(std::make_pair(updateType, observer));
                }
            }
        }

        void unregisterObserver(ObserverType* observer, const char* updateType = NULL)
        {
            TA_Base_Core::ThreadGuard guard(m_observerLock);

            if (NULL == updateType)
            {
                m_observersForAllUpdate.erase(observer);
            }
            else
            {
                std::pair<UpdateMultiMap::iterator, UpdateMultiMap::iterator> keyRange = m_observersForOneUpdate.equal_range(updateType);

                UpdateMultiMap::iterator it;

                for (it = keyRange.first;  it != keyRange.second;  ++it)
                {
                    if (it->second == observer)
                    {
                        m_observersForOneUpdate.erase(it);
                        break;
                    }
                }
            }
        }

        virtual void notifyObservers(const char* updateType, TA_Base_Bus::EventArgs* args = NULL)
        {
            TA_Base_Core::ThreadGuard guard(m_observerLock);

            {
                std::set<ObserverType*>::iterator it;

                // notify duty update
                for (it = m_observersForAllUpdate.begin(); it != m_observersForAllUpdate.end(); ++it)
                {
                    (*it)->notifyUpdate(updateType, args);
                }
            }

            if (updateType != NULL)
            {
                UpdateMultiMap::iterator it;
                std::pair<UpdateMultiMap::iterator, UpdateMultiMap::iterator> keyRange = m_observersForOneUpdate.equal_range(updateType);

                for (it = keyRange.first;  it != keyRange.second;  ++it)
                {
                    ObserverType* observer = it->second;
                    TA_ASSERT(m_observersForAllUpdate.find(observer) == m_observersForAllUpdate.end(), "should not register all update type");

                    observer->notifyUpdate(updateType, args);
                }
            }
        }

    protected:

        /** remove observer from special update registration
          *
          */
        void removeObserverFromSpecialUpdate(ObserverType* observer)
        {
            UpdateMultiMap::iterator it;

            for (it = m_observersForOneUpdate.begin();  it != m_observersForOneUpdate.end();)
            {
                if (it->second == observer)
                {
                    m_observersForOneUpdate.erase(it++);
                }
                else
                {
                    it++;
                }
            }
        }

        /** exist the registration or not
          *
          */
        bool exists(ObserverType* observer, const char* updateType)
        {
            if (updateType == NULL)
            {
                return m_observersForAllUpdate.find(observer) != m_observersForAllUpdate.end();
            }

            UpdateMultiMap::iterator it;
            std::pair<UpdateMultiMap::iterator, UpdateMultiMap::iterator> keyRange = m_observersForOneUpdate.equal_range(updateType);

            for (it = keyRange.first;  it != keyRange.second;  ++it)
            {
                if (it->second == observer)
                {
                    return true;
                }
            }

            return false;
        }

    protected:

        TA_Base_Core::ReEntrantThreadLockable m_observerLock;  ///< observer lock
        typedef std::multimap<std::string, ObserverType*> UpdateMultiMap;  ///<  multi-map for observer update
        std::set<ObserverType*> m_observersForAllUpdate;  ///< observer collection for all update
        UpdateMultiMap m_observersForOneUpdate; ///< observer collection for special update
    };

    /** Interface for View Manager
      *
      */
    class IViewFactory
    {
    public:
        virtual ~IViewFactory() {};

        /** get the view based on specific name
          *
          *
          * @param viewType
          * @param viewID
          * @return ITransActiveView *
          */
        virtual ITransActiveView* createView(const char* viewType) = 0;
    };


    /** Interface for View Manager
      *
      */
    class ITransActiveViewManager
    {
    public:

        virtual ~ITransActiveViewManager() {};

        /** get the view based on specific name
          *
          *
          * @param viewType
          * @param viewID
          * @return ITransActiveView *
          */
        virtual ITransActiveView* getView(const char* viewType, const char* viewID,  bool createIfNew = true) = 0;

        /** add item to the repository. If duplicate item name exists, it will return false
          *
          *
          * @param item
          * @return bool
          */
        virtual bool addView(ITransActiveView* item) = 0;

        /** remove Item from the repository. It will delete the item
          *
          *
          * @param itemName
          * @return void
          */
        virtual bool removeView(ITransActiveView* item) = 0;

        /** add item to the repository. If duplicate item name exists, it will return false
          *
          *
          * @param item
          * @return bool
          */
        virtual void setViewFactory(IViewFactory* viewFactory) = 0;

        /** close All Views
          *
          */
        virtual void closeAllViews() = 0 ;

        /** switchLanguage
        *    switch language of all views
        */
        virtual void switchLanguage() = 0;

        virtual void autoResizeAllView() = 0;

        /** get Main View
          *  there should only have one main view in an application.
          */
        virtual ITransActiveView*   getMainView() = 0;

        /*PostMessage, simulate WIN32:PostMessage
         *  The message will be queued in main thread
         *  messageID:  message ID(should be unique in one component)
         *  wParam: additional message-specific information
         *  lParam: additional message-specific information
        */
        virtual void postBoardcastMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM  lParam) = 0;

        /*sendMessage, simulate WIN32:sendMessage
        *  The message will be queued in main thread
        *  messageID:  message ID(should be unique in one component)
        *  wParam: additional message-specific information
        *  lParam: additional message-specific information
        */
        virtual void sendBoardcastMessage(unsigned long messageID, TRA_WPARAM   wParam, TRA_LPARAM  lParam) = 0;
    };


    /** Interface for TransActiveWinApp
      *
      */
    class ITransActiveApp : public IUserInteractiveReply,
        public IUserNotificationServiceCenter
    {
    public:

        virtual ~ITransActiveApp() {};

        //virtual void setViewManager(ITransActiveViewManager * pViewManager) = 0;

        virtual ITransActiveViewManager* getViewManager() = 0;

        /**
        * setStyleSheet
        * set StyleSheet
        * @param sheetFileName:  name of SyleSheet file
        */
        virtual void    setStyleSheet(const std::string& sheetFileName) = 0;

        /**
        * switchLanguage
        *
        * change Language of application
        * @param languageShortcut:  language shortcur (e.g. ch, en)
        */
        virtual void    switchLanguage(const std::string& languageShortcut) = 0;

        /** Get the pointer to application controller.
          *
          *
          * @return IApplicationController *
          */
        virtual IApplicationController* getApplicationController() = 0;

        /** process method request dynamically based on method name and name value parameters. Normally called by IApplicationController class
          *
          *
          * @param methodName
          * @param params
          * @return void
          */
        virtual void invoke(const char* methodName, const NameValueParams& params) = 0;

        /** Get specific view configuration from TransActiveWinApp. Those configuration could be gotten from user setting in db
          *
          *
          * @param parameterName
          * @return std::string
          */
        virtual std::string getViewConfig(const char* parameterName) = 0;

        /** Close all transactive message box. Normally used when exit the application
          *
          *
          * @return void
          */
        virtual void closeAllTransActiveMessages() = 0;

        /** display about dialog of GUI application
          *
          *
          * @return void
          */
        virtual void displayAboutBox() = 0;

        /** Change application focus. Normally called when launched GUI Application,
          *
          * The Control Station needs to alert the GUI applications when they should regain
          * the focus on the screen. This is the most common focus request by the Control
          * Station but it may also need to tell the applications to minimize or maximize.
          * This is done through this method. This takes an enumeration indicating how the
          * GUI should change it's focus.
          *
          * @param  focus - This indicates how the window should change it's focus.
          */
        virtual void changeAppFocus(const TA_Base_Core::EFocusType focus) = 0;

        /** get the pointer to main view
          *
          *
          * @return ITransActiveView *
          */
        virtual ITransActiveView* getMainView() = 0;

        /**
          *  move main view according to specific position.
          *
          * @param viewPositionInfo
          * @param isFirstCalculatePosition
          * @return void
          */
        virtual void moveMainView(const WindowPositionInfo& viewPositionInfo, bool isFirstCalculatePosition) = 0;

        virtual const std::string& getCommandLine() = 0;
        //virtual void  setCommandLine(std::string cmdline) =0;
        //virtual void  setCommandLine(int argc, char *argv[]) =0;

        virtual int startApp() = 0;
        //virtual void terminatedApp();
    };

    /** Interface for runtime environment change notification, including server is down/up, entity data change or duty change
      *
      */
    class IRuntimeEnvionmentNotification
    {
    public:

        virtual ~IRuntimeEnvionmentNotification() { };

        /**
          * Notification of entity data change.
          *
          * This will be called by GenericGUI when it receives a callback indicating that the
          * GUI entity has been modified. GenericGUI will have invalidated the entity database
          * object and will tell the GUI which items were modified. The GUI just has to retrieve
          * the details out of it and update accordingly. This should be implemented by the
          * lowest level class as it knows exactly what to do with the updates.
          *
          * @param changes - This is a list of all attributes changed in the
          *                                  object.
          *
          */
        virtual void entityChanged(const std::vector<std::string>& changes) = 0;

        /**
          * notify server is down.
          *
          * This is called when the server the workstation is using goes down. The GUI should
          * indicate to the user that the server is down. It should also then limit operations
          * so that the user can only view existing data. This must be implemented by the lowest
          * level class as it knows what to do when this occurs.
          */
        virtual void serverIsDown() = 0;

        /**
          * Notify server is up
          *
          * This is called when the server the workstation is using starts functioning again.
          * The GUI should indicate to the user that the server is functioning. It should
          * then allow the user to perform all valid operations again. This must be implemented by
          * the lowest level class as it knows what to do when this occurs.
          */
        virtual void serverIsUp() = 0;

        /**
          * notify that duty is changed
          *
          * The method notifies an application that duty has changed for the session in which it is running.
          * The application is responsible for re-checking all duty based functionality.
          */
        virtual void dutyChanged() = 0;
    };

    /** Interface for GUI application controller.
      *   class AbstractGUI of older GenericGUI framework, used to control and coordinate presentation model behaviors
      *   It needs to allocate and cleanup application level resource, most of the business object should be maintained by this class object
      *   This class is a central class of GenericGUI framework:
      *
      *   -  Most of application level data will be kept by it, such as entity data, session info, etc.
      *       Moreover, you can initialize other application-level business object from this class
      *   -    It will monitor server state change, such as GroupOnline and GroupOffline
      *   -    It will monitor runtime parameter change, session id change and duty change
      *   -    It should keep some necessary service object reference, such as AuditMessageSender, which used to send AuditMessage
      *
      */
    class IApplicationController: public IRuntimeEnvionmentNotification,
        public ICommandGateway
    {
    public:

        virtual ~IApplicationController() {};

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
        virtual void invoke(const char* methodName, const NameValueParams& params) = 0;

        /** Initialize application controller.
          *
          *  Called by TransActiveWinApp during application initialization, ofter create business object during this method
          *  @param cmdLine
          *  @param mustConnectControlStation
          *  @return void
          */
        virtual void initAppController(const std::string& commandLine, bool mustConnectControlStation) = 0;

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

        /** Get the pointer to ITransActiveApp.
          *
          *
          *  @return ITransActiveApp *
          */
        virtual ITransActiveApp* getWinApp() = 0;

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
        virtual const char* getConsoleName() = 0;

        /** Get the application display name
          *
          *
          *  @return const char *
          */
        virtual const char* getApplicationDisplayName() = 0;

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
        virtual TA_Base_Core::ItaAuditing* getAuditMessageSender() = 0;

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
                                         const TA_Rect& objectDim,
                                         const TA_Rect& boundaryDim) = 0;

        /** get work area from control station based on corba interface
          *
          */
        virtual void getWorkAreaFromControlStation(const TA_Rect& rcNormalPosition, TA_Rect& workingArea) = 0;

        /** get presentation model based on name
          *
          */
        virtual ITransActivePModel*  getPModel(const std::string& pModelName) = 0;
    };

    /** Interface for the observer which monitor domain data change
      *
      */
    class IStateChangeObserver
    {
    public:
        virtual ~IStateChangeObserver() {};

        /** notify presentation model when domain data has changed
          *
          *  Notify domain data change, from business logic layer to presentation logic layer,
          *  if brings status data change by pContent, then business object should be responsible to free the memory
          *  @param updateType
          *  @param args
          *  @return void
          */
        virtual void notifyUpdate(const char* updateType, TA_Base_Bus::EventArgs* args = NULL) = 0;
    };



    /** Interface for TransActiveView
      *
      *  It is responsible to display and modify the presentation data but not for storing it
      */

    class ITransActiveView: public IStateChangeObserver,
        public IUserInteractiveReply
    {
    public:

        virtual ~ITransActiveView() {};


        //************************************
        // Method:    getViewHandle
        // FullName:  TA_Base_Bus::ITransActiveView::getViewHandle
        // Access:    virtual public
        // Returns:   HWND
        // Qualifier: presentation model will use this method to access related window property, such as window rect
        //************************************

        /** Get window view Position
          *
        */
        virtual TA_Rect getViewPosition() = 0;
        virtual bool setVeiwPosition(const TA_Rect& rect) = 0;
        virtual std::string getViewID() = 0;
        virtual std::string getViewType() = 0;
        virtual void setViewID(const char* viewID) = 0;
        virtual void showView(bool modal) = 0;

        //Adjusts the size of the widget to fit its contents.
        //This function uses sizeHint() if it is valid, i.e., the size hint's width and height are >= 0. Otherwise, it sets the size to the children rectangle that covers all child widgets (the union of all child widget rectangles).
        //For windows, the screen size is also taken into account.If the sizeHint() is less than(200, 100) and the size policy is expanding, the window will be at least(200, 100).The maximum size of a window is 2 / 3 of the screen's width and height.
        virtual void autoResize() = 0;
        virtual int doModal() = 0;
        virtual void hideView() = 0;
        virtual bool closeView() = 0;

        /* PostMessage, simulate WIN32:PostMessage
        *  The message will be queued in main thread
        *  messageID:  message ID(should be unique in one component)
        *  wParam: additional message-specific information
        *  lParam: additional message-specific information
        */
        virtual void postMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam) = 0;

        /* sendMessage, simulate WIN32:sendMessage
        *  The message will be processed before return of the function.
        *  messageID:  message ID(should be unique in one component)
        *  wParam: additional message-specific information
        *  lParam: additional message-specific information
        */
        virtual void sendMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam) = 0;

        /* return presentation model name that view attach
          *
        */
        virtual std::string getMyPModelName() = 0;
        virtual bool init() = 0;
        virtual bool isMainView() = 0;
        virtual void setMainView(bool isMainView) = 0;
        virtual void translateUI() = 0;
        /*
        Used to receive message sent from view manager or view.postMessage/sendMessage.
        */
        virtual void doNotifyMessage(unsigned long messageID, TRA_WPARAM wParam, TRA_LPARAM lParam) = 0;
    };

    /** Interface for TransActiveViewModel
      *
      */
    class ITransActivePModel: public IStateChangeObserver
    {
    public:

        virtual ~ITransActivePModel() {};

        /** Allocate resource related with corresponding view
          *
          *
          *  @return void
          */
        virtual void allocViewResource() = 0;

        /** Cleanup resource related with corresponding view
          *
          *
          *  @return void
          */
        virtual void cleanup() = 0;

        /** attach view to this presentation model object
          *
          */
        virtual void attachView(ITransActiveView* pView, const char* updateType = NULL) = 0;

        /** detach view to this presentation model object
          *
          */
        virtual void detachView(ITransActiveView* pView, const char* updateType = NULL) = 0;

        /** return presentation model name
          *
          */
        virtual std::string getName() = 0;

    protected:

        /** notify views when received notification
          *
          */
        virtual void notifyViews(const char* updateType, TA_Base_Bus::EventArgs* args = NULL) = 0;
    };
}
