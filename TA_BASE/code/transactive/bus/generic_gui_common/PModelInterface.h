#ifndef PMODEL_INTERFACE_H__
#define PMODEL_INTERFACE_H__

#include "bus/generic_gui_pmod/src/UserInteraction.h"
#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "bus/security/authentication_library/src/SessionInfo.h"
//#include "bus/generic_gui_pmod/src/ItemRepository.h"
#include "bus/generic_gui_pmod/src/CommandGateway.h"


#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/message/public_interfaces/ItaAuditing.h"
#include "core/utilities/src/DebugUtil.h"

#include "boost/shared_ptr.hpp"
#include <vector>
#include <string>
#include <map>

namespace TA_Base_Bus_Test
{
    class AbstractStateChangeSubjectTest;
}

namespace TA_Base_Bus
{
	class ITransActivePModel;
    class IGUIApplicationController;
    class ITransActiveView;

	/** use to store parameter name and value's pointer
	*	
	*/
	typedef std::map<std::string, void *> NameValueParams;


}

#endif
