/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution in any form.
  *
  * Source:    $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_agent/src/JobFactoryInitialisationException.h $
  * @author:   Dirk McCormick
  * @version:  $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Thrown when there is a problem initialising an IJobFactory subclass.
  *
  */


#ifndef JOB_FACTORY_INITIALISATION_EXCEPTION_H
#define JOB_FACTORY_INITIALISATION_EXCEPTION_H

#include "SchedulingException.h"
#include "app/scheduling/scheduling_agent/src/JobRunner.h"

namespace TA_Base_App
{

class JobFactoryInitialisationException: public SchedulingException
{

public:

    JobFactoryInitialisationException(
        const std::string & message)
        :
        SchedulingException(message)
    {
    }

    JobFactoryInitialisationException(const JobFactoryInitialisationException & rhs)
        :
        SchedulingException(rhs)
    {
    }

    JobFactoryInitialisationException()
        :
        SchedulingException()
    {
    }

    virtual ~JobFactoryInitialisationException() {};

    virtual const char *what() const
    {
        return SchedulingException::what();
    }

};


}; // TA_Base_App

#endif // JOB_FACTORY_INITIALISATION_EXCEPTION_H
