/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/TrainAlarms.cpp $
  * @author:  Ripple
  * @version: $Revision: #4 $
  *
  * Last modification: $DateTime: 2016/11/23 18:37:10 $
  * Last modified by:  $Author: Noel $
  *
  */
// TrainAlarms.cpp: implementation of the TrainAlarms class.
//
//////////////////////////////////////////////////////////////////////

 
#include "bus/alarm/alarm_list_control/src/TrainAlarms.h"
 
#include "bus/alarm/alarm_list_control/pmodel/src/TrainAgentAccess.h"


#include "bus/generic_gui_view/src/TransactiveMessage.h"
#include "bus/cctv/video_switch_agent/agent_access/src/VideoSwitchAgentFactory.h"
#include "bus/trains/TrainAgentCorba/idl/src/ITrainExceptionsCorba.h"

#include "core/data_access_interface/entity_access/src/TrainCctvCameraEntityData.h"
#include "core/data_access_interface/entity_access/src/tvssagententitydata.h"

#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/TransactiveException.h"
#include "core/exceptions/IDL/src/CommonExceptionsCorbaDef.h"

#include "core/utilities/src/RunParams.h"
#include "core\utilities\src\DebugUtil.h"

#include <QObject>

#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CCTV_ACTIVATE_CATCH_HANDLER()\
catch (const TA_Base_Core::TransactiveException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", ex.what() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << ex.what();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Core::OperationModeException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::OperationModeException", ex.reason.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << std::string( "Operation mode error" );\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Core::AccessDeniedException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::AccessDeniedException", ex.reason.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << std::string( "Access Denied" );\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::InvalidTrainException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::InvalidTrainException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::InvalidParameterException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::InvalidParameterException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::TransmissionFailureException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::TransmissionFailureException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::TransactionTimeoutException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::TransactionTimeoutException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::TransactionCancelledException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::TransactionCancelledException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::MpuChangeoverException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::MpuChangeoverException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::BadCommandException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::BadCommandException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::TrainCctvException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::TrainCctvException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_040042);\
}\
catch ( ... )\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "...", "..." );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_650019);\
}


#define CCTV_DEACTIVATE_CATCH_HANDLER( TrainId )\
catch (const TA_Base_Core::TransactiveException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", ex.what() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << ex.what();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( TA_Base_Core::OperationModeException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::OperationModeException", ex.reason.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << std::string( "Operation mode error" );\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( TA_Base_Core::AccessDeniedException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::AccessDeniedException", ex.reason.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << std::string( "Access Denied" );\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::InvalidTrainException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::InvalidTrainException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::InvalidParameterException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::InvalidParameterException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::TransmissionFailureException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::TransmissionFailureException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::TransactionTimeoutException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::TransactionTimeoutException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::TransactionCancelledException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::TransactionCancelledException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::MpuChangeoverException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::MpuChangeoverException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( TA_Base_Bus::ITrainExceptionsCorba::BadCommandException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::BadCommandException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
    catch ( TA_Base_Bus::ITrainExceptionsCorba::TrainCctvException& ex)\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "ITrainExceptionsCorba::TrainCctvException", ex.details.in() );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    userMsg << ex.details.in();\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\
catch ( ... )\
{\
    LOG_EXCEPTION_CATCH( SourceInfo, "...", "..." );\
    TA_Base_Bus::TransActiveMessage userMsg;\
    userMsg << TrainId;\
    UINT selectedButton = userMsg.showMsgBox(IDS_UE_090076);\
}\


namespace TA_Base_Bus
{
	TrainAlarms::TrainAlarms()// : m_locationSelectionDialog(0)
	{
        FUNCTION_ENTRY( "TrainAlarms" );
        FUNCTION_EXIT;
   	}


	TrainAlarms::~TrainAlarms()
	{
        FUNCTION_ENTRY( "~TrainAlarms" );     
 
       
        FUNCTION_EXIT;
	}

	TrainAlarms& TrainAlarms::getInstance()
	{
        FUNCTION_ENTRY( "getInstance" );
        
        FUNCTION_EXIT;
		return *( TrainAlarmsSingleton::instance() );   
	}

	void TrainAlarms::acknowledgeAlarm(TA_Base_Bus::AlarmDataItem* details)
    {
        FUNCTION_ENTRY( "acknowledgeAlarm" );
		
		//step 1 get the tvssagent corba
		std::string strTVSSAgentName = TA_Base_Core::TVSSAgentEntityData::getStaticType();
        TA_Base_Core::CorbaNameList names = TA_Base_Core::EntityAccessFactory::getInstance().getCorbaNamesOfTypeAtLocation(strTVSSAgentName, details->locationKey,true );
        
        //step 2 call interface to tvssAcknowledgeAlarm
        try
        {
			if (names.size() == 0)
			{
				char szMsg[255];
				TAUtfString format = TAUtfString(tr("Configuration error for entity name for %s at location %ld. Please contact your administrator."));
				sprintf(szMsg, format.std_str().c_str(), strTVSSAgentName.c_str(), details->locationKey);
				TA_THROW( TA_Base_Core::TransactiveException(szMsg));
			}
			m_tvssManagerNameObj.setCorbaName(names[0]);
			std::string sessionId = TA_Base_Core::RunParams::getInstance().get( RPARAM_SESSIONID );

			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "call tvssAcknowledgeAlarm,entitykey=%ul,sessionId=%s",details->assocEntityKey,sessionId.c_str());
            CORBA_CALL(m_tvssManagerNameObj,tvssAcknowledgeAlarm,(details->assocEntityKey,sessionId.c_str()));
        }
        catch (TA_Base_Bus::TVSSManagerCorbaDef::TVSSException& e)
        {
            // object resolution etc
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "TVSSException", e.what );
			TAUtfString caption = tr("TVSSException");
			MessageBox(NULL, e.what, caption.std_str().c_str(), MB_OK);
        }
        catch (TA_Base_Core::TransactiveException& te)
        {
            // object resolution etc
            LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "TransactiveException", te.what() );
			TAUtfString caption = tr("TransactiveException");
			MessageBox(NULL, te.what(), caption.std_str().c_str(), MB_OK);
        }
        catch( const CORBA::Exception& cex )
        {
            std::string exceptionMsg( "Exception thrown while trainBorneAcknowleageAlarm: " );
            exceptionMsg.append( TA_Base_Core::CorbaUtil::getInstance().exceptionToString( cex ) );

            LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "CORBA", exceptionMsg.c_str() );
			TAUtfString caption = tr("CORBA Exception");
			MessageBox(NULL, exceptionMsg.c_str(), caption.std_str().c_str(), MB_OK);
        }

        FUNCTION_EXIT;
    }
 

}
