/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/FlashIconButton.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  * FlashIconButton.cpp : implementation file
  *
  */


#pragma warning(disable:4786)   // Decorated name too long

#include "bus/banner_framework/src/FlashIconButton.h"
#include "bus/banner_framework/src/BannerFrameworkDialog.h"
#include <QPalette.h>

namespace TA_Base_Bus
{
	FlashIconButton::FlashIconButton(const TA_Base_Bus::BannerIcon& bannerIcon, int buttonId, std::map<int, QPixmap*>& iconCache)
    : m_bFlashing(false),
    m_currentFlashStateOn(false),
    m_flashPeriodMillisec(500),  // default to 1/2 second
    m_buttonId(buttonId),
    m_isChecked(false) //See class comments as to why I am not using the standard CStatic "checked" functionality
	{ 
		this->setFlat(true);
		this->setAutoFillBackground(true);
		
		m_onImage = (iconCache[bannerIcon.onIcon]);
		if (m_onImage->isNull())
		{
			LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "Unable to load bitmap");
		}
            //MAKEINTRESOURCE(bannerIcon.onIcon), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);	

		m_offImage = (iconCache[bannerIcon.offIcon]);
            //MAKEINTRESOURCE(bannerIcon.offIcon), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);

		m_onDownImage = (iconCache[bannerIcon.onDownIcon]);
            //MAKEINTRESOURCE(bannerIcon.onDownIcon), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);

		m_offDownImage = (iconCache[bannerIcon.offDownIcon]);
            //MAKEINTRESOURCE(bannerIcon.offDownIcon), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);

		// Setup Connection
		// Set up a timer so we know when to update the list and make it flash 
		connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
		
    }


    FlashIconButton::~FlashIconButton()
    {
		StopFlashing();
	}

    /////////////////////////////////////////////////////////////////////////////
    // FlashIconButton message handlers


    void FlashIconButton::SetFlashPeriod(unsigned long flashPeriodMillisec)
    {
	    // store the new blinking period
	    m_flashPeriodMillisec = flashPeriodMillisec;
	    
	    // if flashing
	    if (m_bFlashing)
	    {
		    // Stop & re-start flashing, this will force the timer to use
		    // the new flash period
		    StopFlashing();
		    StartFlashing();
	    }
    }


    void FlashIconButton::StartFlashing()
    {
	    if ( false == m_bFlashing ) // if not already flashing
	    {
		    // change the flash state right now... (this prevents delay introduced by the timer period)
		    GetNextFlashState();
			QPixmap* pImage;
			if(m_currentFlashStateOn)
		    {
			    if(m_isChecked)
			    {	
					pImage = m_onDownImage;
			    }
			    else
			    {
					pImage = m_onImage;
			    }
		    }
		    else
		    {
			    if(m_isChecked)
			    {
					pImage = m_offDownImage;
			    }
			    else
			    {
					pImage = m_offImage;
			    }
		    }
		    
			this->setIcon(QIcon(*pImage));
			this->setIconSize(pImage->rect().size());
			
		    // start the flash timer
			m_timer.start(m_flashPeriodMillisec);
		    m_bFlashing = true;
	    }
    }

    void FlashIconButton::StopFlashing()
    {
	    if (m_bFlashing) // if flashing
	    {
		    m_bFlashing = false;

		    // stop the flash timer
			m_timer.stop();

		    m_currentFlashStateOn = false;
			QPixmap* pImage;
		    if(m_isChecked)
		    {
				pImage = m_onDownImage;
		    }
		    else
		    {
				pImage = m_onDownImage;
		    }
			this->setIcon(QIcon(*pImage));
			this->setIconSize(pImage->rect().size());
			
	    }
    }


    bool FlashIconButton::GetNextFlashState()
    {
	    m_currentFlashStateOn = !m_currentFlashStateOn;	    
	    return m_currentFlashStateOn;
    }


    void FlashIconButton::OnTimer() 
    {
	    // if it's our flash timer
		if (m_bFlashing)
		{
			// get the next flash state
			GetNextFlashState();
			// repaint self
			QPixmap* pImage;
			
			if(m_currentFlashStateOn)
			{
				if(m_isChecked)
				{
					pImage = m_onDownImage;
				}
				else
				{
					pImage = m_onImage;
				}
			}
			else
			{
				if(m_isChecked)
				{
					pImage = m_offDownImage;
				}
				else
				{
					pImage = m_offImage;
				}
			}
			this->setIcon(QIcon(*pImage));
			this->setIconSize(pImage->rect().size());
			
		}
    }

    void FlashIconButton::ToggleFlash()
    {
	    // if flashing, stop... if not, start
	    if (m_bFlashing)
	    {
		    StopFlashing();
	    }
	    else
	    {
		    StartFlashing();
	    }
    }


    //See class comments as to why I am not using the standard CStatic "checked" functionality
    bool FlashIconButton::isChecked()
    {
	    return m_isChecked;
    }


    //See class comments as to why I am not using the standard CStatic "checked" functionality
    void FlashIconButton::setCheck(bool checkIt)
    {
	    m_isChecked = checkIt;
		QPixmap* pImage;
		if (m_isChecked)
		{
			pImage = m_offDownImage;
		}
		else
		{
			pImage = m_offImage;
		}
		this->setIcon(QIcon(*pImage));
		this->setIconSize(pImage->rect().size());
    }
}
