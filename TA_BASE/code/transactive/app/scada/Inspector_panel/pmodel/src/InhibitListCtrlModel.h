//#include "EquipmentProxy.h"
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorpanelAppController.h"
#include "AbstractEquipmentPModel.h"

namespace TA_Base_App
{
	
	struct InhibitListCtrlItem
	{
		std::string description;
		std::string type;
		bool		isWritable;
		bool		isCtrlInhibit;
		bool		isScanInhibit;
		bool		isAlarmInhibit;
		bool		isOverrideInhibit;
		bool		isMmsInhibit;
		std::string	valueString;
	public:
		InhibitListCtrlItem();
	};

	class InhibitListCtrlModel:public AbstractEquipmentPModel
	{
	public:
		InhibitListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~InhibitListCtrlModel(void);
	public:
		InhibitItem					getInhibitListCtrlItem(unsigned int iPoint);
		std::vector<InhibitItem>	getAllInhibitDisplayItem();

		/*bool	isChildScanInhibited(unsigned int iPoint);
		bool	isChildMmsAlarmInhibited(unsigned int iPoint);
		bool	isChildAlarmInhibited(unsigned int iPoint);
		void	setInputInhibitStatus(unsigned int iPoint,EInputInhibitState inhibit, const std::string& sessionId);
		void	setOutputInhibitStatus(unsigned int iPoint,EOutputInhibitState inhibit,const std::string& sessionId);*/
		bool				isWritableDp(unsigned int iPoint);
		std::string			getDataPointDescription(unsigned int iPoint);
		bool				isOkToSetScanInhibit(unsigned int iPoint, bool bRequestScanInhibit);
		bool				isOkToSetAlarmInhibit(unsigned int iPoint, bool bRequestAlarmInhibit);
		bool				isOkToSetControlInhibit(unsigned int iPoint, bool bRequestControlInhibit);
		bool				isOkToSetMmsInhibit(unsigned int iPoint, bool bRequestMmsInhibit);

		bool				setScanInhibit(unsigned int iPoint, bool bRequestScanInhibit);
		bool				setAlarmInhibit(unsigned int iPoint, bool bRequestAlarmInhibit);
		bool				setControlInhibit(unsigned int iPoint, bool bRequestControlInhibit);
		bool				setMmsInhibit(unsigned int iPoint, bool bRequestMmsInhibit);
		
		bool				isScanInhibitPermit(unsigned int iPoint);
		bool				isAlarmInhibitPermit(unsigned int iPoint);
		bool				isMmsAlarmInhibitPermit(unsigned int iPoint);
		bool				isControlInhibitPermit(unsigned int iPoint);

		bool				isScanInhibitColumnEnable(unsigned int iPoint);
		bool				isAlarmInhibitColumnEnable(unsigned int iPoint);
		bool				isMmsInhibitColumnEnable(unsigned int iPoint);
		bool				isControlInhibitColumnEnable(unsigned int iPoint);
		void				submitAuditMessage(const std::string& inhibitType, const std::string& addRemove, 
												const std::string& dataPoint, unsigned long entityKey);
		//void				postStatusLine(const std::string& statusLine);
	protected:
		virtual void		onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType);
		virtual void		onDataNodePorxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType);

	private:
	};
}