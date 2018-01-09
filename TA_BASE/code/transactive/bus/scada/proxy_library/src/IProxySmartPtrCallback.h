/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/scada/proxy_library/src/IProxySmartPtrCallback.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
///////////////////////////////////////////////////////////
//  IProxySmartPtrCallback.h
//  Implementation of the Interface IProxySmartPtrCallback
//  Created on:      28-Jun-2004 09:45:05 AM
///////////////////////////////////////////////////////////

#if !defined(IProxySmartPtrCallback_613B8976_A9D3_4a20_ACF7_CB788877362D__INCLUDED_)
#define IProxySmartPtrCallback_613B8976_A9D3_4a20_ACF7_CB788877362D__INCLUDED_

/**
 * Interface used to notify scada factory of proxy smart pointer changes
 * @version 1.0
 * @created 28-Jun-2004 09:45:05 AM
 */
namespace TA_Base_Bus
{
	class IProxyObserverNotifier;
	class IEntityUpdateEventProcessor;

	class IProxySmartPtrCallback
	{
		public:

			/*
			 * DataPoint Proxy created
			 */
			virtual void onCreateProxy(IProxyObserverNotifier* observerNotifier, IEntityUpdateEventProcessor& observer) = 0;

			/*
			 * DataPoint Proxy copied
			 */
			virtual void onCopyProxy(IProxyObserverNotifier* observerNotifier, IEntityUpdateEventProcessor& observer) = 0;

			/**
			 * DataPoint Proxy deleted
			 */
			virtual void onDeleteProxy(IProxyObserverNotifier* observerNotifier, IEntityUpdateEventProcessor& observer) =0;

	};
};
#endif // !defined()
