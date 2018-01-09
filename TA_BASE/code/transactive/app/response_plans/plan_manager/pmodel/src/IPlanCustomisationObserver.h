﻿//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/IPlanCustomisationObserver.h $
// @author:  Bart Golab
// @version: $Revision: #3 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>

#if !defined(AFX_IPLANCUSTOMISATIONOBSERVER_H__1F1798B7_0F68_4F60_930F_C983C721E08D__INCLUDED_)
#define AFX_IPLANCUSTOMISATIONOBSERVER_H__1F1798B7_0F68_4F60_930F_C983C721E08D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // IPlanCustomisationObserver Interface

    class IPlanCustomisationObserver
    {
    public:
        virtual void planCustomised(const TA_Base_Core::PlanCustomisationUpdate &planCustomDetail) = 0;
    };

} // namespace TA_Base_App

#endif // !defined(AFX_IPLANCUSTOMISATIONOBSERVER_H__1F1798B7_0F68_4F60_930F_C983C721E08D__INCLUDED_)
