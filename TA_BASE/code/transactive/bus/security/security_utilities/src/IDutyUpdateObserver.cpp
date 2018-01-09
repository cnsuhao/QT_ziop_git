/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/security/security_utilities/src/IDutyUpdateObserver.cpp $
  * @author:  Gregg Kirkpatrick
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Implements the update() method of IDutyUpdateObserver.
  *
  */

#ifdef _MSC_VER
	#pragma warning(disable: 4503)  // decorated name length exceeded, name was truncated
#endif // _MSC_VER

#include "bus/security/security_utilities/src/SecurityUtilitiesInternal.h"

namespace TA_Base_Bus
{
	IDutyUpdateObserver::IDutyUpdateObserver( DutyUpdateSubject& dutyUpdate ) :
		SeObserver<DutyUpdateSubject>(dutyUpdate)
	{
	}

	void IDutyUpdateObserver::update( DutyUpdateSubject& dutyUpdate )
	{
		dutyUpdate.interpretUpdate(*this);
	}

    void IDutyUpdateObserver::dutyChanged( const std::string& session )
	{
		LOG( SOURCE_INFO, GENERIC_LOG_TYPE, DEBUG_LOG_LEVEL, "Duty change notification ignored (SessionID %s)", session.c_str() );
	}
}
