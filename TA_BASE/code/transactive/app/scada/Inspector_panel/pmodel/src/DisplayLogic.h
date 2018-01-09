#pragma  once
#include "Equipment.h"
#include "bus\scada\proxy_library\src\IDataPointProxy.h"
#include "bus\generic_gui_pmod\src\PModelInterface.h"

namespace TA_Base_App
{
	struct InformationItem
	{
		unsigned int    iPoint;
		std::string		description;
		std::string		shortName;
		std::string		valueString;
		std::string		timestamp;
		std::string		qualityString;
	};


	struct ControlItem
	{
		unsigned int	iPoint;
		std::string		description;
		std::string		type;
		std::string		state;
		std::string		newValue;
		std::string		units;
	};


	struct InhibitItem
	{
		unsigned int iPoint;
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
		InhibitItem();
	};

	struct LimitDetailItem
	{
		std::string		typeString;
		bool			isEnable;
		std::string		currentValueString;
		std::string		thresholdString;
		double			thresholdValue;
	};

	struct MeterItem
	{
		unsigned int	iPoint;
		std::string		description;
		std::string		type;
		std::string		currentValue;
	};


	struct OverrideItem
	{
		unsigned int	iPoint;
		std::string		description;
		std::string		type;
		bool			isOverride;
		std::string		overrideValue;
		std::string		currentValue;

		OverrideItem();
	};


	enum InhibitType
	{
		scanInhibit				= 0,
		alarmInhibit			= 1,
		mmsInhibit				= 2,
		controlInhibit			= 3
	};



	class DisplayLogic
	{
	public:
		DisplayLogic(CEquipment*	pEquipment);
		~DisplayLogic();
	public:
		InformationItem					getInfomationDisplayItem(unsigned int iPoint);
		std::vector<InformationItem>	getAllInfomationDisplayItem();

		ControlItem						getControlDisplayItem(unsigned int iPoint);
		std::vector<ControlItem>		getAllControlDisplayItem();

		InhibitItem						getInhibitDisplayItem(unsigned int iPoint);
		std::vector<InhibitItem>		getAllInhibitDisplayItem();

		std::vector<LimitDetailItem> getAllLimitDetailDisplayItem(unsigned int iPoint);

		MeterItem						getMeterItemDisplayItem(unsigned int iPoint);
		std::vector<MeterItem>			getAllMeterDisplayItem();

		OverrideItem					getOverrideDisplayItem(unsigned int iPoint);
		std::vector<OverrideItem>		getAllOverrideDisplayItem();
	public:
		bool							isActionPermitted(unsigned int iPoint, AccessControlActions action);
	public:
		bool							isWritableDp(unsigned int iPoint);
		bool							isNotWritableDp(unsigned int iPoint);
		int								constructControlStateString(std::vector<std::string>& parameters);

		bool							isControlStateInPending(unsigned int iPoint);
		bool							isBoolean(unsigned iPoint);
		bool							isDerived(unsigned iPoint);
		bool							isNumber(unsigned iPoint);
		bool							isText(unsigned iPoint);
		std::vector<std::string>		getAllValues(unsigned int iPoint);
		bool							isOverrideEnabled(unsigned int iPoint);
		bool							isOkToOverride(unsigned int iPoint);
		bool							isOkToRemoveOverride(unsigned int iPoint);
		std::string						getDataPointDescription(unsigned int iPoint);
		int								getDataPointDisplayOrder(unsigned int iPoint);
		
		//for inhibit
		bool							isScanInhibitable(unsigned int iPoint);
		bool							isAlarmInhibitable(unsigned int iPoint);
		bool							isMmsInhibitable(unsigned int iPoint);
		bool							isControlInhibitable(unsigned int iPoint);

		bool							isOkToSetScanInhibit(unsigned int iPoint, bool bRequestScanInhibit);
		bool							isOkToSetAlarmInhibit(unsigned int iPoint, bool bRequestAlarmInhibit);
		bool							isOkToSetControlInhibit(unsigned int iPoint, bool bRequestControlInhibit);
		bool							isOkToSetMmsInhibit(unsigned int iPoint, bool bRequestMmsInhibit);
		//for meter
		bool							checkValueAdjustable(unsigned int iPoint, const std::string& newValue,double& correction,double& adjustedValue);
		double							getChildValueAsDouble(unsigned int iPoint);
		std::string						getChildValueAsString(UINT iPoint);
		std::string						getDataNodeAssetName();
		std::string						getDataNodeDescription();
		std::string						getDataNodeLabel();
		//for limits detail
		std::string						getDataPointName(unsigned int iPoint);
		bool							findAlarmProperty(unsigned int iPoint, const std::string& alarmTypeString,AlarmProperty& config);
		std::string						getDataPointUnit(unsigned int iPoint);
		double							getDataPointLimit(unsigned int iPoint, bool lowLimit);

		//for limits
		std::string						getLimitInfoString(unsigned int iPoint);
		bool							isLimitDp(unsigned int iPoint);
		//for tag
		TA_Base_Bus::ETagState			getDataNodeTaggingState();
		TA_Base_Bus::InhibitState		getDataNodeTagInhibitState();
		std::string						getDataNodePtwId();
		std::string						getDataNodeTagComment();
		unsigned long					getDataNodeKey();
		bool							isInhibitEnable(bool buttonChecked, InhibitType type);
		bool							hasInformationPoint();
		bool							hasControlPoint();
		//for nottes
		std::string						getNotes();
		//for abstract pmodel
		EDataPointStatusColor			getDataPointStatusColor(unsigned int iPoint);
		std::string						getDisplayValueWithUnit(unsigned int iPoint);
		//for inspector pmodel
		bool							haveAllDataPointsConfigured();
		//for regester update 
		void							registerObserver(TA_Base_Bus::IStateChangeObserver* observer, const char * updateType = NULL);
		void							unregisterObserver(TA_Base_Bus::IStateChangeObserver* observer, const char * updateType = NULL);

		//for direct control
		unsigned int					getDirectControlPoint();
		std::string						getDirectControlValue();
	public:
		std::map<unsigned int, bool>	getAllowedToWriteMap();
		bool							getDpAllowedToWrite(unsigned int iPoint);
	public:
		unsigned int getChildDataPointIndexFromEntityKey(unsigned long key);
		std::vector<unsigned long>&		getWriteableIndex();
		std::vector<unsigned long>&		getMeterIndex();
		std::vector<unsigned long>&		getLimitIndex();
	private:
		void fillThresholdString(LimitDetailItem& item, TA_Base_Bus::DataPointProxySmartPtr& datapoint);
		std::string alarmTypeToString(TA_Base_Bus::EDataPointAlarms alarmType);
		std::string getControlStatusString ( TA_Base_Bus::EDataPointControlState state );
		std::string getCurrentValueAsString(DataPointProxySmartPtr& dataPoint);
		
		//std::string addControlStateToStatusLine ( TA_Base_Bus::DataPointProxySmartPtr & datapoint, TA_Base_Bus::EDataPointControlState state );
		int		addControlStateToStatusLine ( TA_Base_Bus::DataPointProxySmartPtr & datapoint, TA_Base_Bus::EDataPointControlState state, std::vector<std::string>&parameters ); //wenching++ (TD14179)
		






	private:
		CEquipment	*m_pEquipment;
	};
}