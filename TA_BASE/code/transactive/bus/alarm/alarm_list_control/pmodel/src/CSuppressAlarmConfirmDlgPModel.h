#pragma once


#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"


namespace TA_Base_Bus
{
	class CSuppressAlarmConfirmDlgPModel
		: public AbstractPModel
	{
	public:
		CSuppressAlarmConfirmDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		~CSuppressAlarmConfirmDlgPModel();
		
		//ITransActiveView interface
		virtual std::string  getMyPModelName(void) { return ""; };
		virtual void setupPModel(void);

		void setConfirmationText(std::string confirmText);
		std::string getConfirmationText();

	private:
		std::string m_confirmationText;
	};

}