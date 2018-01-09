
#pragma once

#include "bus/alarm/shared_alarm/src/AlarmDataItem.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include <string>

namespace TA_Base_Bus
{
	struct ConflictingTrains
	{
		int offendingTrain;
		std::string offendingTrainString;
		int owningTrain;
		std::string owningTrainString;
	};

	class PreConflictAckDlgModel
		: public TA_Base_Bus::AbstractPModel
	{
	public: //Methods
		PreConflictAckDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		~PreConflictAckDlgModel();

		void deactivateTrain(const unsigned int trainID);
		
		ConflictingTrains GetAlarmParameters(TA_Base_Bus::AlarmDataItem* details);
		ConflictingTrains decodeParameterString(std::string parameterString);
	private: // Methods
	private: // Variables
		
	};

}