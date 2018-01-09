/**
  * The source code in this file is the property of Ripple Systems and is not for redistribution in
  * any form.
  *
  * Source:		$File: //depot/4669_T01271350/TA_BASE/transactive/core/threads/src/AsyncWorkThreadPool.cpp $
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


#include <sstream>
#include "bus/generic_gui_pmod/src/AsyncWorkThreadPool.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/threads/src/ThreadPoolManager.h"


namespace TA_Base_Bus
{
	//
	// static declaration
	//

	AsyncWorkThreadPool *				AsyncWorkThreadPool::m_singletonManager = 0;
	TA_Base_Core::ReEntrantThreadLockable			AsyncWorkThreadPool::m_lock;

	AsyncWorkThreadPool::AsyncWorkThreadPool()
		: m_threadPoolManager (NULL)
	{
		// create ThreadedWorker objects, based on the runtime parameter
		// "ThreadedWorkers". Use the default number if not defined
		int maxNumber =1;	 
		m_threadPoolManager = new TA_Base_Core::ThreadPoolManager(maxNumber, maxNumber);
		TA_ASSERT (m_threadPoolManager != NULL, "Thread Pool Manager not created");
	}


	AsyncWorkThreadPool::~AsyncWorkThreadPool()
	{
		delete m_threadPoolManager;
		m_threadPoolManager = NULL;
	}

	AsyncWorkThreadPool * AsyncWorkThreadPool::getInstance()
	{
		// prevent multiple threads attempting to delete/create
		// simultaneously
		TA_THREADGUARD( m_lock );

		// if the singleton has not been created yet
		if ( 0 == m_singletonManager )
		{
			// create one and only one instance
			m_singletonManager = new AsyncWorkThreadPool();
		}
		return m_singletonManager;
	}


	void AsyncWorkThreadPool::removeInstance (  )
	{
		// prevent multiple threads attempting to delete/create
		// simultaneously
		TA_THREADGUARD( m_lock );

		if ( 0 != m_singletonManager )
		{			
			// destroy the only instance
			delete m_singletonManager;
			m_singletonManager = 0;
			
		}
		// else do nothing
	}

	void AsyncWorkThreadPool::queueWorkItem ( TA_Base_Core::IWorkItemPtr workItem )
	{
		m_threadPoolManager->queueWorkItem(workItem);
	}
	
	void AsyncWorkThreadPool::removeWorkItem ( TA_Base_Core::IWorkItemPtr workItem )
	{
		m_threadPoolManager->removeWorkItem(workItem);
	}


    unsigned long AsyncWorkThreadPool::getOriginalSize()
    {
        return m_threadPoolManager->getOriginalSize();
    }
    
    
    unsigned long AsyncWorkThreadPool::getNumberOfThreads()
    {
        return m_threadPoolManager->getNumberOfThreads();
    }


    unsigned long AsyncWorkThreadPool::getUpperLimit()
    {
        return m_threadPoolManager->getUpperLimit();
    }


    void AsyncWorkThreadPool::setUpperLimit(unsigned long upperLimit)
    {
        m_threadPoolManager->setUpperLimit(upperLimit);
    }

}
