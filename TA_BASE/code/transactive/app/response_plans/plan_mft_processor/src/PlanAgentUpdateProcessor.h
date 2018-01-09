/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_mft_processor/src/PlanAgentUpdateProcessor.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Plan Manager's one and only subscriber of Plan Agent updates for plan execution, customisation and configuration.
  * Update consumers should register with PlanAgentUpdateProcessor to be notified of updates as they are received.
  *
  **/

#if !defined(AFX_PLANAGENTUPDATEPROCESSOR_H__INCLUDED_)
#define AFX_PLANAGENTUPDATEPROCESSOR_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/message/src/SpecialisedMessageSubscriber.h"
#include "core/message/IDL/src/CommsMessageCorbaDef.h"


namespace TA_Base_Core
{
    struct ActivePlanDetail;
}

namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // PlanAgentUpdateProcessor

    class PlanAgentUpdateProcessor : public TA_Base_Core::SpecialisedMessageSubscriber<TA_Base_Core::CommsMessageCorbaDef>
    {
    // Operations
    public:
        ~PlanAgentUpdateProcessor();

        static PlanAgentUpdateProcessor &getInstance();
        static void destroyInstance();

    protected:
        void receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message);

    private:
        PlanAgentUpdateProcessor();
        PlanAgentUpdateProcessor(const PlanAgentUpdateProcessor &other);
        PlanAgentUpdateProcessor &operator=(const PlanAgentUpdateProcessor &other);

    // Attributes
    private:
        static PlanAgentUpdateProcessor *s_instance;
        static TA_Base_Core::NonReEntrantThreadLockable s_singletonLock;
    };
}

#endif // !defined(AFX_PLANAGENTUPDATEPROCESSOR_H__2AC77A74_70F8_49BE_A459_C95D51962BC2__INCLUDED_)
