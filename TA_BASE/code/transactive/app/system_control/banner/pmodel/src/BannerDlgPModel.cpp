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

#include "app/system_control/banner/pmodel/src/BannerDlgPModel.h"
#include "app/system_control/banner/pmodel/src/BannerPModelConst.h"

namespace TA_Base_App
{


	BannerDlgPModel::BannerDlgPModel(int xPos, int yPos, int width, int height, TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
		, m_pBannerDlgLogic(NULL)
	{
		m_pBannerDlgLogic = new BannerDlgBusLogic(xPos, yPos, width, height);
		m_pBannerDlgLogic->registerObserver(this); 
	}

	BannerDlgPModel::~BannerDlgPModel(void)
	{
		if (NULL != m_pBannerDlgLogic)
		{
			m_pBannerDlgLogic->unregisterObserver(this);
			delete m_pBannerDlgLogic;
			m_pBannerDlgLogic;
		}
	}

	void BannerDlgPModel::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		if (_stricmp(updateType, PMODEL_START_PAGE_UPDATE) == 0)
		{
			notifyViews(updateType, args);
		}
	}

	void BannerDlgPModel::setBannerPage()
	{
		m_pBannerDlgLogic->setBannerPage();
	}
}