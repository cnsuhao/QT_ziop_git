#ifndef Generic_GUI_Common_TransActiveViewManager_H_
#define Generic_GUI_Common_TransActiveViewManager_H_


#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "bus/generic_gui_common/src/ITransActiveViewManager.h"
//#include "bus/generic_gui_common/src/IViewFactory.h"
//#include "bus/generic_gui_common/src/ITransActiveView.h"

#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{
	 
 
	/** Interface for View Manager
	 *	
	 */
	class TransActiveViewManager : public ITransActiveViewManager
	{
	public:
		
		TransActiveViewManager();
		
		virtual ~TransActiveViewManager() {};

		/** get the view based on specific name
		* 
		*
		* @param viewType
		* @param viewID
		* @return ITransActiveView *
		*/
		virtual ITransActiveView * getView(const char * viewType, const char * viewID,  bool createIfNew=true);



		/** add item to the repository. If duplicate item name exists, it will return false
		* 
		*
		* @param item
		* @return bool
		*/
		virtual bool addView(ITransActiveView * item) ;



		/** remove Item from the repository.
		* 
		*
		* @param itemName
		* @return void
		*/
		virtual bool removeView(ITransActiveView * item);


		/** add item to the repository. If duplicate item name exists, it will return false
		* 
		*
		* @param item
		* @return bool
		*/
		virtual void setViewFactory(IViewFactory* viewFactory) ;
	private:
		ITransActiveView * createView(const char * viewType, const char * viewID);

		//typedef std::vec<IViewFactory*> ViewFactoryVec;
		typedef std::map<std::string, ITransActiveView*> ViewRepositoryMap;

        TA_Base_Core::ReEntrantThreadLockable m_mapLock;  ///<  lock for repository map  
		IViewFactory*  m_ViewFactory;  //view factories	
		ViewRepositoryMap m_ViewMap;  //view is identified by view plus id
		 
	};

}
#endif