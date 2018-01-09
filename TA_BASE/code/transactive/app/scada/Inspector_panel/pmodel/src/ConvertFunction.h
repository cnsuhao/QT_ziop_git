#include "bus\scada\common_library\src\CommonDefs.h"
#include <string>

using namespace TA_Base_Bus;

namespace TA_Base_App
{
	class ConvertFunction
	{
	public:
		static std::string		convertDataPointControlStateToString ( TA_Base_Bus::EDataPointControlState state );
		static std::string		convertAlarmTypeToString(TA_Base_Bus::EDataPointAlarms alarmType);
	};
}