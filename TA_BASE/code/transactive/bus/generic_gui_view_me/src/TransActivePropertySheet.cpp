/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/generic_gui_view/src/TransActivPropertySheet.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/11/29 14:23:30 $
  * Last modified by:  $Author: limin.zhu $
  * 
  * This inherits from CPropertySheet and implements all common functionality for
  * TransActive dialogs..
  */

#pragma warning(disable:4786)

#include "bus/generic_gui_view/src/TransActivePropertySheet.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/MFCControlHelper.h"
#include "bus/generic_gui_view/src/TransActiveConsts.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"
#include "bus/resources/resource.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/exceptions/src/ApplicationException.h"

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using namespace TA_Base_Bus::TA_GenericGui;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_Bus
{

    BEGIN_MESSAGE_MAP(AbstractPropertyPage, CPropertyPage)
        //{{AFX_MSG_MAP(AbstractPropertyPage)
        ON_WM_DESTROY()
        ON_MESSAGE(WM_TASETLANG, &AbstractPropertyPage::OnSetLanguage)
        //}}AFX_MSG_MAP
		ON_WM_CREATE()
	END_MESSAGE_MAP()


    AbstractPropertyPage::AbstractPropertyPage(ITransActiveWinApp * pWinApp,  UINT nIDTemplate, UINT nIDCaption)
    : AbstractTransActiveFormView(pWinApp)
    , CPropertyPage(nIDTemplate, nIDCaption)
    {
        setWindowText( nIDTemplate, nIDCaption );
    }


	int TA_Base_Bus::AbstractPropertyPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (__super::OnCreate(lpCreateStruct) == -1)
			return -1;

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to create presentation model");

		attachPModel();

		return 0;
	}

    BOOL AbstractPropertyPage::OnInitDialog()
    {

        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to create presentation model");

        attachPModel();

        TA_ASSERT(m_pModel != NULL, "Presentation Model should be initialized sucessfully");

        CDialog::OnInitDialog();

        int idd = (LONG)(LONG_PTR)this->m_lpszTemplateName;
        TATranslateDialog( idd, this );

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

        return TRUE;  // return TRUE  unless you set the focus to a control
    }



    void AbstractPropertyPage::OnDestroy()
    {
        //detach from the presentation model
        if (m_pModel != NULL)
        {
            m_pModel->detachView(this);
        }
		CPropertyPage::OnDestroy();
    }


    LRESULT AbstractPropertyPage::OnSetLanguage(WPARAM wParam, LPARAM lParam)
    {
        UINT idd = (LONG)(LONG_PTR)this->m_lpszTemplateName;
        TATranslateDialog( idd, this );
        return TRUE;
    }


    void AbstractPropertyPage::setWindowText(UINT nIDTemplate, UINT nIDCaption)
    {
        if (nIDCaption != 0)
        {
            m_strCaption = TALoadString(nIDCaption).c_str();
        }
        else
        {
            m_strCaption = TALoadDialogCaption( nIDTemplate ).c_str();
        }

        if ( m_strCaption.IsEmpty() != TRUE )
        {
            m_psp.dwFlags |= PSP_USETITLE;
            m_psp.pszTitle = const_cast<TCHAR*>( (const TCHAR*)m_strCaption );
        }
    }

    /////////////////////////////////////////////////////////////////////////////

    BEGIN_MESSAGE_MAP(AbstractPropertySheet, CPropertySheet)
        //{{AFX_MSG_MAP(AbstractPropertySheet)
        ON_WM_DESTROY()
        //}}AFX_MSG_MAP
		ON_WM_CREATE()
	END_MESSAGE_MAP()


    AbstractPropertySheet::AbstractPropertySheet(ITransActiveWinApp * pWinApp,
                                         const UINT captionId,
                                         CWnd* pParent /*=NULL*/)
        : AbstractTransActiveFormView(pWinApp)
        , CPropertySheet(captionId, pParent)
    {
    }

	int TA_Base_Bus::AbstractPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (__super::OnCreate(lpCreateStruct) == -1)
			return -1;

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to create presentation model");

		attachPModel();

		return 0;
	}

    BOOL AbstractPropertySheet::OnInitDialog()
    {

        TA_ASSERT(m_pModel != NULL, "Presentation Model should be initialized sucessfully");

        CPropertySheet::OnInitDialog();

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

        return TRUE;  // return TRUE  unless you set the focus to a control
    }

	AbstractPropertySheet::~AbstractPropertySheet()
	{
	}

    void AbstractPropertySheet::OnDestroy()
    {
        if (m_pModel != NULL)
        {
            m_pModel->detachView(this);
        }
		CPropertySheet::OnDestroy();
    }


    LRESULT AbstractPropertySheet::OnSetLanguage(WPARAM wParam, LPARAM lParam)
    {
        for ( int i = 0; i < GetPageCount(); ++i )
        {
            if ( ::IsWindow( GetPage(i)->m_hWnd ))
            {
                GetPage(i)->PostMessage(WM_TASETLANG, 0, 0);
            }
        }

        CTabCtrl* pTab = GetTabControl();
        ASSERT (pTab);

        for ( int i = 0; i < GetPageCount(); ++i )
        {
            AbstractPropertyPage* page = dynamic_cast<AbstractPropertyPage*>( GetPage(i) );

            if ( page )
            {
                TAUnicodeString caption = TALoadDialogCaption( page->getID() );

                if ( caption.IsEmpty() == FALSE )
                {
                    TC_ITEM ti;
                    ti.mask = TCIF_TEXT;
                    ti.pszText = const_cast<CString&>(caption.c_str()).GetBuffer();
                    VERIFY (pTab->SetItem (i, &ti));
                }
            }
        }

        return 0;
    }


