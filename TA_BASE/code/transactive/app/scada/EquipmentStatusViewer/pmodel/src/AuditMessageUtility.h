/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/AuditMessageUtility.h $
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

#if !defined(AFX_AUDITMESSAGEUTILITY_H__EE8A5E03_48A5_47E6_B232_47D67A619541__INCLUDED_)
#define AFX_AUDITMESSAGEUTILITY_H__EE8A5E03_48A5_47E6_B232_47D67A619541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "core/message/types/ESVAudit_MessageTypes.h"
#include "core/data_access_interface/src/PrimitiveWrapper.h"

namespace TA_Base_App
{
	class CAuditMessageUtility  
	{
	public:
		void LogAuditMessage(std::string &aReportName, std::string &aLocationName, const TA_Base_Core::MessageType &aMessageType);
		
		CAuditMessageUtility();
		virtual ~CAuditMessageUtility();

        /// Sets the entity to be used for audit message purposes
        /// (this should be the entity key of the application)
        static void setEntityKey(unsigned long entityKey);

	private:
        static TA_Base_Core::PrimitiveWrapper<unsigned long> s_entityKey;
	};

} // TA_Base_App

#endif // !defined(AFX_AUDITMESSAGEUTILITY_H__EE8A5E03_48A5_47E6_B232_47D67A619541__INCLUDED_)
