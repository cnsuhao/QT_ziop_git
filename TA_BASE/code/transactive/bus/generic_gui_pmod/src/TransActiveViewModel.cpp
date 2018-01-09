
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"

#include "core/exceptions/src/ApplicationException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/DebugUtilMacros.h"
#include "bus/generic_gui_pmod/src/AsyncWorkThreadPool.h"
 
namespace TA_Base_Bus
{

    AbstractPModel::AbstractPModel(IApplicationController* appController, const char * pmodelName)
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

	void AbstractPModel::executeWorkAync(TA_Base_Core::IWorkItemPtr workItem)
	{
		 
		TA_Base_Bus::AsyncWorkThreadPool* asyncManager = TA_Base_Bus::AsyncWorkThreadPool::getInstance();		 
		asyncManager->queueWorkItem(workItem);
	}	 
	

}
