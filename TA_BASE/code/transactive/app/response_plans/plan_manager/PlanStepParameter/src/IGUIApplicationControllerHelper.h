#pragma once

namespace TA_Base_Bus
{
	class IGUIApplicationController;
}

namespace TA_Base_App
{
	class IGUIApplicationControllerHelper
	{
	public:
		static TA_Base_Bus::IGUIApplicationController* getAppController();
		IGUIApplicationControllerHelper(void);
		~IGUIApplicationControllerHelper(void);
	};
};
