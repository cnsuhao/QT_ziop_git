

#include "core/synchronisation/src/ThreadGuard.h"
#include "bus/generic_gui_view/src/TransActiveViewManager.h"
#include <QApplication>
#include <QThread>
#include <QGenericArgument>
#include <string>
#include <map>
#include <set>
#include "bus/generic_gui_view/src/TransActiveDialog.h"
 
namespace TA_Base_Bus
{
	 
     using namespace std;
	 TransActiveViewManager::TransActiveViewManager():m_ViewFactory(NULL)
		 ,m_mainView(NULL)  
	 {
		 
	 }

	 ITransActiveView * TransActiveViewManager::getViewfromMap(const char * viewType, const char * viewID)
	 {
		 TA_Base_Core::ThreadGuard guard(m_mapLock);
		 std::string realID= std::string(viewType) + std::string(".") + std::string(viewID);
		 ViewRepositoryMap::iterator it=m_ViewMap.find(realID);
		  ITransActiveView * pItem = NULL;
		 if (it!=m_ViewMap.end())
		 {
			 pItem=  m_ViewMap[realID];
		 }		 
		 return  pItem;
	 }
	 	 
	ITransActiveView * TransActiveViewManager::getView(const char * viewType, const char * viewID,  bool createIfNew)
	{
		TA_Base_Core::ThreadGuard guard(m_mapLock);		 
		ITransActiveView * pItem = NULL;
		//try to find view from cache
		pItem=getViewfromMap(viewType,viewID);
		if (pItem==NULL && createIfNew)
		{
			//check whether the current thread is GUI thread. 
			if (QThread::currentThread() ==QApplication::instance()->thread())
			{
				createAndRegisterView(viewType,viewID);
			}
			else
			{
				//make sure the view will be created and register before return.
				QMetaObject::invokeMethod(this,"createAndRegisterView",Qt::BlockingQueuedConnection,
					Q_ARG(const char*, viewType),
					Q_ARG(const char*, viewID));
			}
			pItem=getViewfromMap(viewType,viewID);
		}
	 
		return  pItem;
	}
		 
	bool TransActiveViewManager::addView(ITransActiveView * item) 
	{
		std::string realID=item->getViewType() + "." +  item->getViewID();
		//TA_Base_Core::ThreadGuard guard(m_mapLock);
		if (m_ViewMap.find(realID) == m_ViewMap.end())
		{
			m_ViewMap[realID] = item;
			if (item->isMainView())
			{
				m_mainView=item;
			}
			return true;
		}
		return false;
	}		
      
	bool TransActiveViewManager::removeView(ITransActiveView * item) 
	{
		std::string realID=item->getViewType() + "." +  item->getViewID();
		TA_Base_Core::ThreadGuard guard(m_mapLock);
		//if (m_ViewMap.find(realID) != m_ViewMap.end())
		//{
		m_ViewMap.erase(realID);
		if (item == m_mainView)
		{
			m_mainView=NULL;
		}
		//}
		return true;		 
	}
	
     void TransActiveViewManager::setViewFactory(IViewFactory* viewFactory) 
	 {
 		 TA_Base_Core::ThreadGuard guard(m_mapLock);
		 m_ViewFactory=viewFactory;		 
	 }
	 	 
	 void TransActiveViewManager::closeAllViews()
	 {
		TA_Base_Core::ThreadGuard guard(m_mapLock);
		ViewRepositoryMap::iterator it=m_ViewMap.begin();
		for (;it!=m_ViewMap.end();++it)
		{
			ITransActiveView * item=it->second;
			item->closeView();	//will trigger removeView on destruction of view, so set it to begin	
			//it=m_ViewMap.begin(); // Cause infinite loop
		}
	 }


	 void TransActiveViewManager::switchLanguage()
	 {
		 TA_Base_Core::ThreadGuard guard(m_mapLock);
		 ViewRepositoryMap::iterator it = m_ViewMap.begin();
		 for (; it != m_ViewMap.end(); ++it)
		 {
			 ITransActiveView * item = it->second;
			 item->translateUI();		
			 //item->autoResize();
		 }
	 }

	 void TransActiveViewManager::autoResizeAllView()
	 {
		 TA_Base_Core::ThreadGuard guard(m_mapLock);
		 ViewRepositoryMap::iterator it = m_ViewMap.begin();
		 for (; it != m_ViewMap.end(); ++it)
		 {
			 ITransActiveView * item = it->second;
			 item->translateUI();
			 item->autoResize();
		 }
	 }
	 
	 ITransActiveView *  TransActiveViewManager::getMainView()
	 {
		return m_mainView;
	 }

	 ITransActiveView * TransActiveViewManager::createView(const char * viewType, const char * viewID)
	 {
		 //TA_Base_Core::ThreadGuard guard(m_mapLock);		  
		 if (m_ViewFactory!=NULL)
		 {
			 ITransActiveView* v= m_ViewFactory->createView(viewType);
			 if (v != NULL)
			 {
				 v->setViewID(viewID);				
				 v->init();
				 return v;
			 }			 
		 }
		  
		 return NULL;
	 }

	 void TransActiveViewManager::createAndRegisterView(const char * viewType, const char * viewID)
	 {
		 //TA_Base_Core::ThreadGuard guard(m_mapLock);		
		 ITransActiveView * pItem = NULL;
		 pItem = createView(viewType,viewID);
		 addView(pItem);       
	 }


	 void TransActiveViewManager::postBoardcastMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam)
	 {
		 notifyMessage(false,messageID,wParam,lParam);
	 }


	 void TransActiveViewManager::sendBoardcastMessage(unsigned long messageID, TRA_WPARAM wParam, TRA_LPARAM lParam)
	 {
		 notifyMessage(true,messageID,wParam,lParam);
	 }

	 void TransActiveViewManager::notifyMessage(bool isSync, unsigned long messageID, TRA_WPARAM wParam, TRA_LPARAM lParam)
	 {		
		 TA_Base_Core::ThreadGuard guard(m_mapLock);
		 ViewRepositoryMap::iterator it=m_ViewMap.begin();
		 for (; it != m_ViewMap.end(); ++it)
		 {
			 ITransActiveView * item=it->second;
		     if (item==NULL)
			 {
				 continue;
			 }			 
			 if (isSync==false)  
			 {					
				 ////asynchronization call
				 item->postMessage(messageID, wParam, lParam);
				 
			 }
			 else 
			 {
				 //Synchronization call
				 item->sendMessage(messageID, wParam, lParam);
			 }
 
		 }//end of for       

	 }

}
 