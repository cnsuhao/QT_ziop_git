/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/synchronisation/src/Semaphore.cpp $
  * @author:  B Fountain
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * Implementation of the ThreadGuard class
  *
  */

#include "core/synchronisation/src/Semaphore.h"
#include "omnithread.h"

namespace TA_Base_Core
{
	Semaphore::Semaphore( unsigned int initial )
		: m_semaphore( NULL )
	{
		m_semaphore = new omni_semaphore( initial );
	}


	Semaphore::~Semaphore()
	{
		if ( NULL != m_semaphore )
		{
			delete m_semaphore;
			m_semaphore = NULL;
		}
	}


	void Semaphore::wait(void)
	{
		m_semaphore->wait();
	}


	bool Semaphore::tryWait(void)
	{
		if ( 0 == m_semaphore->trywait() )
		{
			return false;
		}

		return true;
	}


	void Semaphore::post(void)
	{
		m_semaphore->post();
	}
}
