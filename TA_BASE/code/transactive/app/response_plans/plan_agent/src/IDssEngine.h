/*
 * The source code in this file is the property of Ripple Systems and is not for redistribution in any form.
 *
 * Source : $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/IDssEngine.h $
 * @author  Huang Qi
 * @version $Revision: #1 $
 * Last modification : $DateTime: 2015/10/09 15:42:35 $
 * Last modified by : $Author: CM $
 *
 * 
 */

#ifndef IDSSENGINE_H
#define IDSSENGINE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace TA_Base_App
{

class IDssEngine
{
public:
    virtual void triggerUpdate( const unsigned long ulTriggerKey, const unsigned long ulTimestamp, const bool bValue, const bool bInitial ) = 0;
    virtual bool isEnabled() = 0;
};

} // namespace TA_Base_App

#endif // IDSSENGINE_H
