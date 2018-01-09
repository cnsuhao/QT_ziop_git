
#ifndef __BANNER_VIEW_FACTORY_H_INCLUDED__
#define __BANNER_VIEW_FACTORY_H_INCLUDED__

#include "bus/generic_gui_view/src/TransActiveWinApp.h"


namespace TA_Base_App
{

	class BannerViewFactory : public TA_Base_Bus::IViewFactory
	{

	public:

		BannerViewFactory(TA_Base_Bus::ITransActiveApp* winApp);
		virtual ~BannerViewFactory(){};
		/** get the view based on specific name
		*
		*
		* @param viewType
		* @param viewID
		* @return ITransActiveView *
		*/
		TA_Base_Bus::ITransActiveView * BannerViewFactory::createView(const char * viewType);

	private:
		TA_Base_Bus::ITransActiveApp* m_winApp;

	};

} // namespace TA_Base_App

#endif //__BANNER_VIEW_FACTORY_H_INCLUDED__