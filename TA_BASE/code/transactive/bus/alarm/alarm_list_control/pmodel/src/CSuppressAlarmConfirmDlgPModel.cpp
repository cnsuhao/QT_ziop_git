#include "CSuppressAlarmConfirmDlgPModel.h"


namespace TA_Base_Bus
{
	CSuppressAlarmConfirmDlgPModel::CSuppressAlarmConfirmDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
	{
	}

	CSuppressAlarmConfirmDlgPModel::~CSuppressAlarmConfirmDlgPModel()
	{
	}

	void CSuppressAlarmConfirmDlgPModel::setupPModel(void)
	{

	}

	void CSuppressAlarmConfirmDlgPModel::setConfirmationText(std::string confirmText)
	{
		m_confirmationText = confirmText;
	}

	std::string CSuppressAlarmConfirmDlgPModel::getConfirmationText()
	{
		return m_confirmationText;
	}
}