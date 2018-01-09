/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP/TA_BASE/transactive/app/event/event_viewer/src/FilterCriteria.cpp $
  * @author:  huirong.luo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2010/09/15 8:54:40 $
  * Last modified by: xiangmei.lu
  * 
  * FilterCriteria is the filter criteria for each display task( either real time mode or historical mode) 
  * in EventViewer. It will be used for filtering events in searching or refreshing.
  *
  */


//#include "atltime.h"
#include "FilterCriteria.h"
#include "ConstantItems.h"



namespace TA_Base_App{

	void ISCSDateTime::addSec(short int sec){
		if(sec>59){
			sec = sec%60;
		}
		/*CTime time(Year,Month,Day,Hour,Minute,Second);
		CTimeSpan timespan (0,0,0,sec);
		time = time + timespan;
		Year = time.GetYear();
		Month = time.GetMonth();
		Day = time.GetDay();
		Hour = time.GetHour();
		Minute = time.GetMinute();
		Second = time.GetSecond();*/

		struct tm * timeinfo;
		timeinfo->tm_year = Year - 1900;
		timeinfo->tm_mday = Day;
		timeinfo->tm_mon = Month-1;
		timeinfo->tm_hour = Hour;
		timeinfo->tm_min = Minute;
		timeinfo->tm_sec = Second;
		timeinfo->tm_isdst=0;

		timeinfo->tm_sec += sec;
		mktime(timeinfo);

		Year = timeinfo->tm_year + 1900;
		Month = timeinfo->tm_mon +1;
		Day = timeinfo->tm_mday;
		Hour = timeinfo->tm_hour;
		Minute = timeinfo->tm_min;
		Second = timeinfo->tm_sec;

	}

	time_t ISCSDateTime::getTime_t(){
		//CTime ct (Year,Month,Day,Hour,Minute,Second);
		//return (time_t) ct.GetTime();

		struct tm * timeinfo;
		time_t rawtime;
		time (&rawtime);
		timeinfo = localtime (&rawtime);
		timeinfo->tm_year = Year - 1900;
		timeinfo->tm_mday = Day;
		timeinfo->tm_mon = Month-1;
		timeinfo->tm_hour = Hour;
		timeinfo->tm_min = Minute;
		timeinfo->tm_sec = Second;
		timeinfo->tm_isdst=0;
		return mktime(timeinfo);
	}

	void ISCSDateTime::setByATSDateTime(const IATSEventsAPI::ATSDateTime& time)
	{

		this->Year = time.Year;
		this->Month = time.Month;
		this->Day = time.Day;
		this->Hour = time.Hour;
		this->Minute = time.Minute;
		this->Second = time.Second;
		this->Nanosecond = time.Nanosecond;
	}

