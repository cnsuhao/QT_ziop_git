/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/3001/transactive/app/incidents/incident_manager/pmodel/src/StartTabEventArgs.h $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#ifndef __START_TAB_EVENT_ARGS_H_INCLUDED__
#define __START_TAB_EVENT_ARGS_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "bus/generic_gui_pmod/src/PModelDataType.h"

#pragma once

namespace TA_Base_App
{
	class StartTabEventArgs :
		public TA_Base_Bus::EventArgs
	{
	public:
		StartTabEventArgs(int typeCode, std::string strArgValue)
			: EventArgs(typeCode)
			, m_strArgsValue(strArgValue)
		{};

		~StartTabEventArgs(void){};

		std::string getStartTab(){ return m_strArgsValue; };

	private:
		std::string m_strArgsValue;
	};

}

#endif //__START_TAB_EVENT_ARGS_H_INCLUDED__

