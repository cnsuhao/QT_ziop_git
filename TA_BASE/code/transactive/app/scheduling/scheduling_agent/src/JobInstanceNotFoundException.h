/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution in any form.
  *
  * Source:    $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_agent/src/JobInstanceNotFoundException.h $
  * @author:   Dirk McCormick
  * @version:  $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Thrown when a job instance cannot be found.
  *
  */


#ifndef JOB_INSTANCE_NOT_FOUND_EXCEPTION_H
#define JOB_INSTANCE_NOT_FOUND_EXCEPTION_H

#include "SchedulingException.h"

namespace TA_Base_App
{

class JobInstanceNotFoundException: public SchedulingException
{

public:

    JobInstanceNotFoundException( const std::string & message )
        :
        SchedulingException(message)
    {
    }

    JobInstanceNotFoundException()
        :
        SchedulingException()
    {
    }

    virtual ~JobInstanceNotFoundException() throw () {};

    virtual const char *what() const throw ()
    {
        return SchedulingException::what();
    }

};


}; // TA_Base_App

#endif // JOB_INSTANCE_NOT_FOUND_EXCEPTION_H
