#pragma once

#include "core/data_access_interface/plans/src/AlarmPlanAssocAccessFactory.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"


namespace TA_Base_Bus
{
	class PlanAssociationDlgPModel
		: public AbstractPModel
	{
	
	public:
		PlanAssociationDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		~PlanAssociationDlgPModel();

		/**
		* getSelectedPlanPath
		*
		* This will return the full path of plan the user selected. It will be set after OnOK has been called
		*
		* @return std::string - The path of the selected plan
		*/
		std::string getSelectedPlanPath() const
		{
			return m_selectedPlanPath;
		}

		void setSelectedPlanPath(const std::string& planPath) { m_selectedPlanPath = planPath;  }
	
		void setPlanDetailList(TA_Base_Core::PlanDetailsList& plans);

		TA_Base_Core::PlanDetailsList& getPlanDetailList();

	private:
		TA_Base_Core::PlanDetailsList m_plans;
		std::string m_selectedPlanPath;
	};

}