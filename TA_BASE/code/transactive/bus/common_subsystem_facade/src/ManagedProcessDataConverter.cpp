#include "ManagedProcessDataConverter.h"

namespace TA_Base_Bus
{

	void ManagedProcessDataConverter::convertProcessData( const TA_Base_Core::ProcessData & processDataCorbaDef, ManagedProcessData & processData )
	{
		processData.clear();
		processData.entityName = processDataCorbaDef.ProcessId.in();
		processData.started = processDataCorbaDef.started;
		processData.host = processDataCorbaDef.host.in();
		processData.state = processDataCorbaDef.state;
		processData.requestedState = processDataCorbaDef.requestedState;
		processData.operationMode = processDataCorbaDef.operationMode;
		processData.restarts = processDataCorbaDef.restarts;
		processData.debugLevel = processDataCorbaDef.debugLevel.in();
	}

	void ManagedProcessDataConverter::convertProcessDataSeq( const TA_Base_Core::ProcessDataSeq & processDataCorbaSeq, std::vector<ManagedProcessData> & vtProcessData )
	{
		vtProcessData.clear();
		unsigned long vtSize = processDataCorbaSeq.length();
		ManagedProcessData processData;
		for(unsigned long i=0; i<vtSize; i++)
		{
			processData.clear();
			convertProcessData(processDataCorbaSeq[i], processData);
			vtProcessData.push_back(processData);
		}
	}

    void ManagedProcessDataConverter::convertRunParamSeq( const TA_Base_Core::RunParamSeq & runParamSeqCorbaDef, TA_Base_Core::RunParams::ParamVector & vtRunParam )
    {
        vtRunParam.clear();
        unsigned long vtSize = runParamSeqCorbaDef.length();

        TA_Base_Core::RunParams::ParamNameValue runParamInfo;

        for(unsigned long i=0; i<vtSize; i++)
        {
            runParamInfo.name = runParamSeqCorbaDef[i].name;
            runParamInfo.value = runParamSeqCorbaDef[i].value;
            vtRunParam.push_back(runParamInfo);
        }
        
    }
}