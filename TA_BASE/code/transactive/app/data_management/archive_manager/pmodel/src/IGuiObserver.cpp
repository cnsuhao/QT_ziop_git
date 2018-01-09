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

#include "app/data_management/archive_manager/pmodel/src/IGuiObserver.h"
namespace TA_Base_App
{
	IGuiObserver* IGuiObserver::m_instance = NULL;

	IGuiObserver* IGuiObserver::getInstance()
	{		
		return m_instance;
	}

	void IGuiObserver::freeInstance()
	{		
		if(m_instance!=NULL)
		{
			delete m_instance;
			m_instance = NULL;			
		}	
	}
	
	void IGuiObserver::setInstance(IGuiObserver * instance)
	{
		if(m_instance == NULL)
		{
			m_instance = instance;
		}
	}
}