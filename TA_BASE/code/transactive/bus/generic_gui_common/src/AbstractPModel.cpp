
#include "bus/generic_gui_common/src/AbstractPModel.h"


#include "core/exceptions/src/ApplicationException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/DebugUtilMacros.h"

namespace TA_Base_Bus
{

    AbstractPModel::AbstractPModel(IGUIApplicationController* appController, const char * pmodelName)
        : m_appController(appController)
        , m_pmodelName(pmodelName)
	{
	}

	AbstractPModel::~AbstractPModel(void)
	{
	}

    void AbstractPModel::cleanup()
    {
        LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "AbstractPModel::cleanup");

        releaseViewResource();

    }

}