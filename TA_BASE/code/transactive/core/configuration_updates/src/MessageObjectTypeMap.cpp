/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/configuration_updates/src/MessageObjectTypeMap.cpp $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * This class defines the interface that must be implemented
  * by GUIs that want to receive online updates
  */

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif // defined (_MSC_VER)

#include "core/configuration_updates/src/MessageObjectTypeMap.h"
#include "core/message/types/ConfigUpdate_MessageTypes.h"
#include "core/message/types/MessageTypes.h"

namespace TA_Base_Core
{
    MessageObjectTypeMap::MessageObjectTypeMap()	
    {
        // Initialise our map with Object Types and their corresponding
        // string value generated by 9999P01010101-TransActive_CORBA_Messages_Register.xls
        // To add new object type - config message map, just append to the end of this section.
        m_objectTypeKey[ConfigUpdate::ConfigAlarmPlanAssociation.getTypeKey()] = ALARM_PLAN_ASSOCIATION;
        m_objectTypeKey[ConfigUpdate::ConfigAlarmSeverity.getTypeKey()] = ALARM_SEVERITY;
        m_objectTypeKey[ConfigUpdate::ConfigAlarmType.getTypeKey()] = ALARM_TYPE;
        m_objectTypeKey[ConfigUpdate::ConfigApplication.getTypeKey()] = APPLICATION;
        m_objectTypeKey[ConfigUpdate::ConfigEventType.getTypeKey()] = EVENT_TYPE;
        m_objectTypeKey[ConfigUpdate::ConfigInterlocking.getTypeKey()] = INTERLOCKING;
        m_objectTypeKey[ConfigUpdate::ConfigLocation.getTypeKey()] = LOCATION;
        m_objectTypeKey[ConfigUpdate::ConfigSubsystem.getTypeKey()] = SUBSYSTEM;
        m_objectTypeKey[ConfigUpdate::ConfigProfile.getTypeKey()] = PROFILE;
        m_objectTypeKey[ConfigUpdate::ConfigEntity.getTypeKey()] = ENTITY;
        m_objectTypeKey[ConfigUpdate::ConfigEntityOwner.getTypeKey()] = ENTITY_OWNER;
        m_objectTypeKey[ConfigUpdate::ConfigOperator.getTypeKey()] = OPERATOR;
        m_objectTypeKey[ConfigUpdate::ConfigRegion.getTypeKey()] = REGION;
        m_objectTypeKey[ConfigUpdate::ConfigSecurity.getTypeKey()] = SECURITY;
        m_objectTypeKey[ConfigUpdate::ConfigMessageType.getTypeKey()] = MESSAGE_TYPE;
        m_objectTypeKey[ConfigUpdate::ConfigScadaDisplay.getTypeKey()] = SCADA_DISPLAY;
        m_objectTypeKey[ConfigUpdate::ConfigActionGroup.getTypeKey()] = ACTION_GROUP;
        m_objectTypeKey[ConfigUpdate::ConfigSystemController.getTypeKey()] = SYSTEM_CONTROLLER;
        m_objectTypeKey[ConfigUpdate::ConfigSystemControllerGroup.getTypeKey()] = SYSTEM_CONTROLLER_GROUP;
        m_objectTypeKey[ConfigUpdate::ConfigAlarmRule.getTypeKey()] = ALARM_RULE;
        m_objectTypeKey[ConfigUpdate::ConfigPaZone.getTypeKey()] = PA_ZONE;
        m_objectTypeKey[ConfigUpdate::ConfigPaZoneGroup.getTypeKey()] = PA_ZONE_GROUP;
        m_objectTypeKey[ConfigUpdate::ConfigPaDvaMessage.getTypeKey()] = PA_DVA_MESSAGE;
        m_objectTypeKey[ConfigUpdate::ConfigPaTrainDvaMessage.getTypeKey()] = PA_TRAIN_DVA_MESSAGE;
        m_objectTypeKey[ConfigUpdate::ConfigPaDvaVersion.getTypeKey()] = PA_DVA_VERSION;
        m_objectTypeKey[ConfigUpdate::ConfigPaAtsTriggeredBroadcast.getTypeKey()] = PA_ATS_TRIGGERED_BROADCAST;
        m_objectTypeKey[ConfigUpdate::ConfigPaStation.getTypeKey()] = PA_STATION;
        m_objectTypeKey[ConfigUpdate::ConfigMmsSchedule.getTypeKey()] = MMS_SCHEDULE;
        m_objectTypeKey[ConfigUpdate::ConfigVideoStation.getTypeKey()] = VI_STATION;
        m_objectTypeKey[ConfigUpdate::ConfigVideoTriggeringEvent.getTypeKey()] = VI_TRIGGERING_EVENTS;
		// 7196-Start
		// Added for ECS Config plugins Update
		m_objectTypeKey[ConfigUpdate::ConfigEcMasterMode.getTypeKey()]		= ECS_MASTER_MODE;
        m_objectTypeKey[ConfigUpdate::ConfigEcStationMode.getTypeKey()]		= ECS_STATION_MODE;
        m_objectTypeKey[ConfigUpdate::ConfigEcZone.getTypeKey()]			= ECS_ZONE;
		//For Global Parameters
		m_objectTypeKey[ConfigUpdate::ConfigGlobalParameter.getTypeKey()]	= GLOBAL_PARAMETERS;
		// 7196-End
		m_objectTypeKey[ConfigUpdate::ConfigPidGroupUpdate.getTypeKey()]	= STIS_PID_GROUP;
		//TD15203
		m_objectTypeKey[ConfigUpdate::ConfigIncidentRights.getTypeKey()]	= INCIDENT_RIGHTS;
        //Mintao ++ TD15114 
        m_objectTypeKey[ConfigUpdate::ConfigOPCTagAlias.getTypeKey()]	= OPC_TAG_ALIAS;
		// TD15687 ajlaredo++
		m_objectTypeKey[ConfigUpdate::ConfigSoundFile.getTypeKey()]	= SOUND_FILE;
    }


    bool MessageObjectTypeMap::isMessageValid(std::string& message) const
    {
        return (m_objectTypeKey.count(message) != 0);
    }


    EObjectType MessageObjectTypeMap::getObjectType(std::string& message) const
    {
        std::map<std::string, EObjectType>::const_iterator iter = m_objectTypeKey.find(message);
        TA_ASSERT(iter != m_objectTypeKey.end(), "Unrecognised object type");
        return iter->second;
    }

}

