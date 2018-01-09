/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActiveWinApp.h $
  * @author:  LUO HUIRONG
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime:   $
  * Last modified by:  $Author:   $
  *
  * This inherits from QApplication and implements all common functionality for
  * TransActive applications.
  */
#pragma once
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParamUser.h"
#include <QtWidgets/QApplication>
#include <QMetaType>
#include <QTranslator>
#include <string>
#include <vector>

namespace TA_Base_Core
{
    class MessageResourceException;
    class GenericGUIException;
}

Q_DECLARE_METATYPE(TA_Base_Bus::TRA_WPARAM);
Q_DECLARE_METATYPE(TA_Base_Bus::TRA_LPARAM);
//Q_DECLARE_METATYPE(unsigned long);
Q_DECLARE_METATYPE(std::string);

namespace TA_Base_Bus
{
    class GenericGUI;
    class IApplicationController;

    class TransActiveWinApp
        : public QApplication,
          public ITransActiveApp,
          public TA_Base_Core::RunParamUser
    {
        Q_OBJECT

    public:

        /** Constructor function
          *
          *
          *  @param appType
          *  @param mustConnectControlStation
          *  @return
          */
        TransActiveWinApp(int argc, char* argv[], MainViewType appType, bool mustConnectControlStation);


        /** Display help
          *
          *  __NOTE__:     it is is replaced by WinHelpInternal under VS 2005 and VS 2008
          *  @param dwData
          *  @param nCmd
          *  @return void
          */
        //virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);

        /** Display help
          *
          *
          *  @param dwData
          *  @param nCmd
          *  @return void
          */
        //virtual void WinHelpInternal(DWORD dwData, UINT nCmd = HELP_CONTEXT);

        /**
          * Destructor
          */
        virtual ~TransActiveWinApp();


        /**
          * InitInstance
          *
          * This method must be implemented to ensure that when the application is
          * started it is started correctly and the same for each TransActive application.
          *
          * __NOTE_: no need to override
          */
        virtual int startApp();


        /**
        * setStyleSheet
        * set StyleSheet
        * @param sheetFileName:  name of SyleSheet file
        */
        virtual void setStyleSheet(const std::string& sheetFileName);

        /**
        * changeLanguage
        *
        * change Language of application
        * @param languageShortcut:  language shortcur (e.g. ch, en)
        */
        virtual void switchLanguage(const std::string& languageShortcut);


        //---------- Methods Inherited from RunParamUser --------//

        /**
        * onRunParamChange
        *
        * Called whenever a RunParam is updated during run-time.
        *
        * @param name The name of the RunParam that has changed.
        * @param value The new value of the RunParam.
        */
        void onRunParamChange(const std::string& name, const std::string& value);

        // --------------------- BEGIN Interface ITransActiveApp virtual method -------------------------

    public:

        virtual const std::string& getCommandLine();


        /* virtual void setViewManager(ITransActiveViewManager * pViewManager)
         {
             TA_ASSERT(pViewManager != NULL, "ViewManager should not be NULL");
             m_pViewManager = pViewManager;
         }*/

        virtual ITransActiveViewManager* getViewManager()
        {
            return m_pViewManager;
        }

        virtual IApplicationController* getApplicationController()
        {
            TA_ASSERT(m_appController != NULL, "application controller should be initialized successfully");
            return m_appController;
        }

        virtual void invoke(const char* methodName, const NameValueParams& params) {}

        /** Get view configuration,
          *
          *  Empty function
          */
        virtual std::string getViewConfig(const char* parameterName)
        {
            return "NVL Value";
        }

        void closeAllTransActiveMessages();

        void changeAppFocus(const TA_Base_Core::EFocusType focus);

        void moveMainView(const WindowPositionInfo& viewPositionInfo, bool isFirstCalculatePosition);

        void displayAboutBox();

        virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent);

        // This function is actually just a redundancy for requestUserReply - if reply is not necessary then just ignore it.
        virtual void processUserNotification(UINotificationEventArgsPtr uiEvent) {};

