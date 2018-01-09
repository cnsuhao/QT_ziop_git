#include "PreConflictAckDlgModel.h"

#include "bus/alarm/alarm_list_control/pmodel/src/TrainAgentAccess.h"

#include "core/alarm/src/AlarmConfig.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"
#include "core/data_access_interface/entity_access/src/TrainAgentEntityData.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"

#include "boost/tokenizer.hpp"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"

namespace TA_Base_Bus
{
	PreConflictAckDlgModel::PreConflictAckDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
	{

	}

	PreConflictAckDlgModel::~PreConflictAckDlgModel()
	{
	}

	void PreConflictAckDlgModel::deactivateTrain(const unsigned int trainID)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "trainToDeactivate =%d", trainID);//CL-20990++ TestLog
		// get the session id from the runparams
		std::string session = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		try
		{
			// call setCarrierOff on the train agent (use TrainCctvAccess to get to the train agent)
			CORBA_CALL(TA_Base_Bus::TrainAgentAccess::getInstance().getTrainCctvAgent(), setCarrierOff, (trainID, session.c_str()));//limin
		}
		// catch any exceptions and display the error to the operator
		catch (TA_Base_Core::TransactiveException& e)
		{
			LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "deactivateTrain: Caught TransactiveException : %s", e.what());
			throw e;
		}
		catch (TA_Base_Core::AccessDeniedException& ae)
		{
			LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "deactivateTrain: Caught AccessDeniedException");
			throw ae;
		}
		catch (...)
		{
			LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "deactivateTrain: Caught unknown exception");
			throw;
		}
	}

	ConflictingTrains PreConflictAckDlgModel::decodeParameterString(std::string parameterString)
	{
		//paramter string in the form 
		//"OffendingTrain:x,OwningTrain:y,"
		//where x and y are numbers

		ConflictingTrains trains;

		std::vector<std::string> parts;

		typedef boost::tokenizer< boost::char_separator<char> > tokenizer;

		boost::char_separator<char> sep(",");
		tokenizer tokens(parameterString, sep);

		for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
		{
			boost::char_separator<char> sep2(":");
			tokenizer tokens2((*tok_iter), sep2);

			tokenizer::iterator tok2_iter = tokens2.begin();

			if (*tok2_iter == "OffendingTrain")
			{
				tok2_iter++;
				if (tok2_iter != tokens2.end())
				{
					trains.offendingTrain = atoi((*tok2_iter).c_str());
					trains.offendingTrainString = (*tok2_iter).c_str();
				}
			}
			else if (*tok2_iter == "OwningTrain")
			{
				tok2_iter++;
				if (tok2_iter != tokens2.end())
				{
					trains.owningTrain = atoi((*tok2_iter).c_str());
					trains.owningTrainString = (*tok2_iter).c_str();
				}
			}
		}
		return trains;
	}

	ConflictingTrains PreConflictAckDlgModel::GetAlarmParameters(TA_Base_Bus::AlarmDataItem* details)
	{
		// Load the message Type data
		ConflictingTrains trains;
		std::string strDescriptionModel =
			TA_Base_Core::AlarmTypeTableAccessor::getInstance().GetAlarmDescriptionModel(details->messageTypeKey);

		std::vector<std::string> subStrings;
		typedef boost::tokenizer< boost::char_separator<char> > tokenizer;

		boost::char_separator<char> sep(" ");
		tokenizer tokens(strDescriptionModel, sep);
		for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); it++)
		{

			subStrings.push_back(*it);
		}

		long lPrePos1(0), lNextPos1(0), lPrePos2(0), lNextPos2(0), lPrePos1_1(0), lPrePos2_2(0);
		std::string strAlarmParameters("");
		std::string strCurrentAlarmDescription = details->alarmDescription;
		std::vector<std::string> strCrrentDescription;
		boost::char_separator<char> Crrent(" ");
		tokenizer Crrenttokens(strCurrentAlarmDescription, Crrent);
		for (tokenizer::iterator itr = Crrenttokens.begin(); itr != Crrenttokens.end(); itr++)
		{
			strCrrentDescription.push_back((*itr));
		}

		for (unsigned int i = 0; i< subStrings.size(); i++)
		{
			if (subStrings[i] != strCrrentDescription[i])
			{
				lPrePos1 = strDescriptionModel.find("]", lNextPos1);
				lPrePos1_1 = strDescriptionModel.find("[", lNextPos1);

				lPrePos2 = strCurrentAlarmDescription.find(strCrrentDescription[i], lNextPos2);
				lPrePos2_2 = strCrrentDescription[i].length() + lPrePos2;

				if (strDescriptionModel.substr(lPrePos1_1 + 1, lPrePos1 - 1 - lPrePos1_1) == "OffendingTrain")
				{
					trains.offendingTrain = atoi((strCurrentAlarmDescription.substr(lPrePos2, lPrePos2_2 - lPrePos2)).c_str());
					trains.offendingTrainString = (strCurrentAlarmDescription.substr(lPrePos2, lPrePos2_2 - lPrePos2)).c_str();

				}
				else if (strDescriptionModel.substr(lPrePos1_1 + 1, lPrePos1 - 1 - lPrePos1_1) == "OwningTrain")
				{
					trains.owningTrain = atoi((strCurrentAlarmDescription.substr(lPrePos2, lPrePos2_2 - lPrePos2)).c_str());
					trains.owningTrainString = (strCurrentAlarmDescription.substr(lPrePos2, lPrePos2_2 - lPrePos2)).c_str();
				}
				lNextPos1 = lPrePos1 + 1;
			}
		}
		return trains;
	}
}

