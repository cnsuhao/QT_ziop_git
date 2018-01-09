

#include "core/synchronisation/src/ThreadGuard.h"
#include "bus/generic_gui_common/src/TransActiveViewManager.h"
#include <string>
#include <map>
#include <set>

 
namespace TA_Base_Bus
{
	 
     using namespace std;
	 TransActiveViewManager::TransActiveViewManager():m_ViewFactory(NULL)
	 {
		 
	 }
	 
	ITransActiveView * TransActiveViewManager::getView(const char * viewType, const char * viewID,  bool createIfNew)
	{
		TA_Base_Core::ThreadGuard guard(m_mapLock);
		std::string realID= std::string(viewType) + std::string(".") + std::string(viewID);
		ViewRepositoryMap::iterator it=m_ViewMap.find(realID);
		if (it!=m_ViewMap.end())
		{
			return  m_ViewMap[realID];
		}
		ITransActiveView * pItem = NULL;
		if (createIfNew)
		{
			pItem = createView(viewType,viewID);
			m_ViewMap[realID] = pItem;
		}
		return  pItem;

	}

	 
	bool TransActiveViewManager::addView(ITransActiveView * item) 
	{
		std::string realID=item->getViewType() + "." +  item->getViewID();
		TA_Base_Core::ThreadGuard guard(m_mapLock);
		if (m_ViewMap.find(realID) == m_ViewMap.end())
		{
			m_ViewMap[realID] = item;
			return true;
		}
		return false;
	}
		
      
	bool TransActiveViewManager::removeView(ITransActiveView * item) 
	{
		std::string realID=item->getViewType() + "." +  item->getViewID();
		TA_Base_Core::ThreadGuard guard(m_mapLock);
		ITransActiveView * pItem = m_ViewMap[realID];
		m_ViewMap.erase(realID);
		return true;
		 
	}
	

     void TransActiveViewManager::setViewFactory(IViewFactory* viewFactory) 
	 {
 		 TA_Base_Core::ThreadGuard guard(m_mapLock);
		 m_ViewFactory=viewFactory;		 
	 }

	 ITransActiveView * TransActiveViewManager::createView(const char * viewType, const char * viewID)
	 {
		 TA_Base_Core::ThreadGuard guard(m_mapLock);		  
		 if (m_ViewFactory!=NULL)
		 {
			 ITransActiveView* v= m_ViewFactory->createView(viewType);
			 if (v != NULL)
			 {
				 v->setViewID(viewID);
				 addView(v);
				 return v;
			 }			 
		 }
		  
		 return NULL;

	 }

}
 