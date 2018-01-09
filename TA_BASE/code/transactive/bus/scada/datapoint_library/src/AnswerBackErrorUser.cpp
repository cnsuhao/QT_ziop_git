/**
  * The source code in this file is the property of Ripple Systems and is not for redistribution in
  * any form.
  *
  * Source:		$File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/scada/datapoint_library/src/AnswerBackErrorUser.cpp $
  * @author:	HoaVu
  * @version:	$Revision: #1 $
  *
  * Last modification:	$DateTime: 2015/10/09 15:42:35 $
  * Last modified by:	$Author: CM $
  *
  * Description:
  *
  */

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif // _MSC_VER

#include <sstream>

#include "bus/mathematical_evaluation/src/ExpressionEvaluator.h"
#include "bus/scada/datapoint_library/src/IAnswerBackErrorNotification.h"
#include "bus/scada/datapoint_library/src/RealNumberContext.h"
#include "bus/scada/datapoint_library/src/BooleanContext.h"
#include "bus/scada/datapoint_library/src/AnswerBackErrorUser.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/exceptions/src/MathematicalEvaluationException.h"
#include "core/timers/src/SingletonTimerUtil.h"

namespace TA_Base_Bus
{

	AnswerBackErrorUser::AnswerBackErrorUser ( IAnswerBackErrorNotification * user,
											   unsigned long abeTimeoutPeriodMSeconds,
											   const std::string & abeExpression )
	:
	m_clockHasStarted ( false ),
	m_abeTimeoutPeriodMSeconds ( abeTimeoutPeriodMSeconds ),
	m_abeExpression ( abeExpression ),
	m_abeUser ( 0 ),
	m_BusExpressionEvaluator ( 0 ),
	m_booleanContext ( 0 ),
	m_realNumberContext ( 0 )
	{
		// if the user is valid
		if ( 0 != user )
		{
			// save the ABE user for further reference
			m_abeUser = user;

			// create an instance of the expression evaluator (TA_Base_Bus library)
			m_BusExpressionEvaluator = new TA_Base_Bus::ExpressionEvaluator ( m_abeExpression );
			m_booleanContext = new BooleanContext ( this );
			m_realNumberContext = new RealNumberContext ( this );

			// add the context to the ExpressionEvaluator
			m_BusExpressionEvaluator->addBooleanContext ( m_booleanContext );
			m_BusExpressionEvaluator->addRealNumberContext ( m_realNumberContext );
		}
	}


	AnswerBackErrorUser::~AnswerBackErrorUser()
	{
		// do not delete reference to the callback user

		// deregister the ABE user with timeout clock		
		TA_Base_Core::SingletonTimerUtil::getInstance().stopPeriodicTimeOutClock(this);

		// delete expression evaluator stuffs
		if ( 0 != m_BusExpressionEvaluator )
		{
			delete m_BusExpressionEvaluator;
			m_BusExpressionEvaluator = 0;
		}

		if ( 0 != m_booleanContext )
		{
			delete m_booleanContext;
			m_booleanContext = 0;
		}

		if ( 0 != m_realNumberContext )
		{
			delete m_realNumberContext;
			m_realNumberContext = 0;
		}
	}


	void AnswerBackErrorUser::threadedTimerExpired(long timerId, void* userData)
	{
		//
		// ABE timeout clock has expired, evaluate the ABE expression
		//

		std::stringstream ss;
		ss << "Timeout clock for ABE expression condition (" << m_abeExpression << ") has expired ...Calling m_BusExpressionEvaluator->evaluate()";
		LOG( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, ss.str().c_str() );


		bool result = false;
		try
		{
			result = m_BusExpressionEvaluator->evaluateBooleanExpression();

			// if the status of the result is bad
			if ( false == m_booleanContext->isGoodCombinedStatus() )
			{
				// force the result to false, no matter what result have
				// come out of the expression evaluator
				result = false;

				std::stringstream ss1;
				ss1 << "Forced the result of the Return Expression " << m_abeExpression << " to FALSE because the dependencies have bad status";
				LOG( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, ss1.str().c_str() );
			}
			// else do nothing, accept the result from the expression evaluator
		}
		catch ( TA_Base_Core::MathematicalEvaluationException & )
		{
		}
		catch ( ... )
		{
		}

		try
		{
			// callback the ABE user for post evaluation execution
			m_abeUser->processPostEvaluationStep ( result );
		}
		catch ( ... )
		{
		}

		// stop the clock
		stopTiming();
	}


