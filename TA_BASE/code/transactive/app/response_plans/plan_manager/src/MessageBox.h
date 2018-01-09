/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/MessageBox.h $
* @author:  Bart Golab
* @version: $Revision: #3 $
*
* Last modification: $DateTime: 2017/05/22 15:28:33 $
* Last modified by:  $Author: hoa.le $
*
* This class implements custom MessageBox.
*/

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <qstring.h>

#define MessageBox MessageBox

namespace TA_Base_Core
{
    class TransactiveException;
    class RightsException;
    class PlanStepException;
    class PlanStepDetailException;
    class PlanAgentException;
}

class QString;

namespace TA_Base_App
{
    class PlanNotFoundException;


    /////////////////////////////////////////////////////////////////////////////
    // MessageBox utility

    class MessageBox
    {
    // Operations
    public:
		MessageBox();
		virtual ~MessageBox();
        /**
          * error
          *
          * Displays the error message based on the what's specified in the exception
          *
          * @param actionId The type of action that is being performed that caused the exception
          *                 to be thrown.
          * @param exception The exception that was thrown
          *
          */
        static void error(const char* actionStr, const TA_Base_Core::TransactiveException& exception);

        /**
          * error
          *
          * Displays the error message based on the what's specified in the exception
          *
          * @param actionId The type of action that is being performed that caused the exception
          *                 to be thrown.
          * @param actionParam Additional parameter to the action
          * @param exception The exception that was thrown
          *
          */
        static void error(const char* actionStr, const QString& actionParam, const TA_Base_Core::TransactiveException& exception);

        /**
          * error
          *
          * Displays the generic "Cannot do x because an error has occurred" message.
          *
          * @param actionId The type of action that is being performed
          */
        static void error(const char* actionStr);

    private:

        /**
          * displayError
          *
          * Displays the message caused by error. Determines error type from the exception.
          *
          * @param actionName The action that is being performed that caused the exception
          *                 to be thrown.
          * @param exception The exception that was thrown
          *
          * @return True if it has displayed the appropriate message
          *
          */
        static void displayError(const QString& actionName, const TA_Base_Core::TransactiveException& exception);

        /**
          * displayPermissionError
          *
          * Displays the message caused by permission error
          *
          * @param actionName The action that is being performed that caused the exception
          *                 to be thrown.
          * @param exception The exception that was thrown
          *
          * @return True if it has displayed the appropriate message
          *
          */
        static void displayPermissionError(const QString& actionName, const TA_Base_Core::RightsException& exception);


        /**
          * displayPlanNotFoundError
          *
          * Displays the message as a result of plan not being found
          *
          * @param actionName The action that is being performed that caused the exception
          *                 to be thrown.
          * @param exception The exception that was thrown
          *
          * @return True if it has displayed the appropriate message
          *
          */
        static void displayPlanNotFoundError(const QString& actionName, const PlanNotFoundException& exception);


        /**
          * displayPlanStepError
          *
          * Displays the message caused by invalid plan step
          *
          * @param actionName The action that is being performed that caused the exception
          *                 to be thrown.
          * @param exception The exception that was thrown
          *
          * @return True if it has displayed the appropriate message
          *
          */
        static void displayPlanStepError(const QString& actionName, const TA_Base_Core::PlanStepException& exception);


        /**
          * displayPlanStepDetailError
          *
          * Displays the message caused by step detail error
          *
          * @param actionName The action that is being performed that caused the exception
          *                 to be thrown.
          * @param exception The exception that was thrown
          *
          * @return True if it has displayed the appropriate message
          *
          */
        static void displayPlanStepDetailError(const QString& actionName, const TA_Base_Core::PlanStepDetailException& exception);


        /**
          * displayPlanAgentError
          *
          * Displays the message caused by a plan agent error that does not fall into above's category
          *
          * @param actionName The action that is being performed that caused the exception
          *                 to be thrown.
          * @param exception The exception that was thrown
          *
          * @return True if it has displayed the appropriate message
          *
          */
        static void displayPlanAgentError(const QString& actionName, const TA_Base_Core::PlanAgentException& exception);
    };

} // namespace TA_Base_App

#endif // MESSAGEBOX_H


