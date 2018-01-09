/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActiveMDIFrame.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2013/05/13 15:44:27 $
  * Last modified by:  $Author: limin.zhu $
  * 
  * This inherits from CMDIFrameWnd and implements all common functionality for
  * TransActive document MDIFrames.
  */

#include "bus/generic_gui_view/src/stdafx.h"
#include "bus/generic_gui_view/src/TransActiveMDIFrame.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/MFCControlHelper.h"
#include "bus/generic_gui_view/src/TransActiveWinApp.h"
#include "bus/generic_gui_view/src/TopmostWindowMessageSender.h"

#include "bus/generic_gui_view/src/TransActiveConsts.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"

#include "bus/resources/resource.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/exceptions/src/ApplicationException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using namespace TA_Base_Bus::TA_GenericGui;


namespace TA_Base_Bus
{
    /////////////////////////////////////////////////////////////////////////////
    // AbstractTransActiveMDIFrame

    IMPLEMENT_DYNCREATE(AbstractTransActiveMDIFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(AbstractTransActiveMDIFrame, CMDIFrameWnd)
    //{{AFX_MSG_MAP(AbstractTransActiveMDIFrame)
    ON_WM_CREATE()
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_WM_SYSCOMMAND()
    ON_WM_GETMINMAXINFO()
    ON_WM_WINDOWPOSCHANGING()
    ON_REGISTERED_MESSAGE(WM_SET_WINDOW_POSITION, OnSetWindowPosition)
    ON_MESSAGE(WM_TASETLANG,  &AbstractTransActiveMDIFrame::OnSetLanguage)
    ON_COMMAND_RANGE(LANGUAGE_ID_FIRST, LANGUAGE_ID_LAST, &AbstractTransActiveMDIFrame::OnLangCommand)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, &AbstractTransActiveMDIFrame::OnToolTipText)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, &AbstractTransActiveMDIFrame::OnToolTipText)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // AbstractTransActiveMDIFrame construction/destruction

    AbstractTransActiveMDIFrame::AbstractTransActiveMDIFrame()
    : AbstractTransActiveFormView(NULL)
    {
        CWinApp * pWinApp = AfxGetApp();
        TA_ASSERT(pWinApp != NULL, "It should be WinApp type");

        TransActiveWinApp * traApp = static_cast<TransActiveWinApp *> (pWinApp);
        TA_ASSERT(traApp != NULL, "It should be TransActiveWinApp type");

        m_pWinApp = traApp;

		m_resizeViewHelper = std::auto_ptr<ResizedViewHelper> ( new ResizedViewHelper(m_pWinApp, this) );
    }


    AbstractTransActiveMDIFrame::~AbstractTransActiveMDIFrame()
    {

    }


    /////////////////////////////////////////////////////////////////////////////
    // AbstractTransActiveMDIFrame message handlers
    
    int AbstractTransActiveMDIFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
    {
        if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        {
            return -1;
        }

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to create presentation model");

        attachPModel();

        TA_ASSERT(m_pModel != NULL, "Presentation Model should be initialized sucessfully");

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to init UI Control");

        initUIControl();

        //behavior similar to onInitGenericGUICompleted
        {
            LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to update view");

            CWaitCursor cur;
            LockWindowUpdate();
            updateView();
            UnlockWindowUpdate();
            //UpdateWindow();
        }

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to sync data to presentation model");

        syncUIDataToPModel();

        return 0;
    }

    void AbstractTransActiveMDIFrame::OnClose() 
    {
        LOG( SourceInfo, DebugUtil::FunctionEntry, "OnClose");

        if (m_pWinApp != NULL)
        {
             m_pWinApp->getApplicationController()->terminateFromUser();
        }
        CMDIFrameWnd::OnClose();

        LOG( SourceInfo, DebugUtil::FunctionExit, "OnClose");
    }


    void AbstractTransActiveMDIFrame::OnSysCommand(UINT nID, LPARAM lParam) 
    {
        // In WM_SYSCOMMAND messages, the four low-order bits of the nID parameter 
        // are used internally by Windows CE. When an application tests the value of 
        // nID, it must combine the value 0xFFF0 with the nID value by using the 
        // bitwise-AND operator to obtain the correct result.
        if ((nID & APPCOMMAND) == IDM_ABOUTBOX)
        {
            HelpLauncher::getInstance().displayAboutBox();
        }
        else
        {
	        CMDIFrameWnd::OnSysCommand(nID, lParam);
        }
        
    }


    void AbstractTransActiveMDIFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
    {
        CMDIFrameWnd::OnGetMinMaxInfo(lpMMI);
        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "--------OnGetMinMaxInfo");


        // Maximum size is restricted by the size of the available workstation screen (i.e. the parts not
        // taken up by banner and/or control station bar)
        // To work out that space, need to know which screen the current window is in.
        int currentScreen = -1;
        RECT rcNormalPosition;

        m_resizeViewHelper->getViewPosition(rcNormalPosition, currentScreen);
        m_resizeViewHelper->msgProcessMinMaxInfo(lpMMI, currentScreen, rcNormalPosition);
    }

    
    void AbstractTransActiveMDIFrame::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
    {
        CMDIFrameWnd::OnWindowPosChanging(lpwndpos);
        
        m_resizeViewHelper->msgProcessWindowPosChanging(lpwndpos);
    }


    LRESULT AbstractTransActiveMDIFrame::OnSetWindowPosition(WPARAM wParam, LPARAM lParam)
    {
        return MFCControlHelper::setWindowPosition(this, wParam, lParam);
    }

    void AbstractTransActiveMDIFrame::OnDestroy()
    {
        if (m_pModel != NULL)
        {
            m_pModel->detachView(this);
        }
		CMDIFrameWnd::OnDestroy();
    }


    void AbstractTransActiveMDIFrame::GetMessageString(UINT nID, CString& strMessage) const
    {
        CString str = TALoadString( nID ).c_str();

        if ( str.IsEmpty() != TRUE )
        {
            strMessage = str;
        }
    }


    void AbstractTransActiveMDIFrame::OnLangCommand(UINT nID)
    {
        TASetCurrentLang( nID );
        TASetDefaultLang( nID );
        TopmostWindowMessageSender::instance().postMessage( WM_TASETLANG );
    }


    LRESULT AbstractTransActiveMDIFrame::OnSetLanguage(WPARAM wParam, LPARAM lParam)
    {
        // TODO: do something for this frame

        CMDIChildWnd* pActiveChild = (CMDIChildWnd*)GetActiveFrame();

        if ( pActiveChild != NULL )
        {
            for ( CMDIChildWnd* pChild=(CMDIChildWnd*)pActiveChild->GetWindow(GW_HWNDFIRST); pChild; pChild = (CMDIChildWnd*)pChild->GetWindow(GW_HWNDNEXT) )
            {
                pChild->PostMessage(WM_TASETLANG, 0, 0 );
            }
        }

        return 0;
    }


    BOOL AbstractTransActiveMDIFrame::OnToolTipText(UINT nID , NMHDR* pNMHDR , LRESULT * pResult)
    {
        return TAOnToolTipText(nID, pNMHDR, pResult);
    }

}

BOOL TA_Base_Bus::AbstractTransActiveMDIFrame::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	if(afxContextIsDLL)
		afxContextIsDLL = FALSE;

	return __super::DestroyWindow();
}
