#pragma  once
#include "bus\scada\common_library\src\CommonDefs.h"

namespace TA_Base_App
{
	enum EquipmentMsgType
	{
		Unknown,
		EntityUpdate,
		PermissionUpdate
	};


	struct EquipmentMessage
	{
		EquipmentMsgType msgType;
		unsigned long	 entityKey;
		TA_Base_Bus::ScadaEntityUpdateType	updateType;
	};
};