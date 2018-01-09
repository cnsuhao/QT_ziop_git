/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemManager.h $
  * @author:  Justin Ebedes
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the main class for the System Manager.
  * It does not inherit from TransactiveWinApp, as
  * the System Manager must be able to run without
  * a Control Station. Instead it inherits directly
  * from CWinApp and re-implements some of the
  * TransactiveWinApp code.
  *
  */

#if !defined(AFX_SYSTEMMANAGER_H__DDF277E9_975E_4C52_8E47_A00562AE406D__INCLUDED_)
#define AFX_SYSTEMMANAGER_H__DDF277E9_975E_4C52_8E47_A00562AE406D__INCLUDED_

#pragma once
#include "bus/generic_gui_view/src/TransActiveWinApp.h"
#include "pmodel/src/SystemManagerAppController.h"


namespace TA_Base_Bus
{
    class GenericGUI; // Forward declaration
}

namespace TA_Base_App
{

    class SystemManager : public TA_Base_Bus::TransActiveWinApp
    {
    public:
        SystemManager();

        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(SystemManager)
    public:

        std::string getOperatorName();
        //}}AFX_VIRTUAL


        //{{AFX_MSG(SystemManager)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

        TA_Base_Bus::IApplicationController* createApplicationController();

        CWnd* createMainView(UINT& dlgID);

        void invoke(const char* methodName, const TA_Base_Bus::NameValueParams& params);

        virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent);

        /** create Application Controller
        *
        *
        *  @return IApplicationController *
        */
        virtual IApplicationController* createApplicationController() {
            /*TODO*/ return NULL;
        }

        /** create main View
        *
        *  @return ITransActiveView*
        */
        virtual ITransActiveView* createMainView() {
            /*TODO*/ return NULL;
        }

    private:

        // Copy constructor and operator= made private to prevent them being used.
        SystemManager(const SystemManager&);
        SystemManager& operator=(const SystemManager&);

        /**
          * runWithoutControlStation
          *
          * If the System Manager cannot connect to the Control Station then it must attempt to
          * run via other means. This method attempts to achieve this.
          *
          * @return This returns a boolean indicating whether the application should enter
          * the application message pump:
          *    True for Yes
          *    False for No
          */
        bool runWithoutControlStation();


        /**
          * submitAppStartedAuditMessage
          *
          * Submit an audit message with the name of the console on which this application is running
          *
          */
        void submitAppStartedAuditMessage();

    private:
        SystemManagerAppController* getSystemMangerController()
        {
            SystemManagerAppController* pController = dynamic_cast<SystemManagerAppController*>(m_appController);
            TA_ASSERT(pController != NULL, "SystemManger application controller should be valid");
        }
        void doLogin();
        void locateSystemController();
    };

} // namespace TA_Base_App

#endif // !defined(AFX_SYSTEMMANAGER_H__DDF277E9_975E_4C52_8E47_A00562AE406D__INCLUDED_)
