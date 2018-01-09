/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: $
* @author:  
* @version: $Revision:  $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:  $
*
* 
*
*/

#ifndef IGUIOBSERVER_INTERFACE_1234567890_H
#define IGUIOBSERVER_INTERFACE_1234567890_H		

#include <string>
#include <Windows.h>

namespace TA_Base_App
{	
	class IGuiObserver
	{	
	public:
		virtual std::string getStringResourceFormated(UINT resourceId, std::string argument1) = 0;
		virtual std::string getStringResource(UINT resourceId) = 0;
		virtual std::string  getStringResourceFormated(UINT resourceId, std::string argument1,std::string argument2) = 0;
		virtual std::string  getStringResourceFormated(UINT resourceId, std::string argument1,UINT argument2) = 0;
		virtual std::string  getStringResourceFormated(UINT resourceId, std::string argument1,double argument2,std::string argument3) = 0;
		virtual std::string  getStringResourceFormated(UINT resourceId, std::string argument1,std::string argument2,std::string argument3) = 0;
		virtual std::string  getStringResourceFormated(UINT resourceId, UINT argument1) = 0;
		static IGuiObserver* getInstance();
		static void freeInstance();	
		static void setInstance(IGuiObserver * instance);
	private:
		static IGuiObserver * m_instance;		
	};
}

#endif //IGUIOBSERVER_INTERFACE_1234567890_H