BEGIN_MESSAGE_MAP(AbstractTransActivePropertySheet, AbstractPropertySheet)
    //{{AFX_MSG_MAP(AbstractTransActivePropertySheet)
    ON_WM_SYSCOMMAND()
    ON_COMMAND(IDOK, OnOK)
    ON_COMMAND(IDCANCEL, OnCancel)
    ON_WM_GETMINMAXINFO()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_CLOSE()
    ON_REGISTERED_MESSAGE(WM_SET_WINDOW_POSITION, OnSetWindowPosition)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


    /////////////////////////////////////////////////////////////////////////////
    // AbstractTransActivePropertySheet message handlers

    AbstractTransActivePropertySheet::AbstractTransActivePropertySheet(ITransActiveWinApp * pWinApp,
                                const UINT dialogId,    CWnd* pParent /*=NULL*/)
        : AbstractPropertySheet(pWinApp, dialogId, pParent)
    {
		m_resizeViewHelper = std::auto_ptr<ResizedViewHelper> ( new ResizedViewHelper(pWinApp, this) );


        CString caption = TALoadString(dialogId).c_str();

        if ( caption.IsEmpty() != TRUE )
        {
            m_psh.pszCaption = caption;
        }
    }


    BOOL AbstractTransActivePropertySheet::OnInitDialog()
    {
        // TODO: how to translate?

        return AbstractPropertySheet::OnInitDialog();
    }


    void AbstractTransActivePropertySheet::OnSysCommand(UINT nID, LPARAM lParam)
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
	        CPropertySheet::OnSysCommand(nID, lParam);
        }
    }


    void AbstractTransActivePropertySheet::OnOK()
    {
        OnClose();
    }


    void AbstractTransActivePropertySheet::OnCancel()
    {
        OnClose();
    }


    void AbstractTransActivePropertySheet::OnClose() 
    {
		DestroyWindow();
    }


    BOOL AbstractTransActivePropertySheet::DestroyWindow() 
    {
        LOG( SourceInfo, DebugUtil::FunctionEntry, "DestroyWindow");

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Destroy Main window");
		
		//if it is a main window, then need do some work before destroy window
		if (AfxGetMainWnd() == this)
		{
			LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "User close the window, do some work before call DestroyWindow");
			getTransActiveWinApp()->getApplicationController()->terminateFromUser();
		}
        
        LOG( SourceInfo, DebugUtil::FunctionExit, "DestroyWindow");

		if(afxContextIsDLL)
			afxContextIsDLL = FALSE;

		return AbstractPropertySheet::DestroyWindow();
    }

    
    void AbstractTransActivePropertySheet::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
    {
        CPropertySheet::OnGetMinMaxInfo(lpMMI);

        if (m_pModel == NULL)
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Main PModel has not been initialized, ignore GetMinMaxInfo message");
            return; 
        }
        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "--------OnGetMinMaxInfo");


		// Maximum size is restricted by the size of the available workstation screen (i.e. the parts not
		// taken up by banner and/or control station bar)
		// To work out that space, need to know which screen the current window is in.
        int currentScreen = -1;
        RECT rcNormalPosition;

        m_resizeViewHelper->getViewPosition(rcNormalPosition, currentScreen);
        m_resizeViewHelper->msgProcessMinMaxInfo(lpMMI, currentScreen, rcNormalPosition);

    }


    void AbstractTransActivePropertySheet::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
    {
        CPropertySheet::OnWindowPosChanging(lpwndpos);
        if (m_pModel == NULL)
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Main PModel has not been initialized, ignore WindowPosChanging message");
            return; 
        }
        LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "----------OnWindowPosChanging");

		m_resizeViewHelper->msgProcessWindowPosChanging(lpwndpos);
    }


    LRESULT AbstractTransActivePropertySheet::OnSetWindowPosition(WPARAM wParam, LPARAM lParam)
    {
        if (m_pModel == NULL)
        {
            LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Main PModel has not been initialized, ignore SetWindowPosition message");
            return true; 
        }
        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "--------OnSetWindowPosition");
        return MFCControlHelper::setWindowPosition(this, wParam, lParam);
    }

    void AbstractTransActivePropertySheet::initUIControl()
    {
        // Add "About..." menu item to system menu.

        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if (pSysMenu != NULL)
        {
            TAUnicodeString strAboutMenu = _T("About ");

			TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(RunParams::getInstance().get(RPARAM_APPNAME).c_str());
            strAboutMenu += tmpUniStr;
            strAboutMenu += _T("...");
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu.c_str());
        }
    }
}
