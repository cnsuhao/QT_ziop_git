#include "OptionsDlgModel.h"
#include "AppServiceCenter.h"

namespace TA_Base_App
{

    OptionsDlgModel::OptionsDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
        : TA_Base_Bus::AbstractPModel(appController, pmodelName)
    {
    }

    OptionsDlgModel::~OptionsDlgModel(void)
    {
    }

	void OptionsDlgModel::setRefreshRate( unsigned int newRate )
	{
		AppServiceCenter::getInstance().setRefreshRate(newRate);
	}

	std::vector<int> OptionsDlgModel::getRefreshIntervals()
	{
		return AppServiceCenter::getInstance().getRefreshIntervals();
	}

	unsigned int OptionsDlgModel::getCurrentRefreshRate()
	{
		return AppServiceCenter::getInstance().getCurrentRefreshRate();
	}
}