/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution in any form.
  *
  * Source:    $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_agent/src/IJobInstance.h $
  * @author:   Dirk McCormick
  * @version:  $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This interface represents an instance of an executable job.
  *
  */



#ifndef IJOB_INSTANCE_H
#define IJOB_INSTANCE_H

#include <string>

namespace TA_Base_App
{

class IJobInstance
{

public:

    typedef enum JobInstanceState
    {
        NotStarted,
        Started,
        Completed
    };

    /**
     *  Gets the id of this job.
     */
    virtual std::string getJobId() const = 0;

    /**
     *  Gets the description of this job.
     */
    virtual std::string getDescription() const = 0;

    /**
     *  Gets the location id of this job.
     */
    virtual unsigned long getLocationId() const = 0;

    /**
     *  Gets the profile id of this job.
     */
    virtual unsigned long getProfileId() const = 0;

    /**
     *  Gets the job instance state of this job.
     */
    virtual int getJobInstanceState(unsigned long minuteStarted, std::string& errorMessage) const = 0;

    /**
     *  Returns the minute at which the job instance started.
     */
    virtual unsigned long getMinuteStarted() const = 0;

    /**
     *  Executes the job.
     *
     *  This method must execute the job, and return after the job has
     *  finished.
     *
     *  @exception JobInstanceExecutionException
     *             thrown if there is a problem executing the job
     */
    virtual void execute() = 0;


    /**
     *  Destructor.
     */
    virtual ~IJobInstance() {}
};


}; // TA_Base_App

#endif // IJOB_INSTANCE_H
