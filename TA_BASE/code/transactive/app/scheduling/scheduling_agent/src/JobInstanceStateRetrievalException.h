/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution in any form.
  *
  * Source:    $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_agent/src/JobInstanceStateRetrievalException.h $
  * @author:   Dirk McCormick
  * @version:  $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Thrown when the state of a job instance cannot be retrieved
  *
  */


#ifndef JOB_INSTANCE_STATE_RETRIEVAL_EXCEPTION_H
#define JOB_INSTANCE_STATE_RETRIEVAL_EXCEPTION_H

#include "SchedulingException.h"
#include "app/scheduling/scheduling_agent/src/JobRunner.h"

namespace TA_Base_App
{

class JobInstanceStateRetrievalException: public SchedulingException
{

public:

    JobInstanceStateRetrievalException(
        const std::string & message)
        :
        SchedulingException(message)
    {
    }

    JobInstanceStateRetrievalException(const JobInstanceStateRetrievalException & rhs)
        :
        SchedulingException(rhs)
    {
    }

    JobInstanceStateRetrievalException()
        :
        SchedulingException()
    {
    }

    virtual ~JobInstanceStateRetrievalException() throw () {};

    virtual const char *what() const throw ()
    {
        return SchedulingException::what();
    }

};


}; // TA_Base_App

#endif // JOB_INSTANCE_STATE_RETRIEVAL_EXCEPTION_H
