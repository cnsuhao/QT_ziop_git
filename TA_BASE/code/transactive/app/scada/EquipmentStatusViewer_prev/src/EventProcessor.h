/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scada/EquipmentStatusViewer_prev/src/EventProcessor.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
#if !defined(AFX_EVENTPROCESSOR_H__DFE14FCD_A1AB_467A_9359_85891741D7B6__INCLUDED_)
#define AFX_EVENTPROCESSOR_H__DFE14FCD_A1AB_467A_9359_85891741D7B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace TA_Base_App
{
	class CEventProcessor  
	{
		virtual void stopReportLoading(bool aStop) = 0;
	};

} // TA_Base_App

#endif // !defined(AFX_EVENTPROCESSOR_H__DFE14FCD_A1AB_467A_9359_85891741D7B6__INCLUDED_)
