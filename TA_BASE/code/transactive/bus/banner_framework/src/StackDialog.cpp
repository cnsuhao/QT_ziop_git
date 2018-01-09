/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/StackDialog.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  */
/*#############################################################################
# STACKDIALOG.CPP
#
# SCA Software International S.A.
# http://www.scasoftware.com
# scaadmin@scasoftware.com
#
# Copyright (c) 1999 SCA Software International S.A.
#
# Date: 17.12.1999.
# Author: Zoran M.Todorovic
#
# This software is provided "AS IS", without a warranty of any kind.
# You are free to use/modify this code but leave this header intact.
# 
#############################################################################*/

#include "bus/banner_framework/src/StackDialog.h"
#include "bus/banner_framework/src/StackedPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_Bus
{

	TStackedDialog::TStackedDialog(ITransActiveApp * pWinApp,
		std::string viewType, QWidget* pParent /*= NULL*/)
		: AbstractTransActiveDialog(pWinApp, viewType, pParent)
    {
        //init(nPlaceholder);
    }


    TStackedDialog::~TStackedDialog()
    {
		flush();
    }


	void TStackedDialog::initPlaceHolder(QFrame* placeholder)
    {
		m_nPlaceholder = placeholder;
		QRect temp = m_nPlaceholder->geometry();
    }


    void TStackedDialog::flush(void)
    {
        TStackedPageHelper *pPage;
        TStackedPageHelperList::iterator it;
        for ( it = m_PageList.begin(); it != m_PageList.end(); ++it )
        {
            pPage = &(*it);
            destroyPageDialog(pPage);
        }
        m_PageList.clear();
    }


    void TStackedDialog::destroyPageDialog(TStackedPageHelper *pPage)
    {
        if ( pPage->m_bCreated )
        {
            pPage->m_bCreated = FALSE;
            TA_ASSERT(pPage->m_pDialog != NULL, "error");
            pPage->m_pDialog->OnKillActive();
            pPage->m_pDialog->OnDestroyPage();
            
            delete pPage->m_pDialog;
            pPage->m_pDialog = NULL;
        }
    }


    TStackedPageHelper *TStackedDialog::getFirst(void)
    {
        TStackedPageHelperList::iterator it = m_PageList.begin();
        if (it != m_PageList.end())
        {
            return &(*it);
        }
        
        return NULL;
    }


    TStackedPageHelper *TStackedDialog::getCurrent(void)
    {
        TStackedPageHelper *pPage = NULL;
        TStackedPageHelperList::iterator it;
        for ( it = m_PageList.begin(); it != m_PageList.end(); ++it )
        {
            if (it->m_bActive)
            {
                pPage = &(*it);
                break;
            }
        }
        
        return pPage;
    }


    TStackedPageHelper *TStackedDialog::getNext(void)
    {
        TStackedPageHelperList::iterator it;
        for ( it = m_PageList.begin(); it != m_PageList.end(); ++it )
        {
            if (it->m_bActive)
            {
                ++it;
                if (it != m_PageList.end())
                {
                    return &(*it);
                }
                
                return getFirst();
            }
        }

        return getFirst();
    }


    void TStackedDialog::activatePage(TStackedPageHelper *pPage)
    {
        //TD14522 - whenever one of these conditions fail, the application
        // should not raise an ASSERT. However, if ever a Page is activated
        // without having fully initialized, the dialog should not even
        // attempt to show it  until all pages are initialized. 
        //ASSERT(pPage != NULL);
        //ASSERT(pPage->m_pDialog != NULL);
        //ASSERT(pPage->m_bCreated != FALSE);
        if (pPage != NULL && ((pPage->m_pDialog != NULL) && (pPage->m_bCreated == TRUE)))
        {
            TStackedPage *pDialog = pPage->m_pDialog;
            // Show and update window
			pDialog->setVisible(true);
            //pDialog->InvalidateRect(NULL);
            //pDialog->UpdateWindow();
            pDialog->OnSetActive();
            pPage->m_bActive = TRUE;
            OnPageChanged(pPage->m_nDialogID, TRUE);
        }
    }


    void TStackedDialog::deactivatePage(TStackedPageHelper *pPage)
    {
        TA_ASSERT(pPage != NULL, "Error");
		TA_ASSERT(pPage->m_pDialog != NULL, "Error");
		TA_ASSERT(pPage->m_bCreated != false, "Error");
        TStackedPage *pDialog = pPage->m_pDialog;
        pDialog->OnKillActive();
		pDialog->setVisible(false);
        pPage->m_bActive = false;
		OnPageChanged(pPage->m_nDialogID, false);
    }

    // Add a new page (associated child dialog with resource nId) to the 
    // stacked dialog. Returns a pointer to the internal helper object that 
    // maintains the state of the page
	TStackedPageHelper *TStackedDialog::AddPage(const std::string& nId, DWORD dwHelpId, DWORD dwData1, DWORD dwData2)
    {
        TStackedPageHelper page;
        page.m_nDialogID = nId;
        page.m_dwHelpID = dwHelpId;
        page.m_dwData1 = dwData1;
        page.m_dwData2 = dwData2;

        TStackedPageHelperList::iterator it;
        it = m_PageList.insert(m_PageList.end(), page);
        TStackedPageHelper *pPage = &(*it);

        pPage->m_pParent = this;

        pPage->m_pDialog = CreatePage(pPage->m_nDialogID);
        TA_ASSERT(pPage->m_pDialog != NULL, "Error");
		pPage->m_pDialog->setParent(m_nPlaceholder);
        //pPage->m_pDialog->Create(pPage->m_nDialogID,this);
        //ASSERT(IsWindow(pPage->m_pDialog->m_hWnd) != FALSE);

		pPage->m_pDialog->move(m_nPlaceholder->geometry().topLeft()); // , 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
		Qt::WindowFlags flags = pPage->m_pDialog->windowFlags();
		pPage->m_pDialog->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
		pPage->m_pDialog->setVisible(true);
        //pPage->m_pDialog->OnCreatePage();
        pPage->m_bCreated = true;

        return pPage;
    }


    // Delete page identified with a pointer to its helper object
    void TStackedDialog::DelPage(TStackedPageHelper *pPage)
    {
		TA_ASSERT(pPage != NULL, "ERROR");
        destroyPageDialog(pPage);
        
        TStackedPageHelperList::iterator it;
        for ( it = m_PageList.begin(); it != m_PageList.end(); ++it )
        {
            if (pPage == &(*it))
            {
                m_PageList.erase(it);
                break;
            }
        }
    }


    // Activate no page (actually deactivates the active page)
    void TStackedDialog::SetPage(void)
    {
        TStackedPageHelper *pOldPage = NULL;
        TStackedPageHelperList::iterator it;
        for ( it = m_PageList.begin(); it != m_PageList.end(); ++it )
        {
            if ((pOldPage == NULL) && it->m_bActive)
            {
                pOldPage = &(*it);
            }
        }

        if ( NULL != pOldPage )
        {
            deactivatePage(pOldPage);
        }
    }


    // Activate the page with the specified child dialog resource
	void TStackedDialog::SetPage(const std::string& nId)
    {
        TStackedPageHelper *pOldPage = NULL, *pNewPage = NULL;

        TStackedPageHelperList::iterator it;
        for ( it = m_PageList.begin(); it != m_PageList.end(); ++it )
        {
            if ((pNewPage == NULL) && (it->m_nDialogID == nId))
            {
                pNewPage = &(*it);
            }
            
            if ((pOldPage == NULL) && it->m_bActive)
            {
                pOldPage = &(*it);
            }
        }
        
        if (pOldPage)
        {
            deactivatePage(pOldPage);
        }
        
        if (pNewPage)
        {
            activatePage(pNewPage);
        }
    }


    // Set first page as active page
    void TStackedDialog::SetFirstPage(void)
    {
        TStackedPageHelper *pPage = getFirst();
        if (pPage)
        {
            SetPage(pPage->m_nDialogID);
        }
    }


    // Set next page as active page
    void TStackedDialog::SetNextPage(void)
    {
        TStackedPageHelper *pPage = getNext();
        if (pPage)
        {
            SetPage(pPage->m_nDialogID);
        }
    }


    // Get helper object associated with the currently active page
    TStackedPageHelper *TStackedDialog::GetPage(void)
    {
        TStackedPageHelperList::iterator it;
        for ( it = m_PageList.begin(); it != m_PageList.end(); ++it )
        {
            if (it->m_bActive)
            {
                return &(*it);
            }
        }
        return NULL;
    }


    // Get helper object associated with the specified child resource id
	TStackedPageHelper *TStackedDialog::GetPage(const std::string& nId)
    {
        TStackedPageHelperList::iterator it;
        for ( it = m_PageList.begin(); it != m_PageList.end(); ++it )
        {
            if (it->m_nDialogID == nId)
            {
                return &(*it);
            }
        }
        return NULL;
    }
}
