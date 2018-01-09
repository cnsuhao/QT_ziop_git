
#include "BannerViewFactory.h"
#include "bannerdlg.h"
#include "CommonDef.h"

namespace TA_Base_App
{
	BannerViewFactory::BannerViewFactory(TA_Base_Bus::ITransActiveApp* winApp) 
		: m_winApp(winApp)
	{

	}

	TA_Base_Bus::ITransActiveView * BannerViewFactory::createView(const char * viewType)
	{
		TA_Base_Bus::ITransActiveView * viewRetVal = 0;
		std::string tempViewType = viewType;
		if (tempViewType == ViewType_MainBanner)
		{
			viewRetVal = new BannerDlgImpl(m_winApp);
			viewRetVal->setMainView(true);
		}

		return viewRetVal;
	}



}


