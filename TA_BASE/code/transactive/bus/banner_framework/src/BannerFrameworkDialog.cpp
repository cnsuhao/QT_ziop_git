/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/BannerFrameworkDialog.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  */
// bannerDlg.cpp : implementation file
//

// Disable invalid warning ('this' is being used correctly in the ctor)
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#pragma warning(disable: 4355)  // 'this' : used in base member initializer list

#include "bus/banner_framework/src/BannerFrameworkDialog.h"
#include "bus/banner_framework/src/BannerPage.h"

#include "bus/application_types/src/apptypes.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "core/exceptions/src/ApplicationException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using TA_Base_Core::DebugUtil;

namespace TA_Base_Bus
{

	BannerFrameworkDialog::BannerFrameworkDialog(const std::string& viewType, ITransActiveApp * pWinApp,
		int xPos, int yPos, int width, int height, int tabWidth, QWidget* pParent /*=NULL*/)
		: TStackedDialog(pWinApp, viewType, pParent),
    m_xPos( xPos ),
    m_yPos( yPos ),
    m_width( width ),
    m_height( height ),
    m_hIcon( NULL ),
    m_tabs(tabWidth, height, this)
    {
		TA_Base_Bus::ViewResizeProperty resizeProperties;

        resizeProperties.canMaximise = false;
        resizeProperties.maxHeight = height;
        resizeProperties.maxWidth = width;
        resizeProperties.minHeight = height;
        resizeProperties.minWidth = width;

		m_tabs.createEx();
        //Todo: does not support anymore
		//m_resizeViewHelper->setViewResizeProperty(resizeProperties);

		this->setWindowTitle(tr("ControlStationBanner"));
    }


    /////////////////////////////////////////////////////////////////////////////
    //// BannerFrameworkDialog message handlers
    //BEGIN_MESSAGE_MAP(BannerFrameworkDialog, TStackedDialog)
	   // //{{AFX_MSG_MAP(BannerFrameworkDialog)
	   // ON_WM_GETMINMAXINFO()
	   // ON_WM_MOVE()
	   // ON_WM_CLOSE()
	   // ON_WM_DESTROY()
	   // //}}AFX_MSG_MAP
    //END_MESSAGE_MAP()


	void BannerFrameworkDialog::AddBannerPage(const std::string& pageId, const std::string& pageName,
                                            const BannerIcon& bannerIcon, BannerPage* page)
    {
        FUNCTION_ENTRY("AddBannerPage");

		std::map<std::string, BannerPage*>::iterator itPage = m_bannerPages.find(pageId);
        if ( itPage != m_bannerPages.end() )
        {
            LOG_GENERIC( SourceInfo, DebugUtil::DebugError,
                "Banner Page ID %d had already been added with name %s; Ignoring second Banner with name %s",
                pageId, itPage->second->getButtonName().c_str(), pageName.c_str() );
            
            FUNCTION_EXIT;
            return;
        }
        
		std::map<std::string, std::string>::iterator itPageIDName = m_bannerIDs.find(pageName);
        if ( itPageIDName != m_bannerIDs.end() )
        {
            LOG_GENERIC( SourceInfo, DebugUtil::DebugError,
                "Banner Page %s had already been added with ID %d; Ignoring second Banner with ID %d",
                pageName.c_str(), itPageIDName->second, pageId );
            
            FUNCTION_EXIT;
            return;
        }
        
        // Add pointer to the TStackedPage to multi-index map (indexed by pageId and buttonName)
        m_bannerPages[ pageId ] = page;
        m_bannerIDs[ pageName ] = pageId;

	    moveTabs();
        m_tabs.addButton(pageId, bannerIcon);
        AddPage(pageId);
    }


	TStackedPage* BannerFrameworkDialog::CreatePage(const std::string& nId)
    {
        if (m_bannerPages.find(nId) == m_bannerPages.end())
        {
            LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Page ID %d does not exist!", nId );
            return NULL;
        }
        
        return m_bannerPages[ nId ];
    }


	void BannerFrameworkDialog::OnPageChanged(const std::string& nId, BOOL bActivated)
    {
        // what do we want to do here?
    }


    //void BannerFrameworkDialog::OnMove(int x, int y) 
    //{
	   // CDialog::OnMove(x, y);
	   // moveTabs();	
    //}


    void BannerFrameworkDialog::moveTabs()
    {
        // TD8807: If ::IsWindow returns true m_tabs.move will succeed. This expression previously checked
        //         to see if the handle was NULL, however it is still possible for a handle to not
        //         be NULL and still not be a valid window handle. This isn't a certain fix, however it seems
        //         to be the most likely place for the original defect to occur.
		
		//QRect rect = this->geometry();
		//m_tabs.move(rect.topLeft());
		/*if( ::IsWindow( m_tabs.m_hWnd ) )
	    {
		    CRect rect;
		    GetClientRect(&rect);
		    CPoint org = rect.TopLeft();
		    ClientToScreen(&org);
		    m_tabs.move(CPoint(org.x, org.y));
	    }*/
    }


	void BannerFrameworkDialog::showPageByName(const std::string& pageName)
    {
		showPageByID(m_bannerIDs[pageName]);
    }


	void BannerFrameworkDialog::showPageByID(const std::string& pageId)
    {
	    m_tabs.selectButton( pageId );
        SetPage( pageId );
    }



    //void BannerFrameworkDialog::OnClose() 
    //{
    //    m_tabs.ShowWindow(SW_HIDE);
	   // TStackedDialog::OnClose();
    //}


    //void BannerFrameworkDialog::OnDestroy() 
    //{
	   // TStackedDialog::OnDestroy();
    //    UnregisterHotKey(this->m_hWnd,ID_HOT_KEY_ALTF4);	
    //}


	void BannerFrameworkDialog::enableButtonFlash(const std::string& dialogId)
    {
        m_tabs.flashButton(dialogId, true);
    }


	void BannerFrameworkDialog::disableButtonFlash(const std::string& dialogId)
    {
        m_tabs.flashButton(dialogId, false);
    }


	void BannerFrameworkDialog::onTabDoubleClick(const std::string& pageId)
    {
        FUNCTION_ENTRY("onTabDoubleClick");

		std::map<std::string, BannerPage*>::iterator itBannerPage = m_bannerPages.find(pageId);
        if (itBannerPage == m_bannerPages.end())
        {
            LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Button does not exist!");

            FUNCTION_EXIT;
            return;
        }

        itBannerPage->second->onButtonDoubleClick();
    }


    int BannerFrameworkDialog::getXPosition()
    {
        return m_xPos;
    }


    int BannerFrameworkDialog::getYPosition()
    {
        return m_yPos;
    }


    int BannerFrameworkDialog::getWidth()
    {
        return m_width;
    }


    int BannerFrameworkDialog::getHeight()
    {
        return m_height;
    }

    Tabs* BannerFrameworkDialog::getTabs()
    {
	    return &m_tabs;
    }

} // namespace TA_Base_Bus
