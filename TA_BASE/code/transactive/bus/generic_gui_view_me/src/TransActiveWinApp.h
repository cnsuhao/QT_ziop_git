/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActiveWinApp.h $
  * @author:  Karen Graham
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/12/03 14:09:46 $
  * Last modified by:  $Author: limin.zhu $
  *
  * This inherits from CWinApp and implements all common functionality for
  * TransActive applications.
  */

#if !defined(AFX_TRANSACTIVEWINAPP_H__F9076B65_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
#define AFX_TRANSACTIVEWINAPP_H__F9076B65_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#ifndef __AFXWIN_H__
//    #error include 'stdafx.h' before including this file for PCH
//#endif

#include <QtGui/QApplication>
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "core/utilities/src/DebugUtil.h"
#include <string>

namespace TA_Base_Core
{
    class MessageResourceException;
    class GenericGUIException;
};

namespace TA_Base_Bus
{
    class GenericGUI;
    class IGUIApplicationController;


    class TransActiveWinApp : public QApplication, public ITransActiveWinApp
    {
    public:
        /** Constructor function
          *
          *
          *  @param appType
          *  @param mustConnectControlStation
          *  @return
          */
        TransActiveWinApp(int argc, char *argv[],MainViewType appType, bool mustConnectControlStation);


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


    // --------------------- BEGIN Interface ITransActiveWinApp virtual method -------------------------
    public:

        virtual std::string getCommandLine();


      /*    virtual void setViewManager(ITransActiveViewManager * pViewManager)
        {
            TA_ASSERT(pViewManager != NULL, "ViewManager should not be NULL");
            m_pViewManager = pViewManager;
        }*/

        virtual ITransActiveViewManager * getViewManager()
        {
            return m_pViewManager;
        }

        virtual IGUIApplicationController * getApplicationController()
        {
            TA_ASSERT(m_appController != NULL, "application controller should be initialized successfully");
            return m_appController;
        };

        virtual void invoke(const char * methodName, const NameValueParams & params) {};

        /** Get view configuration,
          *
          *  Empty function
          */
        virtual std::string getViewConfig(const char * parameterName)
        {
            return "NVL Value";
        };

        void closeAllTransActiveMessages();

        void changeAppFocus(const TA_Base_Core::EFocusType focus);

        void moveMainView(const WindowPositionInfo & viewPositionInfo, bool isFirstCalculatePosition);

        ITransActiveView* getMainView()
        {
            TA_ASSERT( isMainViewInit(), "Main view should be initialized successfully" );

            return m_mainView;
        }

        void displayAboutBox();

        virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent) {};

        virtual void processUserNotification(UINotificationEventArgsPtr uiEvent) {};


    // --------------------- END Interface ITransActiveWinApp virtual method -------------------------

    //virtual method
    protected:

        virtual void  setCommandLine(int argc, char *argv[]) ;
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
          *  @return IGUIApplicationController *
          */
        virtual IGUIApplicationController * createApplicationController() = 0;

        /** create main View
          *
          *
          *  @param dlgID
          *  @return CWnd*
          */
        virtual ITransActiveView* createMainView() = 0;

        /** do extra operation during launching application
          *
          *  __NOTE__: need override this method if do special initialization
          *
          *  @return void
          */
        virtual void prepareAppRun() {};

    //normal method
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
        static int outOfMemoryHandler( size_t size );

        virtual bool notify(QObject * receiver, QEvent * event);

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

    protected:
        IGUIApplicationController*  m_appController;  ///< pointer to application controller
        MainViewType m_mainViewType; ///< main view type
        ITransActiveView * m_mainView; ///< pointer to main view
        bool m_mustConnectControlStation; ///< must register to ControlStation when launched
        ITransActiveViewManager * m_pViewManager; ///<  pointer to view manager
    private:
        std::string m_commandline;
    };
} // End namespace TA_Base_Bus

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSACTIVEWINAPP_H__F9076B65_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