	void AnswerBackErrorUser::processExpressionChanges()
	{
		//
		// the ABE expression condition has changed due to online update
		// of input data points
		//

		// only proceed if the ABE timeout clock is still running
		if ( true == m_clockHasStarted )
		{
			// evaluate the ABE expression
			bool result = false;

			try
			{
				result = m_BusExpressionEvaluator->evaluateBooleanExpression();

				// if the status of the result is bad
				if ( false == m_booleanContext->isGoodCombinedStatus() )
				{
					// force the result to false, no matter what result have
					// come out of the expression evaluator
					result = false;

					std::stringstream ss1;
					ss1 << "Forced the result of the Return Expression " << m_abeExpression << " to FALSE because the dependencies have bad status";
					LOG( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, ss1.str().c_str() );
				}
				// else do nothing, accept the result from the expression evaluator
			}
			catch ( TA_Base_Core::MathematicalEvaluationException & )
			{
			}
			catch ( ... )
			{
			}

			// if the ABE condition has been evaluated successfully, otherwise
			// shall wait till the end of timeout period before notify the
			// user of the ABE condition result
			if ( true == result )
			{
				try
				{
					std::stringstream ss;
					ss << "ABE expression condition (" << m_abeExpression << ") has been evaluated successfully due to EntityUpdateEvent of input data point proxies...";
					LOG( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, ss.str().c_str() );

					// callback the ABE user for post evaluation execution
					m_abeUser->processPostEvaluationStep ( result );

					// stop the clock
					stopTiming();

					return;
				}
				catch ( ... )
				{
				}
			}
		}
	}


	void AnswerBackErrorUser::updateTimeoutPeriod ( unsigned long newTimeoutPeriod )
	{
		// only proceed if there is a difference
		if ( m_abeTimeoutPeriodMSeconds != newTimeoutPeriod )
		{
			m_abeTimeoutPeriodMSeconds = newTimeoutPeriod;

			// stop and restart the clock
			stopTiming();
			startTiming();
		}
	}


	void AnswerBackErrorUser::updateABEExpression ( const std::string & newExpression )
	{
		// only proceed if there is a difference
		if ( 0 != newExpression.compare ( m_abeExpression ) )
		{
			m_abeExpression = newExpression;

			if ( 0 != m_BusExpressionEvaluator )
			{
				m_BusExpressionEvaluator->setNewExpression ( m_abeExpression );
			}
		}
	}


	void AnswerBackErrorUser::startTiming()
	{
		// start the timeout clock allocated for this user
		TA_Base_Core::SingletonTimerUtil::getInstance().startPeriodicTimeOutClock ( this, m_abeTimeoutPeriodMSeconds );

		setTimeoutStartedFlag ( true );
		m_booleanContext->enableCallback ( true );
		m_realNumberContext->enableCallback ( true );

		// check if the condition has been met yet
		processExpressionChanges();
	}


	void AnswerBackErrorUser::stopTiming()
	{
		// start the timeout clock allocated for this user
		TA_Base_Core::SingletonTimerUtil::getInstance().stopPeriodicTimeOutClock ( this );

		setTimeoutStartedFlag ( false );
		m_booleanContext->enableCallback ( false );
		m_realNumberContext->enableCallback ( false );
	}


	bool AnswerBackErrorUser::getIsCallbackUserMatched ( IAnswerBackErrorNotification * user )
	{
		return ( ( m_abeUser == user ) ? true : false );
	}


	void AnswerBackErrorUser::setTimeoutStartedFlag ( bool started )
	{
		m_clockHasStarted = started;
	}

}
