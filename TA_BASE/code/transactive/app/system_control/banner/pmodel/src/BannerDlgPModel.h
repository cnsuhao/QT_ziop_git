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

#ifndef __BANNER_DIALOG_MODEL_H_INCLUDED__
#define __BANNER_DIALOG_MODEL_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/system_control/banner/pmodel/src/BannerDlgBusLogic.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "core/utilities/src/RunParams.h"


namespace TA_Base_App
{
	class BannerDlgPModel 
		: public TA_Base_Bus::AbstractPModel
	{
	public:
		/*
		* Constructor
		*/
		BannerDlgPModel(int xPos, int yPos, int width, int height, TA_Base_Bus::IApplicationController* appController, const char * pmodelName);

		/*
		* Destructor
		*/
		~BannerDlgPModel(void);

		void setBannerPage();

		int getXPos() { return m_pBannerDlgLogic->getXPos(); };
		int getYPos() { return m_pBannerDlgLogic->getYPos(); };
		int getWidth() { return m_pBannerDlgLogic->getWidth(); };
		int getHeight() { return m_pBannerDlgLogic->getHeight(); };
		std::string getStartTab() { return m_pBannerDlgLogic->getStartTab(); };

	protected:
		void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);

	private: //Methods
		// Copy constructor and operator= made private to prevent them being used.
 		BannerDlgPModel(const BannerDlgPModel&);
 		BannerDlgPModel& operator=(const BannerDlgPModel&);

	private: //Variables
		
		BannerDlgBusLogic* m_pBannerDlgLogic;
	};	

}

#endif //__BANNER_DIALOG_MODEL_H_INCLUDED__

