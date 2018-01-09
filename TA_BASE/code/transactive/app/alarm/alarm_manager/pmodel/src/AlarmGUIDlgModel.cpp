#include "AlarmGUIDlgModel.h"
#include "app\alarm\alarm_manager\pmodel\src\AlarmFilterService.h"

namespace TA_Base_App
{

    AlarmGUIDlgModel::AlarmGUIDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
        : TA_Base_Bus::AbstractPModel(appController, pmodelName)
    {
		FUNCTION_ENTRY("Constructor");
		AlarmManagerService::getInstance().registerObserver(this);

		FUNCTION_EXIT;
    }

    AlarmGUIDlgModel::~AlarmGUIDlgModel(void)
	{
		FUNCTION_ENTRY("Destructor");
		AlarmManagerService::getInstance().unregisterObserver(this);
		FUNCTION_EXIT;
	}

	void AlarmGUIDlgModel::init()
	{
		AlarmManagerService::getInstance().init();
	}

	void AlarmGUIDlgModel::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args/* = NULL*/)
	{
		FUNCTION_ENTRY("notifyUpdate");
		AlarmManagerService::getInstance().notifyObservers(updateType, args);

		FUNCTION_EXIT;
	}
	std::string AlarmGUIDlgModel::getStatusBarData()
	{
		FUNCTION_ENTRY("getStatusBarData");
		FUNCTION_EXIT;
		return AlarmManagerService::getInstance().getStatusBarData();
	}

	void AlarmGUIDlgModel::setStatusBarData(const std::string& value)
	{
		FUNCTION_ENTRY("setStatusBarData");
		AlarmManagerService::getInstance().setStatusBarData(value);
		FUNCTION_EXIT;
	}

	std::list<std::string > AlarmGUIDlgModel::getFilterList()
	{
		FUNCTION_ENTRY("getFilterList");
		FUNCTION_EXIT;
		return AlarmFilterService::getInstance().getFilterNameList();
	}
}
