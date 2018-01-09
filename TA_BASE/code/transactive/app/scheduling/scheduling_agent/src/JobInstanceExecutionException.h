/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution in any form.
  *
  * Source:    $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_agent/src/JobInstanceExecutionException.h $
  * @author:   Dirk McCormick
  * @version:  $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Thrown when a job instance cannot be executed, or fails during execution.
  *
  */


#ifndef JOB_INSTANCE_EXECUTION_EXCEPTION_H
#define JOB_INSTANCE_EXECUTION_EXCEPTION_H

#include "SchedulingException.h"
#include "app/scheduling/scheduling_agent/src/JobRunner.h"

namespace TA_Base_App
{

class JobInstanceExecutionException: public SchedulingException
{

public:

    JobInstanceExecutionException(
        const std::string & message)
        :
        SchedulingException(message)
    {
    }

    JobInstanceExecutionException(const JobInstanceExecutionException & rhs)
        :
        SchedulingException(rhs)
    {
    }

    JobInstanceExecutionException()
        :
        SchedulingException()
    {
    }

    virtual ~JobInstanceExecutionException() throw () {}

    virtual const char *what() const throw ()
    {
        return SchedulingException::what();
    }

};


}; // TA_Base_App

#endif // JOB_INSTANCE_EXECUTION_EXCEPTION_H
