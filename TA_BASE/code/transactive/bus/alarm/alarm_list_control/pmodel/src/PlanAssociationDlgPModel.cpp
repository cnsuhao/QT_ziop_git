#include "PlanAssociationDlgPModel.h"


namespace TA_Base_Bus
{

	PlanAssociationDlgPModel::PlanAssociationDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
	{
	}

	PlanAssociationDlgPModel::~PlanAssociationDlgPModel()
	{
	}

	void PlanAssociationDlgPModel::setPlanDetailList(TA_Base_Core::PlanDetailsList& plans)
	{
		m_plans = plans;
	}

	TA_Base_Core::PlanDetailsList& PlanAssociationDlgPModel::getPlanDetailList()
	{
		return m_plans;
	}
}
