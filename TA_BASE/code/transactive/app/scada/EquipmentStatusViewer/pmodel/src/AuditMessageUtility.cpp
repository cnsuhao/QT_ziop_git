/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/AuditMessageUtility.cpp $
  * @author:  Alan Brims
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * Common utility class to prepare the DescriptionParameters collection before
  * calling bus/scada/common_library/AuditMessageUtility::logAuditMessage.
  *
  * The DescriptionParameters consists of NamedPairs for ReportName and LocationName, and
  * this is the same for all audit messages from the EquipmentStatusViewer. The entity key 
  * of the ESV application itself is stored in the static s_entityKey; this is then
  * available to all instances of this class, so we can create an instance just when we
  * need it and avoid the Singleton pattern.
  *
  */

#include "stdafx.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/AuditMessageUtility.h"
#include "bus/scada/common_library/src/AuditMessageUtility.h"
#include "core/message/src/NameValuePair.h"

namespace TA_Base_App
{
    TA_Base_Core::PrimitiveWrapper<unsigned long> CAuditMessageUtility::s_entityKey;

	CAuditMessageUtility::CAuditMessageUtility()
	{		
	}

	CAuditMessageUtility::~CAuditMessageUtility()
	{
	}


    void CAuditMessageUtility::setEntityKey(unsigned long entityKey)
    {
        s_entityKey.setValue(entityKey);
    }


	void CAuditMessageUtility::LogAuditMessage(std::string &aReportName, 
												std::string &aLocationName, 
												const TA_Base_Core::MessageType &aMessageType)
	{
	
		try
		{				
            unsigned long entityKey = s_entityKey.getValue();

			TA_Base_Core::DescriptionParameters descriptionParameters;
			TA_Base_Core::NameValuePair pair1("ReportName", aReportName);
			TA_Base_Core::NameValuePair pair2("LocationName", aLocationName);

			descriptionParameters.push_back(&pair1);
			descriptionParameters.push_back(&pair2);

			TA_Base_Bus::AuditMessageUtility auditMsgUtil(TA_Base_Core::ESVAudit::Context);

            auditMsgUtil.logAuditMessageWithOperatorId( aMessageType, descriptionParameters, entityKey);

        } 	
		catch(...)
		{
			// failed so just ignore
            LOG(SourceInfo, 
                TA_Base_Core::DebugUtil::ExceptionCatch, 
                "Unknown", 
                "Caught in LogAuditMessage");
		}
	}

} // TA_Base_App