	bool ISCSDateTime::equalTo(ISCSDateTime & time)
	{	
		if( this->Year == time.Year && this->Month==time.Month && this->Day == time.Day
			&& this->Hour == time.Hour && this->Minute == time.Minute && this->Second == time.Second
			&& this->Nanosecond == time.Nanosecond)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	ISCSDateTime ::ISCSDateTime(const ISCSDateTime & time) 
	{
		this->Year = time.Year;
		this->Month = time.Month;
		this->Day = time.Day;
		this->Hour = time.Hour;
		this->Minute = time.Minute;
		this->Second = time.Second;
		this->Nanosecond = time.Nanosecond;
	}

	ISCSDateTime & ISCSDateTime::operator =(const ISCSDateTime& time)
	{
		this->Year = time.Year;
		this->Month = time.Month;
		this->Day = time.Day;
		this->Hour = time.Hour;
		this->Minute = time.Minute;
		this->Second = time.Second;
		this->Nanosecond = time.Nanosecond;

		return *this;
	}

	bool FilterCriteria::equalTo(FilterCriteria filterCri){

		if( this->EndTimePeriodDefined != filterCri.EndTimePeriodDefined
			|| this->StartTimePeriodDefined != filterCri.StartTimePeriodDefined
			|| !this->EndTimePeriod.equalTo(filterCri.EndTimePeriod)
			|| !this->StartTimePeriod.equalTo(filterCri.StartTimePeriod)
			|| this->eventLevel != filterCri.eventLevel
			|| this->pkeyBoundary_ATS.compare( filterCri.pkeyBoundary_ATS) != 0
			|| this->pkeyBoundary_ISCS != filterCri.pkeyBoundary_ISCS
			|| this->searchDirection != filterCri.searchDirection
			|| this->sortColumn != filterCri.sortColumn
			|| this->sortOrder != filterCri.sortOrder
			|| this->value.compare(filterCri.value) != 0 
			|| this->avalancheHeadID.compare(filterCri.avalancheHeadID) != 0)
		{
			return false;
		}

		// compare list<string>
		if( !strListEqual(this->AlarmIDEqualOr, filterCri.AlarmIDEqualOr) 
			|| !strListEqual(this->Asset, filterCri.Asset)
			|| ! strListEqual(this->Description, filterCri.Description)
			|| ! strListEqual(this->LocationEqualOr, filterCri.LocationEqualOr)
			|| ! strListEqual(this->OperatorEqualOr, filterCri.OperatorEqualOr)
			|| ! strListEqual(this->SubsystemNameEqualOr, filterCri.SubsystemNameEqualOr)
			|| ! strListEqual(this->SystemNameEqualOr, filterCri.SystemNameEqualOr)
			|| ! strListEqual(this->EventTypeEqualOr, filterCri.EventTypeEqualOr)
			|| ! strListEqual(this->TMTypeEqualOr, filterCri.TMTypeEqualOr))
		{
			return false;
		}

		// compare list <unsigned long> , vector<int>, NumericList(vector<unsigned long>)
		if( this->AlarmSeverityEqualOr != filterCri.AlarmSeverityEqualOr 
			|| this->severityIds != filterCri.severityIds
			|| this->AlarmTypeEqualOr !=filterCri.AlarmTypeEqualOr 
			|| this->alarmTypes !=  filterCri.alarmTypes
			|| this->appSubsystemIds != filterCri.appSubsystemIds
			|| this->eventTypeIds != filterCri.eventTypeIds
			|| this->locationIds != filterCri.locationIds
			|| this->operatorIds != filterCri.operatorIds
			|| this->physSubsystemIds != filterCri.physSubsystemIds
			|| this->systemIds != filterCri.systemIds
			|| this->profiles != filterCri.profiles)
		{
			return false;
		}

		return true;
	}

	void FilterCriteria::setPrevious1dayForStartTime()
	{
		StartTimePeriodDefined = true;
		//CTime currentTime = CTime::GetCurrentTime();
		//CTime fromTime = CTime( currentTime.GetTime() - EventViewerConstants::TWENTY_FOUR_HOURS );
		time_t currentTime;
		time (&currentTime);
		currentTime = currentTime -  EventViewerConstants::TWENTY_FOUR_HOURS;
		struct tm * timeinfo;
		timeinfo = localtime (&currentTime);
		StartTimePeriod = ISCSDateTime (timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		//EndTimePeriod = ISCSDateTime(currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond());
		//StartTimePeriod = ISCSDateTime (fromTime.GetYear(), fromTime.GetMonth(), fromTime.GetDay(), fromTime.GetHour(), fromTime.GetMinute(), fromTime.GetSecond());

	}

	bool FilterCriteria::strListEqual( std::list<string> & list1, std::list<string> & list2 )
	{
		if( list1.size() != list2.size())
		{
			return false;
		}
		else
		{
			std::list<string>::iterator itr1 = list1.begin();
			std::list<string>::iterator itr2 = list2.begin();
			while(itr1!=list1.end())
			{
				if( (*itr1).compare(*itr2) != 0)
				{
					return false;
				}else
				{
					itr1++;
					itr2++;
				}
			}
			return true;
		}
	}

	FilterCriteria ::FilterCriteria(const FilterCriteria & f) {
		StartTimePeriodDefined = f.StartTimePeriodDefined;

		StartTimePeriod = f.StartTimePeriod;

		EndTimePeriodDefined = f.EndTimePeriodDefined;

		EndTimePeriod = f.EndTimePeriod; 

		list<unsigned long>::const_iterator cit;

		list<string>::const_iterator cit2;
		for ( cit2 = f.Asset.begin(); cit2!=f.Asset.end(); ++cit2){
			Asset.push_back((*cit2).c_str());
		}

		for ( cit2 = f.OperatorEqualOr.begin(); cit2!=f.OperatorEqualOr.end(); ++cit2){
			OperatorEqualOr.push_back((*cit2).c_str());
		}


		for ( cit2 = f.LocationEqualOr.begin(); cit2!=f.LocationEqualOr.end(); ++cit2){
			LocationEqualOr.push_back((*cit2).c_str());
		}

		value = f.value.c_str();


		for ( cit2 = f.SystemNameEqualOr.begin(); cit2!=f.SystemNameEqualOr.end(); ++cit2){
			SystemNameEqualOr.push_back((*cit2).c_str());
		}

		for ( cit2 = f.EventTypeEqualOr.begin(); cit2!=f.EventTypeEqualOr.end(); ++cit2){
			EventTypeEqualOr.push_back((*cit2).c_str());
		}

		for ( cit2 = f.SubsystemNameEqualOr.begin(); cit2!=f.SubsystemNameEqualOr.end(); ++cit2){
			SubsystemNameEqualOr.push_back((*cit2).c_str());
		}


		for ( cit2 = f.AlarmIDEqualOr.begin(); cit2!=f.AlarmIDEqualOr.end(); ++cit2){
			AlarmIDEqualOr.push_back((*cit2).c_str());
		}

		for ( cit = f.AlarmTypeEqualOr.begin(); cit!=f.AlarmTypeEqualOr.end(); ++cit){
			AlarmTypeEqualOr.push_back(*cit);
		}

		for ( cit2 = f.TMTypeEqualOr.begin(); cit2!=f.TMTypeEqualOr.end(); ++cit2){
			TMTypeEqualOr.push_back((*cit2).c_str());
		}

		avalancheHeadID = f.avalancheHeadID.c_str();

		for ( cit = f.AlarmSeverityEqualOr.begin(); cit!=f.AlarmSeverityEqualOr.end(); ++cit){
			AlarmSeverityEqualOr.push_back(*cit);
		}

		for ( cit2 = f.Description.begin(); cit2!=f.Description.end(); ++cit2){
			Description.push_back((*cit2).c_str());
		}

		std::vector<unsigned long>::const_iterator N_itr;
		for ( N_itr = f.systemIds.begin(); N_itr!=f.systemIds.end(); ++N_itr){
			systemIds.push_back(*N_itr);
		}

		for ( N_itr = f.operatorIds.begin(); N_itr!=f.operatorIds.end(); ++N_itr){
			operatorIds.push_back(*N_itr);
		}

		std::vector<int>::const_iterator I_itr;
		for ( I_itr = f.severityIds.begin(); I_itr!=f.severityIds.end(); ++I_itr){
			severityIds.push_back(*I_itr);
		}

		for ( N_itr = f.appSubsystemIds.begin(); N_itr!=f.appSubsystemIds.end(); ++N_itr){
			appSubsystemIds.push_back(*N_itr);
		}

		for ( N_itr = f.physSubsystemIds.begin(); N_itr!=f.physSubsystemIds.end(); ++N_itr){
			physSubsystemIds.push_back(*N_itr);
		}

		for ( N_itr = f.locationIds.begin(); N_itr!=f.locationIds.end(); ++N_itr){
			locationIds.push_back(*N_itr);
		}
		for ( N_itr = f.alarmTypes.begin(); N_itr!=f.alarmTypes.end(); ++N_itr){
			alarmTypes.push_back(*N_itr);
		}
		for ( N_itr = f.eventTypeIds.begin(); N_itr!=f.eventTypeIds.end(); ++N_itr){
			eventTypeIds.push_back(*N_itr);
		}
		for( N_itr = f.profiles.begin(); N_itr!=f.profiles.end(); ++ N_itr){
			profiles.push_back(*N_itr);
		}

		sortColumn = f.sortColumn;
		sortOrder = f.sortOrder;
		eventLevel = f.eventLevel;

		searchDirection = f.searchDirection;
		pkeyBoundary_ATS = f.pkeyBoundary_ATS;
		pkeyBoundary_ISCS = f.pkeyBoundary_ISCS;

	}
	FilterCriteria::FilterCriteria(){

		EndTimePeriodDefined = false;
		StartTimePeriodDefined = false;

		searchDirection = TA_Base_App::Backward;

		pkeyBoundary_ISCS = 0;
		pkeyBoundary_ATS = "";

	}
	FilterCriteria::~FilterCriteria(){
		eventTypeIds.clear();
		Asset.clear();
		OperatorEqualOr.clear();
		LocationEqualOr.clear();
		SystemNameEqualOr.clear();
		SubsystemNameEqualOr.clear();
		EventTypeEqualOr.clear();
		AlarmIDEqualOr.clear();
		AlarmTypeEqualOr.clear();
		TMTypeEqualOr.clear();
		AlarmSeverityEqualOr.clear();
		Description.clear();
	}

	FilterCriteria & FilterCriteria::operator =(const FilterCriteria& f){
		StartTimePeriodDefined = f.StartTimePeriodDefined;

		StartTimePeriod = f.StartTimePeriod;

		EndTimePeriodDefined = f.EndTimePeriodDefined;

		EndTimePeriod = f.EndTimePeriod; 

		list<unsigned long>::const_iterator cit;

		list<string>::const_iterator cit2;
		Asset.clear();
		for ( cit2 = f.Asset.begin(); cit2!=f.Asset.end(); ++cit2){
			Asset.push_back((*cit2).c_str());
		}

		OperatorEqualOr.clear();
		for ( cit2 = f.OperatorEqualOr.begin(); cit2!=f.OperatorEqualOr.end(); ++cit2){
			OperatorEqualOr.push_back((*cit2).c_str());
		}

		LocationEqualOr.clear();
		for ( cit2 = f.LocationEqualOr.begin(); cit2!=f.LocationEqualOr.end(); ++cit2){
			LocationEqualOr.push_back((*cit2).c_str());
		}

		value = f.value.c_str();


		SystemNameEqualOr.clear();
		for ( cit2 = f.SystemNameEqualOr.begin(); cit2!=f.SystemNameEqualOr.end(); ++cit2){
			SystemNameEqualOr.push_back((*cit2).c_str());
		}

		SubsystemNameEqualOr.clear();
		for ( cit2 = f.SubsystemNameEqualOr.begin(); cit2!=f.SubsystemNameEqualOr.end(); ++cit2){
			SubsystemNameEqualOr.push_back((*cit2).c_str());
		}

		EventTypeEqualOr.clear();
		for ( cit2 = f.EventTypeEqualOr.begin(); cit2!=f.EventTypeEqualOr.end(); ++cit2){
			EventTypeEqualOr.push_back((*cit2).c_str());
		}

		AlarmIDEqualOr.clear();
		for ( cit2 = f.AlarmIDEqualOr.begin(); cit2!=f.AlarmIDEqualOr.end(); ++cit2){
			AlarmIDEqualOr.push_back((*cit2).c_str());
		}

		AlarmTypeEqualOr.clear();
		for ( cit = f.AlarmTypeEqualOr.begin(); cit!=f.AlarmTypeEqualOr.end(); ++cit){
			AlarmTypeEqualOr.push_back(*cit);
		}

		TMTypeEqualOr.clear();
		for ( cit2 = f.TMTypeEqualOr.begin(); cit2!=f.TMTypeEqualOr.end(); ++cit2){
			TMTypeEqualOr.push_back((*cit2).c_str());
		}

		avalancheHeadID = f.avalancheHeadID.c_str();

		AlarmSeverityEqualOr.clear();
		for ( cit = f.AlarmSeverityEqualOr.begin(); cit!=f.AlarmSeverityEqualOr.end(); ++cit){
			AlarmSeverityEqualOr.push_back(*cit);
		}

		Description.clear();
		for ( cit2 = f.Description.begin(); cit2!=f.Description.end(); ++cit2){
			Description.push_back((*cit2).c_str());
		}

		
		NumericList::const_iterator N_itr;
		systemIds.clear();
		for ( N_itr = f.systemIds.begin(); N_itr!=f.systemIds.end(); ++N_itr){
			systemIds.push_back(*N_itr);
		}
	
		operatorIds.clear();
		for ( N_itr = f.operatorIds.begin(); N_itr!=f.operatorIds.end(); ++N_itr){
			operatorIds.push_back(*N_itr);
		}

		severityIds.clear();
		std::vector<int>::const_iterator I_itr;
		for ( I_itr = f.severityIds.begin(); I_itr!=f.severityIds.end(); ++I_itr){
			severityIds.push_back(*I_itr);
		}

		appSubsystemIds.clear();
		for ( N_itr = f.appSubsystemIds.begin(); N_itr!=f.appSubsystemIds.end(); ++N_itr){
			appSubsystemIds.push_back(*N_itr);
		}

		physSubsystemIds.clear();
		for ( N_itr = f.physSubsystemIds.begin(); N_itr!=f.physSubsystemIds.end(); ++N_itr){
			physSubsystemIds.push_back(*N_itr);
		}

		locationIds.clear();
		for ( N_itr = f.locationIds.begin(); N_itr!=f.locationIds.end(); ++N_itr){
			locationIds.push_back(*N_itr);
		}

		eventTypeIds.clear();
		for ( N_itr = f.eventTypeIds.begin(); N_itr!=f.eventTypeIds.end(); ++N_itr){
			eventTypeIds.push_back(*N_itr);
		}

		alarmTypes.clear();
		for ( N_itr = f.alarmTypes.begin(); N_itr!=f.alarmTypes.end(); ++N_itr){
			alarmTypes.push_back(*N_itr);
		}

		profiles.clear();
		for ( N_itr = f.profiles.begin(); N_itr!=f.profiles.end(); ++N_itr){
			profiles.push_back(*N_itr);
		}


		sortColumn = f.sortColumn;
		sortOrder = f.sortOrder;
		eventLevel = f.eventLevel;

		searchDirection = f.searchDirection;
		pkeyBoundary_ATS = f.pkeyBoundary_ATS;
		pkeyBoundary_ISCS = f.pkeyBoundary_ISCS;

		return *this;
	}
};

