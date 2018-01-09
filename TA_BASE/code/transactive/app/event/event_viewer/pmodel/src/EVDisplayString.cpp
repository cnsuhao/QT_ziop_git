/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source: $File: //depot/3001_TIP/TA_BASE/transactive/app/event/event_viewer/pmodel/src/EVDisplayStrings.cpp $
* @author
* @version $Revision: # $
*
* Last modification: $DateTime:  $
* Last modified by: 
* 
* 
*
*/

#include "app/event/event_viewer/pmodel/src/EVDisplayStrings.h"

namespace TA_Base_App
{
	EVDisplayStrRes* EVDisplayStrRes::m_instance = NULL;

	EVDisplayStrRes* EVDisplayStrRes::GetInstance()
	{
		if(m_instance == NULL)
		{
			m_instance = new EVDisplayStrRes();
		}
		return m_instance;
	}
	
	void EVDisplayStrRes::ReleaseInstance()
	{
		if(m_instance != NULL)
		{
			delete m_instance;
			m_instance = NULL;
		}
	}
}