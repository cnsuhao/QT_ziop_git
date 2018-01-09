/**
  * The source code in this file is the property of Ripple Systems and is not for redistribution in
  * any form.
  *
  * Source:		$File: //depot/4669_T01271350/TA_BASE/transactive/core/threads/src/AsyncWorkThreadPool.h $
  * @author:	HoaVu
  * @version $Revision: #1 $
  *
  * Last modification : $DateTime: 2008/11/28 16:26:01 $
  * Last modified by : $Author: builder $
  *
  *
  * AsyncWorkThreadPool provides a single interface for scheduling of asynchronous operations,
  * which are represented by instances of IWorkItem interface.  For the current design, AsyncWorkThreadPool
  * creates and manages ten (10) threaded workers to execute the registered asynchronous operations.
  *
  */

#ifndef ASync_AsyncWorkThreadPool_H
#define ASync_AsyncWorkThreadPool_H

#include <vector>

#include "core/threads/src/IThreadPoolWorkerCallback.h"
#include "core/threads/src/ThreadPoolManager.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/threads/src/ThreadPoolManager.h"
#include "core/threads/src/IWorkItem.h"

namespace TA_Base_Bus
{
	//class ThreadPoolManager;

	class AsyncWorkThreadPool	
	{
	public:
		/**
		*
		* getInstance
		*
		* Get an instance of AsyncWorkThreadPool.  Caller is responsible for destroying
		* the instance by calling removeInstance(), passing in the instance created
		* by this method.
		*
		*/
		static AsyncWorkThreadPool * getInstance();


		/**
		*
		* removeInstance
		*
		* Remove an instance of AsyncWorkThreadPool that has been created by the getInstance()
		*
		* @param	instance	The instance of the AsyncWorkThreadPool to be destroyed
		*
		*/
		static void removeInstance ();


		//
		// Operations specific to AsyncWorkThreadPool
		//


		/**
		*
		* queueWorkItem
		*
		* Queue the specified IWorkItem for processing by a thread in the thread pool
		* and return immediately
		*
		* @param	workItem	The work item to be processed by a thread
		*
		*/
		void queueWorkItem (TA_Base_Core::IWorkItemPtr workItem );


		/**
		*
		* removeWorkItem
		*
		* Remove the specified IWorkItem so that it is no longer processed by a thread
		* in the thread pool.  This will block until the specified IWorkItem is successfully
		* removed from the ThreadPool.
		*
		* @param	workItem	The work item to be processed by a thread
		*
		*/
		void removeWorkItem ( TA_Base_Core::IWorkItemPtr workItem );


        /** 
          * getOriginalSize
          *
          * @return the original pool size
          */
        unsigned long getOriginalSize();
        
        /** 
          * getNumberOfThreads
          *
          * @return the current thread pool size
          */
        unsigned long getNumberOfThreads();


        /** 
          * getUpperLimit
          *
          * @return the maximum size the pool can be
          */
        unsigned long getUpperLimit();


        /** 
          * setUpperLimit
          *
          * @return the maximum size the pool can be
          */
        void setUpperLimit(unsigned long upperLimit);

	private:

		virtual ~AsyncWorkThreadPool();
		AsyncWorkThreadPool();
		AsyncWorkThreadPool(const AsyncWorkThreadPool & theInstance){};
		AsyncWorkThreadPool & operator= (const AsyncWorkThreadPool & rhs){};

	protected:

		static AsyncWorkThreadPool *					m_singletonManager;
		static TA_Base_Core::ReEntrantThreadLockable				m_lock;	

		TA_Base_Core::ThreadPoolManager*							m_threadPoolManager;
	};
};

#endif
