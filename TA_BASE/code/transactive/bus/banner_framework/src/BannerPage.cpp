/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/BannerPage.cpp $
  * @author:  Daniel Hoey
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  * BannerPage is an abstract class that all pages in the banner should inherit
  * from. It inherits from the IBannerPage interface so that the main Banner dialog
  * can use this class to communicate with each page in its own DLL.
  *
  */

#pragma warning(disable:4786)   // Decorated name too long


#include "bus/banner_framework/src/BannerPage.h"
#include "bus/banner_framework/src/BannerFrameworkDialog.h"
#include "bus/banner_framework/src/SingletonHelper.h"

#include "core/utilities/src/IDllHandler.h"

namespace TA_Base_Bus
{
	BannerPage::BannerPage(const std::string& buttonName, std::string viewType, int xPosition, int yPosition, int width, int height, BannerFrameworkDialog* parentDialog)
    :
	TStackedPage(parentDialog->getTransActiveWinApp(), viewType, parentDialog),
    m_xPosition(xPosition),
    m_yPosition(yPosition),
    m_width(width),
    m_height(height),
	m_buttonId(viewType),
    m_buttonName(buttonName),
    m_parentDialog(parentDialog)
    {
        TA_ASSERT( m_parentDialog != NULL, "Parent cannot be null");
        //m_parentDialog->AssertValid();
		Qt::WindowFlags flags = this->windowFlags();
		setWindowFlags(flags | Qt::Widget);
    }


    BannerPage::~BannerPage()
    {
    }


    void BannerPage::setUpSingletons(const TA_Base_Core::IDllHandler& dllHandler, const std::string& entityName, 
                                     const std::string& name)
    {
        SingletonHelper helper;
        helper.initialiseUtilities(dllHandler, entityName, name);
    }


	void BannerPage::OnInitDialog()
    {   
		this->setGeometry(m_xPosition, m_yPosition, m_width, m_height);
        //MoveWindow(m_xPosition, m_yPosition, m_width, m_height);
        //return TRUE;
    }


    void BannerPage::enableButtonFlash()
    {
        if ( NULL != m_parentDialog )
        {
            m_parentDialog->enableButtonFlash(m_buttonId);
        }
    }


    void BannerPage::disableButtonFlash()
    {
        if ( NULL != m_parentDialog )
        {
            m_parentDialog->disableButtonFlash(m_buttonId);
        }
    }


    int BannerPage::getXPosition()
    {
        return m_xPosition;
    }


    int BannerPage::getYPosition()
    {
        return m_yPosition;
    }


    int BannerPage::getWidth()
    {
        return m_width;
    }


    int BannerPage::getHeight()
    {
        return m_height;
    }

} // namespace TA_Base_Bus
