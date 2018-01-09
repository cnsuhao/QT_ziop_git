/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/system_control/banner/pmodel/src/BannerDlgPModel.h $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#ifndef __BANNER_DIALOG_BUSINESS_LOGIC_H_INCLUDED__
#define __BANNER_DIALOG_BUSINESS_LOGIC_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/system_control/banner/pmodel/src/StartTabEventAgrs.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "core/utilities/src/RunParams.h"



namespace TA_Base_App
{
	class BannerDlgBusLogic
		: public TA_Base_Bus::AbstractStateChangeSubject<TA_Base_Bus::IStateChangeObserver>
		, public TA_Base_Core::RunParamUser
	{
	public: //Methods
		/*
		* Constructor
		*/
		BannerDlgBusLogic(int xPos, int yPos, int width, int height);
		/*
		* Destructor
		*/
		~BannerDlgBusLogic(void);

		virtual void onRunParamChange(const std::string& name, const std::string& value);
	
		void setBannerPage();

		int getXPos() { return m_xPos; };
		int getYPos() { return m_yPos; };
		int getWidth() { return m_width; };
		int getHeight() { return m_height; };
		std::string getStartTab() { return m_startTab; };

	private: //Methods
		// Copy constructor and operator= made private to prevent them being used.
		BannerDlgBusLogic(const BannerDlgBusLogic&);
		BannerDlgBusLogic& operator=(const BannerDlgBusLogic&);

	private: //Variables
		std::string m_sessionId;

		int m_xPos;
		int m_yPos;
		int m_width;
		int m_height;

		// The tab that is initially selected
		std::string m_startTab;

		unsigned long m_currentProfileId;
	};


} //TA_Base_App


#endif //__BANNER_DIALOG_BUSINESS_LOGIC_H_INCLUDED__