        // --------------------- END Interface ITransActiveApp virtual method -------------------------

    protected:

        virtual void setCommandLine(int argc, char* argv[]) ;
        /**
          * doStopApp
          *
          * This method must be implemented to ensure that when the application is
          * closed it is shutdown correctly and the same for each TransActive application.
          *
          * __NOTE__: no need to override
          */
        virtual int doStopApp();

        /** create Application Controller
          *
          *
          *  @return IApplicationController *
          */
        virtual IApplicationController* createApplicationController() = 0;

        /** create main View
          *
          *  @return ITransActiveView*
          */
        virtual ITransActiveView* createMainView() = 0;


        /** get main View
        *
        *  @return ITransActiveView*
        */
        virtual TA_Base_Bus::ITransActiveView*  getMainView();
        /** do extra operation during launching application
          *
          *  __NOTE__: need override this method if do special initialization
          *
          *  @return void
          */
        virtual void prepareAppRun() {};

    protected:

        /** create, init and display main view
          *
          *  @return BOOL
          */
        BOOL displayMainView();

        /** launching applicatoin in InitInstance
          *
          *  if unexpected behavior or errors happened, it will raise exception
          *  @return void
          */
        void doStartApp();

        /** check main view is initialized successfully
          *
          *
          *  @return BOOL
          */
        bool isMainViewInit()
        {
            //TODO
            /*if (m_pMainWnd != NULL && ::IsWindow(m_pMainWnd->GetSafeHwnd()) )
            {
                return TRUE;
            }*/
            return false;
        }

        /**
          * get console name
          *
          * Returns the console name.  If it managed to connect to the database to get the
          *         name associated with the console, it will return that, otherwise, it
          *         wil just return the name of the console as it is.
          *
          * @return the console name.
          */
        std::string getConsoleName()
        {
            return m_appController->getConsoleName();
        }

    private:

        //TransActiveWinApp( const TransActiveWinApp& theTransActiveWinApp){ };
        //TransActiveWinApp& operator=(const TransActiveWinApp &) { }

        /**
          * outOfMemoryHandler
          *
          * Provides the standard way of handling out of memory problem
          * by throwing a bad_alloc exception
          *
          * @param size_t size
          *
          * @return Nothing as the bad_alloc exception will be thrown
          *
          * @throw bad_alloc
          */
        static int outOfMemoryHandler(size_t size);

        virtual bool notify(QObject* receiver, QEvent* event);

        /** handle GenericGUI Exception
          *
          *
          *  @param ex
          *  @return void
          */
        void handleGenericGUIException(const TA_Base_Core::GenericGUIException& ex);

        /** handle Message Resource Exception
          *
          *
          *  @param ex
          *  @return void
          */
        void handleMessageResourceException(const TA_Base_Core::MessageResourceException& ex);

        /** determine the main view is derived from AbstractTransActiveView
          *
          *
          *  @return bool
          */
        bool isTransActiveMainView();

        /** init Application Controller
          *
          *
          *  @return void
          */
        void initApplicationController();
        //TD18095,jianghp
        void switchTranslator(QTranslator& translator, const QString& filename);

    protected slots:

        void switchLanguageSlot(const std::string& languageShortcut);
        void setStyleSheetSlot(const std::string sheetFileName);

    signals:

        void changeLanguage(const std::string& languageShortcut);
        void changeStyleSheet(const std::string& sheetFileName);

    protected:

        IApplicationController*  m_appController;  ///< pointer to application controller
        MainViewType m_mainViewType; ///< main view type
        ITransActiveView* m_mainView;  ///< pointer to main view
        bool m_mustConnectControlStation; ///< must register to ControlStation when launched
        ITransActiveViewManager* m_pViewManager;  ///<  pointer to view manager

        QTranslator m_translator; /// contains the translations for this application
        QTranslator m_translatorQt; /// contains the translations for qt
        QString m_currLang; /// contains the currently loaded language
        QString m_langPath; /// Path of language files. This is always fixed to /languages.
        QString m_stylePath;

    private:

        std::string m_commandline;
    };
} // End namespace TA_Base_Bus
