/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/GenericEventArgs.h $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  *
  * 
  *
  */

#ifndef GENERIC_EVENT_ARGS_1234567890_H
#define GENERIC_EVENT_ARGS_1234567890_H
#include "bus/generic_gui_pmod/src/CommandGateway.h"

namespace TA_Base_App
{
	class GenericEventArgs: public TA_Base_Bus::EventArgs
	{
	public:
		GenericEventArgs():TA_Base_Bus::EventArgs()
		{
			Param1 = NULL;
			Param2 = NULL;
			Param3 = NULL;
			Param4 = NULL;
		}

		~GenericEventArgs()
		{
			/*if(Param1 != NULL)
			{
				delete Param1;
				Param1 = NULL;
			}
			if(Param2 != NULL)
			{
				delete Param2;
				Param2 = NULL;
			}

			if(Param3 != NULL)
			{
				delete Param3;
				Param3 = NULL;
			}

			if(Param4 != NULL)
			{
				delete Param3;
				Param3 = NULL;
			}*/
		}

		void* Param1;
		void* Param2;
		void* Param3;
		void* Param4;
	};
}

#endif  // GENERIC_EVENT_ARGS_1234567890_H
