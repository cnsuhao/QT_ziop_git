#pragma  once

//#include "EquipmentProxy.h"
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorpanelAppController.h"
#include "AbstractEquipmentPModel.h"

namespace TA_Base_App
{
	struct LimitDetaillListCtrlItem
	{
		std::string		typeString;
		bool			isEnable;
		std::string		currentValueString;
		std::string		thresholdString;
		double			thresholdValue;
	};


	class LimitDetaillListCtrlModel :public AbstractEquipmentPModel
	{
	public:
		LimitDetaillListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~LimitDetaillListCtrlModel(void);
	public:
		std::vector<LimitDetailItem>	getLimitDetaillListCtrlItem(unsigned int iPoint);
		std::string						getChildDataPointDescription(UINT iPoint);
		bool							isWritableDp(unsigned int iPoint);
		std::string						getChildEntityName(unsigned int iPoint);
		//void LimitDetaillListCtrlModel::fillThresholdString(LimitDetaillListCtrlItem& item, unsigned int iPoint);
		std::string						alarmTypeToString(TA_Base_Bus::EDataPointAlarms alarmType);

		std::string						getChildUnit(unsigned int iPoint);
		double							getChildLimit(unsigned int iPoint,bool isLow);

		//std::vector<LimitDetaillListCtrlItem> LimitDetaillListCtrlModel::getLimitDetaillListCtrlItem(unsigned int iPoint);

		bool		setAlarmProperties (unsigned int iPoint, const AlarmProperty & newAlarmProperty );
		bool		getAlarmProperty(unsigned int iPoint,const char* alarmType,AlarmProperty& config);
		bool		isAlarmTypeMatchable(unsigned int iPoint, const std::string& alarmType);
	protected:
		void		onDataPointProxyUpdate(unsigned long key, ScadaEntityUpdateType updateType);
		

	private:
		unsigned int					m_iPoint;
	};
}