#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

#include "core/utilities/src/DebugUtil.h"

namespace TA_Base_App
{
    class ProcessParamsDlgModel :
        public TA_Base_Bus::AbstractPModel
    {
    public:
        ProcessParamsDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
        virtual ~ProcessParamsDlgModel(void);
    public:
        void setProcesses(const std::vector<std::string> & processes)
        {
            m_processes.assign(processes.begin(), processes.end());
        }

		/**
		* setProcessRunParam
		*
		* Sets a runtime parameter for the specified process.
		*
		* @param processId  The entity name of the process.
		*        param      The runparam to set.
		*
		* @exception SystemControllerException  An error occurred communicating
		*                                       with the System Controller.
		*/
		void setProcessRunParam(const std::string& processId, 
			const TA_Base_Core::RunParam& param);

		/**
		* getProcessDebugLevel
		*
		* Returns the debug level for the specified process.
		*
		* @return The debug level as a string.
		*
		* @param processId  The entity name of the process.
		*/
		std::string getProcessDebugLevel(const std::string& processId);
		void ProcessParamsDlgModel::changeProcessParams( const char * debugLevel );
		TA_Base_Core::DebugUtil::EDebugLevel getCurrentDebugLevel( );
	public:
        std::vector<std::string> m_processes;

    };

}