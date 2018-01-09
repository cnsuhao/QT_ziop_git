/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File:  $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  *
  * 
  *
  */

#ifndef POST_MESSAGE__EVENT_ARGS_1234567890_H
#define POST_MESSAGE__EVENT_ARGS_1234567890_H
#include "bus/generic_gui_pmod/src/CommandGateway.h"

namespace TA_Base_App
{
	class PostMessageEventArgs: public TA_Base_Bus::EventArgs
	{
	public:
		PostMessageEventArgs(UINT uID):TA_Base_Bus::EventArgs()
		{
			m_messageID = uID;
			m_lparam=0;
		}

		PostMessageEventArgs(UINT uID,LONG_PTR lparam):TA_Base_Bus::EventArgs()
		{
			m_messageID = uID;
			m_lparam = lparam;
		}

		~PostMessageEventArgs()
		{
		}

		UINT getMessageId()
		{
			return m_messageID;
		}
		
		LONG_PTR getLParam()
		{
			return m_lparam;
		}


	private:
		UINT m_messageID;
		LONG_PTR m_lparam;

	};
}

#endif  // POST_MESSAGE__EVENT_ARGS_1234567890_H
