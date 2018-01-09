#ifndef Generic_GUI_Common_TransActiveViewManager_H_
#define Generic_GUI_Common_TransActiveViewManager_H_


#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
 

#include <string>
#include <map>
#include <set>
#include <QObject>

namespace TA_Base_Bus
{
	 
 
	/** TransActiveViewManager
	 * It is responsible for retrieving/managering views
	 *	
	 */
	class TransActiveViewManager : public QObject,public ITransActiveViewManager 
	{
		Q_OBJECT
	signals:
		void  createAndRegisterViewSignal(const char * viewType, const char * viewID);
    private slots:
		void  createAndRegisterView(const char * viewType, const char * viewID);
		 
	public:
		
		TransActiveViewManager();
		
		virtual ~TransActiveViewManager() {};

		/** get the view based on specific name
		* 
		*
		* @param viewType: name of view type
		* @param viewID:   ID of view
		* @param createIfNew:  True(create a new view if there is not a view in the ViewManager
		* @return ITransActiveView *
		*/
		virtual ITransActiveView * getView(const char * viewType, const char * viewID,  bool createIfNew=true);

 
		/** remove Item from the repository.
		* 
		* @param itemName
		* @return void
		*/
		virtual bool removeView(ITransActiveView * item);


		/** set View factory
		*
		* @param viewFactory		 
		*/
		virtual void setViewFactory(IViewFactory* viewFactory) ;


		/** close All Views
		*  
		*/
		virtual void closeAllViews();


		/** switchLanguage
		*    switch language of all views
		*/
		virtual void switchLanguage();

		virtual void autoResizeAllView();

		/** get Main View
		*  there should only have one main view in an application.
		*/
		virtual ITransActiveView *  getMainView();

		/* postBoardcastMessage, post asynchronous message to all views.
		*  The message will be queued in main thread
		*  If the requirement is sending message to a view, It is recommanded to get the view and use the view's postMessage directly.
		*  messageID:  message ID(should be unique in one component)
		*  wParam: additional message-specific information
		*  lParam: additional message-specific information
		*/
		virtual void postBoardcastMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam);

		/* sendBoardcastMessage, send synchronous message to all views.
		*    This function blocks the ViewManager if there is a view which process the message slowly.
		*    It should be used VERY VERY carefully.
		*    If the requirement is sending message to a view, It is recommanded to get the view and use the view's sendMessage directly.
		*  messageID:  message ID(should be unique in one component)
		*  wParam: additional message-specific information
		*  lParam: additional message-specific information
		*/
		virtual void sendBoardcastMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam);

	 
	private:
		ITransActiveView * createView(const char * viewType, const char * viewID);
		ITransActiveView * getViewfromMap(const char * viewType, const char * viewID);

		/** add item to the repository. If duplicate item name exists, it will return false
		* 
		*
		* @param item
		* @return bool
		*/
		virtual bool addView(ITransActiveView * item) ;


		void notifyMessage(bool isSync, unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam);

		//typedef std::vec<IViewFactory*> ViewFactoryVec;
		typedef std::map<std::string, ITransActiveView*> ViewRepositoryMap;

        TA_Base_Core::ReEntrantThreadLockable m_mapLock;  ///<  lock for repository map  
		IViewFactory*  m_ViewFactory;  //view factories	
		ViewRepositoryMap m_ViewMap;   //view is identified by view plus id
		ITransActiveView*  m_mainView;
		 
	};

}
#endif