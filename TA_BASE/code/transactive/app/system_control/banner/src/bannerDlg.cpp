/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/banner/src/bannerDlg.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  */
// BannerDlgImpl.cpp : implementation file
//

#include "app/system_control/banner/src/banner.h"
#include "app/system_control/banner/src/BannerDlg.h"
#include "app/system_control/banner/src/CommonDef.h"
#include "app/system_control/banner/pmodel/src/BannerPModelConst.h"

#include "bus/banner_framework/src/constants.h"
#include "bus/banner_framework/src/IBannerPage.h"

#include "bus/generic_gui_pmod/src/AppLauncher.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/application_types/src/apptypes.h"


#include "core/exceptions/src/ApplicationException.h"
#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"


#include "core/data_access_interface/entity_access/src/Banner.h"

#include <QEvent.h>

using TA_Base_Core::GenericGUIException;
using TA_Base_Core::RunParams;

namespace TA_Base_App
{

    const int BannerDlgImpl::TAB_WIDTH = 60;
	
	BannerDlgImpl::BannerDlgImpl(TA_Base_Bus::ITransActiveApp * pWinApp, QWidget* parent)
		: BannerFrameworkDialog(ViewType_MainBanner, pWinApp, 0, 0, 0, 0, TAB_WIDTH, parent)
    {
	    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
		this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		m_bannerUi.setupUi(this);
		m_bannerUi.tabLayout->addWidget(&m_tabs);
		this->initPlaceHolder(m_bannerUi.frame);
    }
	
	void BannerDlgImpl::closeEvent(QCloseEvent * event)
    {
		if (true) // Todo: nee dto decide if we really need to close the application
			event->ignore();
		else
			__super::closeEvent(event);
    }

	void BannerDlgImpl::setFlashState(std::string dialogID, bool enable)
    {
        if (enable)
        {
            enableButtonFlash(dialogID);
        }
        else
        {
            disableButtonFlash(dialogID);
        }
    }


    void BannerDlgImpl::setBannerPage()
    {
        // Display the page that is configured as the default page for this profile and location.
        try
        {
            if ( false == m_pRealModel->getStartTab().empty() )
            {
				LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Banner dialog Showing Page %s", m_pRealModel->getStartTab().c_str());
                showPageByName(m_pRealModel->getStartTab());
            }
            else
            {
                LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError,
                             "start tab has no value" );
            }
        }
        catch(...)
        {
            TA_Base_Bus::TransActiveMessage userMsg;
            userMsg << m_pRealModel->getStartTab();
            UINT selectedButton = userMsg.showMsgBox(IDS_UW_020007);
        }
    }
    
	// UI Separation
	std::string BannerDlgImpl::getMyPModelName()
	{
		return PMODEL_BANNER_DLG;
	}

	void BannerDlgImpl::setupPModel()
	{
		m_pRealModel = dynamic_cast<BannerDlgPModel *> (m_pModel);
		TA_ASSERT(m_pRealModel != NULL, "PModel should be valid");

		m_pRealModel->attachView(this, PMODEL_START_PAGE_UPDATE);
	}

	void BannerDlgImpl::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args/*=NULL*/)
	{		
		std::string strUpdateType = updateType;
		LPARAM lparam = 0;
		WPARAM wParam = 0;

		if (strUpdateType.compare(PMODEL_START_PAGE_UPDATE) == 0)
		{ // Post Message
			StartTabEventArgs* strArgs = reinterpret_cast<StartTabEventArgs*>(args);
			std::string startTab = strArgs->getStartTab();
		
			QMetaObject::invokeMethod(this, "OnUpdateStartTab", Qt::AutoConnection,
				Q_ARG(const std::string, startTab));
		}
	}

	void BannerDlgImpl::initUIControl()
	{
		//this->LockWindowUpdate();

		m_xPos = m_pRealModel->getXPos();
		m_yPos = m_pRealModel->getYPos();
		m_width = m_pRealModel->getWidth();
		m_height = m_pRealModel->getHeight();
		
		this->setGeometry(m_pRealModel->getXPos(), m_pRealModel->getYPos(), m_pRealModel->getWidth(), m_pRealModel->getHeight());
		this->move(m_pRealModel->getXPos(), m_pRealModel->getYPos());
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "BannerDlgImpl::init Moved to: (X=%d,Y=%d,W=%d,H=%d)", m_pRealModel->getXPos(), m_pRealModel->getYPos(), m_pRealModel->getWidth(), m_pRealModel->getHeight());

		m_tabs.setHeight(m_pRealModel->getHeight());
		//m_tabs.create();

		// Get the dimensions of each page.
		m_pageWidth = getWidth() - TAB_WIDTH - 2;
		m_pageHeight = getHeight() - 4;
		m_pageXPos = getXPosition() + TAB_WIDTH + 2;
		m_pageYPos = getYPosition(); 

		// Set Place holder Geometry
		m_bannerUi.frame->setGeometry(m_pageXPos, m_pageYPos, m_pageWidth, m_pageHeight+1);
		
		std::vector<Component> pages = m_componentLibraryAccessor.getComponents(0, 0, m_pageWidth, m_pageHeight, this);
		
		// Add each page to the banner
		for (std::vector<Component>::iterator iter = pages.begin(); iter != pages.end(); ++iter)
		{
			try
			{
				iter->bannerPage->addBannerPage();
			}
			catch(...)
			{
				TA_Base_Bus::TransActiveMessage userMsg;
				userMsg << iter->entityName;
				userMsg.showMsgBox(IDS_UW_020012);
			}
		}

		m_pRealModel->setBannerPage();
	}

	void BannerDlgImpl::OnUpdateStartTab(std::string startTab)
	{
		// Store the new session id.
		// Display the page that is configured as the default page for this profile and location.
		try
		{
			if (false == startTab.empty())
			{
				showPageByName(startTab);
			}
			else
			{
				LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError,
					"start tab has no value" );
			}
		}
		catch(...)
		{
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << startTab;
			UINT selectedButton = userMsg.showMsgBox(IDS_UW_020007);
		}
	}

} // namespace TA_Base_App
