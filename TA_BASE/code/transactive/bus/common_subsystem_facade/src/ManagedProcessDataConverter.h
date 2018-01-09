#ifndef MANAGED_PROCESS_DATA_CONVERTER__
#define MANAGED_PROCESS_DATA_CONVERTER__

#include "core/process_management/idl/src/ISystemControllerAdminCorbaDef.h"
#include "core/utilities/src/RunParams.h"

#include <string>
#include <vector>

namespace TA_Base_Bus
{
	// Information about the individual Processes
	struct ManagedProcessData
	{
		std::string         entityName; //processId --> entityName
		unsigned long		started;
		std::string			host;
		TA_Base_Core::EProcessStatus		state;
		TA_Base_Core::EProcessStatus      requestedState;
		TA_Base_Core::EOperationMode		operationMode;
		unsigned short		restarts;
		std::string			debugLevel;
		ManagedProcessData()
			: entityName("")
			, started(0)
			, host("")
			, state(TA_Base_Core::Unstarted)
			, requestedState(TA_Base_Core::Unstarted)
			, operationMode(TA_Base_Core::NotApplicable)
			, restarts(0)
			, debugLevel("")
		{

		}
        
		void clear()
		{
			entityName.clear();
			started = 0;
			host.clear();
			state = TA_Base_Core::Unstarted;
			requestedState = TA_Base_Core::Unstarted;
			operationMode = TA_Base_Core::NotApplicable;
			restarts = 0;
			debugLevel.clear();
		}
	};



	class ManagedProcessDataConverter
	{
	private:
		ManagedProcessDataConverter(void);
		~ManagedProcessDataConverter(void);
	public:
		static void convertProcessData(const TA_Base_Core::ProcessData & processDataCorbaDef, ManagedProcessData & agentdata);
		static void convertProcessDataSeq(const TA_Base_Core::ProcessDataSeq & processDataCorbaDef, std::vector<ManagedProcessData> & agentdata);
        static void convertRunParamSeq(const TA_Base_Core::RunParamSeq & runParamSeqCorbaDef, TA_Base_Core::RunParams::ParamVector & vtRunParam);
	};

}
#endif