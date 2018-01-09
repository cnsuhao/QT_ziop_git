/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/TrainAgentAccessException.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Derived TransActive Exception, adds detail and comment members, along with related
  * accesor methods.
  */

#include "TrainAgentAccessException.h"

using namespace TA_Base_App;


TrainAgentAccessException::TrainAgentAccessException() throw() :
    PlanManagerException()
{
}


TrainAgentAccessException::TrainAgentAccessException(std::string msg) throw() :
    PlanManagerException(msg)
{
}


TrainAgentAccessException::TrainAgentAccessException(std::string msg, std::string detail) throw() :
    PlanManagerException(msg, detail)
{
}


TrainAgentAccessException::TrainAgentAccessException(std::string msg, std::string detail, std::string comment) throw() :
    PlanManagerException(msg, detail, comment)
{
}


TrainAgentAccessException::~TrainAgentAccessException() throw()
{
}
