#ifndef IRuntimeEnvionmentNotification_H_
#define IRuntimeEnvionmentNotification_H_

 
#include <string> 
#include <vector>

namespace TA_Base_Bus
{ 
	/** Interface for runtime environment change notification, including server is down/up, entity data change or duty change 
	 *	
	 */
    class IRuntimeEnvionmentNotification
	{
	public:
		virtual ~IRuntimeEnvionmentNotification(){ };

		/**
		* Notification of entity data change.
		*
		* This will be called by GenericGUI when it receives a callback indicating that the
		* GUI entity has been modified. GenericGUI will have invalidated the entity database
		* object and will tell the GUI which items were modified. The GUI just has to retrieve
		* the details out of it and update accordingly. This should be implemented by the
		* lowest level class as it knows exactly what to do with the updates.
		*
		* @param changes - This is a list of all attributes changed in the
		*                                  object.
		*
		*/
		virtual void entityChanged(const std::vector<std::string>& changes) = 0;


		/**
		* notify server is down.
		*
		* This is called when the server the workstation is using goes down. The GUI should
		* indicate to the user that the server is down. It should also then limit operations
		* so that the user can only view existing data. This must be implemented by the lowest
		* level class as it knows what to do when this occurs.
		*/
		virtual void serverIsDown() = 0;


		/**
		* Notify server is up
		*
		* This is called when the server the workstation is using starts functioning again.
		* The GUI should indicate to the user that the server is functioning. It should 
		* then allow the user to perform all valid operations again. This must be implemented by
		* the lowest level class as it knows what to do when this occurs.
		*/
		virtual void serverIsUp() = 0;

		/**
		* notify that duty is changed
		*
		* The method notifies an application that duty has changed for the session in which it is running.
		* The application is responsible for re-checking all duty based functionality.
		*/
		virtual void dutyChanged() = 0;
	};
}
#endif