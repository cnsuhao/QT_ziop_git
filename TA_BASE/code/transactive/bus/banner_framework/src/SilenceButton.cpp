/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/SilenceButton.cpp $
  * @author:  Justin Ebedes
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  * This class represents a silence button that can be added
  * to any banner page.
  *
  */

#include "bus/banner_framework/src/SilenceButton.h"
#include "bus/resources/resource.h"
#include "bus/sound/sound_client/src/SoundManagerProxy.h"

#include "core/exceptions/src/SoundPlayerException.h"
#include "core/utilities/src/DebugUtil.h"
#include <QPalette.h>

using TA_Base_Core::DebugUtil;

namespace TA_Base_Bus
{

    SilenceButton::SilenceButton(const std::string& soundCategory)
    : m_isSilenced(false),
    m_soundCategory(soundCategory),
    m_soundInterface(NULL)
    {
		//this->setFlat(true);
        try
	    {
		    m_soundInterface = new TA_Base_Bus::SoundManagerProxy(this);
	    }
	    catch(TA_Base_Core::SoundPlayerException& ex)
	    {
		    std::string error = "Unable to initialise SoundManager: ";
		    error += ex.what();
		    LOG_EXCEPTION_CATCH(SourceInfo, "SoundPlayerException", error.c_str());
	    }
		m_muteImage = new QIcon(":/res/icons/silence.ico");
		m_unmuteImage = new QIcon(":/res/icons/unsilence.ico");
    }


    SilenceButton::~SilenceButton()
    {
		// Delete the SoundManagerProxy
		if(m_soundInterface != NULL)
		{
			delete m_soundInterface;
			m_soundInterface = NULL;
		}
	}
  

    void SilenceButton::initialise(bool isSilenced)
    {
        m_isSilenced = isSilenced;
	    handleCategorySilenced(m_soundCategory, isSilenced);
    }


    void SilenceButton::toggleState()
    {
	    handleCategorySilenced(m_soundCategory, !m_isSilenced);

	    try
	    {
            if (m_soundInterface != NULL)
            {
		        m_soundInterface->setSilence(m_soundCategory, m_isSilenced);
            }
	    }
	    catch(TA_Base_Core::SoundPlayerException& ex)
	    {
		    std::string error = "Error setting silence state of ";
            error += m_soundCategory;
            error += " sound category: ";
		    error += ex.what();
		    LOG_EXCEPTION_CATCH(SourceInfo, "SoundPlayerException", error.c_str());
	    }
    }


    void SilenceButton::unsilence()
    {
        if (m_isSilenced)
        {
            try
            {
                handleCategorySilenced(m_soundCategory, !m_isSilenced);
                
                if (m_soundInterface != NULL)
                {
                    m_soundInterface->setSilence(m_soundCategory, false);
                }
            }
            catch(TA_Base_Core::SoundPlayerException& ex)
            {
                std::string error = "Error unsilencing ";
                error += m_soundCategory;
                error += " sound category: ";
		        error += ex.what();
		        LOG_EXCEPTION_CATCH(SourceInfo, "SoundPlayerException", error.c_str());
	        }
	    }
    }


    void SilenceButton::handleCategorySilenced(const std::string& category, bool isSilenced)
    {
	    if (category == m_soundCategory)
	    {
		    // Our sound category was silenced or unsilenced.
		    m_isSilenced = isSilenced;
			QIcon* pImage;
		    if (isSilenced)
		    {
				pImage = m_muteImage;
				this->setChecked(true);
		    }
		    else
		    {
				pImage = m_unmuteImage;
				this->setChecked(false);
		    }
			this->setIcon(*pImage);
			//this->setIconSize(pImage->rect().size());
	    }
    }


    void SilenceButton::handleAllSilenced(bool isSilenced)
    {
	    handleCategorySilenced(m_soundCategory, isSilenced);
    }
} // namespace TA_Base_Bus
