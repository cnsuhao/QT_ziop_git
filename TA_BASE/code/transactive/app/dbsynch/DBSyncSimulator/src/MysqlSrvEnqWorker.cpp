#include "MysqlSrvEnqWorker.h"
#include "UtilityFun.h"

#include "IMsgItem.h"
#include "MsgItemImp.h"
#include "IMsgPackage.h"
#include "MsgPackageImp.h"
#include "TcpConnectionPool.h"

#include "SrvComWorker.h"
#include "IDbData.h"
#include "IDatabase.h"
#include "DatabaseImpl.h" 
#include "Producer.h"

#include "MsgHelper.h"
#include "Message.h"
#include "sqlmacrodef.h"
#include "DBException.h"
#include "core/utilities/src/DebugUtil.h"
using TA_Base_Core::DebugUtil;

NS_BEGIN(TA_DB_Sync)


CMysqlSrvEnqWorker::CMysqlSrvEnqWorker( CSrvComWorker* pComWorker,PAQWorkInfo pDeqEnqInfo, unsigned long ID)
:m_TcpRecvFramesSemaphore(0)
{	
	m_ID=ID;
	m_pComWorkerCopy =	pComWorker;
	m_toTerminate = false;
	m_pProducerDataBase = NULL;
	m_pDBForEnqCheck = NULL;
	m_pCServerProducer = NULL; 	

	m_nEnqWorkIndex = 0;
	m_nEnqTotal = 0;
	m_pMsgItemHelper = NULL;
	m_pMsgItemHelper = new CMsgItemHelper();
	m_strRcvEnqRequestUUID32.clear();
	m_nRcvEnqRequestCount = 0;
	m_nEnqInitOServerID = -1000;
	m_pCurrentRequestFrame = NULL;
	m_ulMsgSeqIDFirst = 0;
	m_ulMsgSeqIDLast = 0;
	m_nMsgOServerID = 0;
	_SetAgtInfo(pDeqEnqInfo);
	m_pProducerDataBase = new CDatabaseImpl(g_system_variables.m_SrcDbConStr);
	m_pDBForEnqCheck = new CDatabaseImpl(g_system_variables.m_SrcDbConStr);
	
	m_pMonitorEnqTime = NULL;
	m_pMonitorEnqTime = new CWorkTime(PROPAGATE_TIMER_LOG_MonitorEnqTime);
	m_pTimer = new CTimeMonitor();
}



CMysqlSrvEnqWorker::~CMysqlSrvEnqWorker()
{
	terminateAndWait();
	m_pComWorkerCopy = NULL;
	 
	//free Enq handle	
	_UnInitEnqWork(); 
							
	if (NULL != m_pCurrentRequestFrame)
	{
		_FreeEnqReqEx(&m_pCurrentRequestFrame);
		m_pCurrentRequestFrame = NULL;
	}
	_FreeLstRecvMsgPackage();
	DEF_DELETE(m_pMsgItemHelper);
	DEF_DELETE(m_pMonitorEnqTime);
	DEF_DELETE(m_pTimer);

}


int	CMysqlSrvEnqWorker::_SetAgtInfo(PAQWorkInfo pDeqEnqInfo)
{
	int nFunRes = 0;

	m_strQName = pDeqEnqInfo->m_strQName;
	m_strClientAgtName = pDeqEnqInfo->m_strClientAgtName;
	m_strServerAgtName = pDeqEnqInfo->m_strServerAgtName;
	m_nEnqInitOServerID = pDeqEnqInfo->m_nINSRVID;
	m_strClientID = pDeqEnqInfo->m_strClientID;					

	LOG3(SourceInfo, DebugUtil::DebugInfo, 
		"_SetAgtInfo [ClientID=%s][ServerAgtName=%s][INSRVID=%d]", 
		m_strClientID.c_str(), m_strServerAgtName.c_str(), m_nEnqInitOServerID);
				   
	return nFunRes;
}


int	CMysqlSrvEnqWorker::_FreeEnqReqEx(ENQREQEX** ppEnqReqEx)
{
	int nFunRes = 0;

	if (NULL != ppEnqReqEx && NULL != (*ppEnqReqEx))
	{
		_FreeMsgVec((*ppEnqReqEx)->m_lstDBMsg);
		DEF_DELETE(*ppEnqReqEx);
	}
	return nFunRes;
}


int CMysqlSrvEnqWorker::_FreeLstRecvMsgPackage()
{
	TA_Base_Core::ThreadGuard guard(m_lockLstTcpRecvFrames);
	lstEnqReqExConIterT iterLstFrame;
	ENQREQEX* pRequestFrame = NULL;
	int nFunRes = 0;  

	if (m_LstTcpRecvFrames.empty())
	{
		return nFunRes;
	}

	iterLstFrame = m_LstTcpRecvFrames.begin();
	while (iterLstFrame != m_LstTcpRecvFrames.end())
	{
		pRequestFrame = (*iterLstFrame); 
		m_LstTcpRecvFrames.erase(iterLstFrame);
		_FreeEnqReqEx(&pRequestFrame);
		pRequestFrame = NULL;
		iterLstFrame = m_LstTcpRecvFrames.begin();

	} 	

	return nFunRes;
}




void  CMysqlSrvEnqWorker::_FreeMsgVec(lstDBMsgConT& rVecMsg)
{
	lstDBMsgConIterT  iter = rVecMsg.begin();
	TA_AQ_Lib::CMessage* pGet = NULL;

	while ( iter != rVecMsg.end() )
	{
		pGet = *iter;
		DEF_DELETE(pGet);
		iter++;		
	}
	rVecMsg.clear();
}



int CMysqlSrvEnqWorker::_InitEnqWork()
{
	int nFunRes = 0;
	std::string strSrcDbConStr = g_system_variables.m_SrcDbConStr; 

	if (strSrcDbConStr.empty())
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "[%s][DbConStr=%s]error! _InitEnqWork()", 
			m_strClientID.c_str(), strSrcDbConStr.c_str());			
		nFunRes = -1;
		return nFunRes;
	}

	try
	{
		if (NULL == m_pDBForEnqCheck)
		{
			m_pDBForEnqCheck = new CDatabaseImpl(strSrcDbConStr.c_str());
		}
		m_pDBForEnqCheck->connect();
			
		DEF_DELETE(m_pCServerProducer);
		if (NULL == m_pProducerDataBase)
		{
			m_pProducerDataBase = new CDatabaseImpl(strSrcDbConStr.c_str());
		}
		m_pProducerDataBase->connect();	

		m_pCServerProducer = new CProducer(m_pProducerDataBase);
	}
	catch (...)
	{
		nFunRes = -1;
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, 	"err: _InitEnqDeqWork have error! [%s][DbConStr=%s]", 
			m_strClientID.c_str(), strSrcDbConStr.c_str());

	}

	m_nEnqWorkIndex = 0;
	return nFunRes;
}


int CMysqlSrvEnqWorker::_UnInitEnqWork()
{	 
	int nRes = 0;

	LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug,"begin CMysqlSrvEnqWorker::_UnInitEnqWork()"); 

	DEF_DELETE(m_pCServerProducer);
	DEF_DELETE(m_pProducerDataBase);
	DEF_DELETE(m_pDBForEnqCheck);

	LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug,"end CMysqlSrvEnqWorker::_UnInitEnqWork()"); 

	return nRes;
}



void CMysqlSrvEnqWorker::_CheckIntValue()
{
	if (m_nEnqWorkIndex > SERVER_LIMIT_LOG_VLAUE_MAX )
	{
		m_nEnqWorkIndex = 0;
	}

	if (m_nEnqTotal > SERVER_LIMIT_LOG_VLAUE_MAX )
	{
		m_nEnqTotal = 0;
	}

	
}


int   CMysqlSrvEnqWorker::_ServerAffairsSendReply(PENQACK pEnqACK)
{
	int nSendRes = 0;	
	MSGPKG* pGetMsgPkg = NULL;

	m_pMsgItemHelper->setSTOCEnqACK(pEnqACK, pGetMsgPkg);	

	if (NULL != m_pComWorkerCopy)
	{
		nSendRes = m_pComWorkerCopy->serverSendMsgPkg(pGetMsgPkg);
	} 
	pGetMsgPkg = NULL;
	return nSendRes;
}


void CMysqlSrvEnqWorker::terminate()
{		
	LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "CMysqlSrvEnqWorker::terminate.");
	m_toTerminate = true;

	LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "CMysqlSrvEnqWorker::terminate. m_TcpRecvFramesSemaphore.post()");
	m_TcpRecvFramesSemaphore.post();

}



bool CMysqlSrvEnqWorker::serverTerminateAndWait(int nMode)
{
	LOG1(SourceInfo, DebugUtil::DebugDebug, 
		"begin CMysqlSrvEnqWorker::serverTerminateAndWait() nMode=%d", nMode); 
		
	terminateAndWait();
	
	LOG1(SourceInfo, DebugUtil::DebugDebug, 
		"end CMysqlSrvEnqWorker::serverTerminateAndWait() nMode=%d", nMode); 

	return true;
}

bool CMysqlSrvEnqWorker::serverStart()
{
	start();
	return true;
}



int	CMysqlSrvEnqWorker::_ProceesCtoSEnqReq(ENQREQEX* pEnqReq)
{ 
	TA_Base_Core::ThreadGuard guard(m_lockLstTcpRecvFrames);
	int					nFunRes = 0;

	m_LstTcpRecvFrames.push_back(pEnqReq);

	LOG3(SourceInfo, DebugUtil::DebugDebug,"[%s][EnqWorkIndex=%d][TcpRecvFrames.size=%d]", 
		m_strClientID.c_str(), m_nEnqWorkIndex, m_LstTcpRecvFrames.size());


	m_TcpRecvFramesSemaphore.post();

    return nFunRes;
}

int	CMysqlSrvEnqWorker::_CheckEnqRequest(ENQREQEX* pRcvFrame)
{
	int					nFunRes = 0;

	if (NULL == pRcvFrame)
	{
		nFunRes = -1;
		return nFunRes;
	}

	if (pRcvFrame->m_lstDBMsg.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}
	
	return nFunRes;
}



void   CMysqlSrvEnqWorker::_Write2ExceptionQ(CMessage* pMsg)
{

	std::string strMsgID;
	std::string strData;
	ulong64 ulSID = 0;
	ulong64 nPriority = 1;
	char* pEscapedPayload = NULL;
	
	if (!m_pProducerDataBase) 
	{
		return;
	}
	
	ulSID = pMsg->getMsgSeqID();
	strMsgID = pMsg->getMsgID();
	pMsg->getPayload(strData);


	pEscapedPayload = CUtilityFun::escapeSQLStr((char*)strData.c_str(), "'", "''");
	strData = pEscapedPayload; 
	free(pEscapedPayload); 
	pEscapedPayload = 0;

	// insert into aq_sys_queue_e(q_name, msg_sid, msgid, priority, user_data) values ('%s', %s, '%s', %s, '%s'); 
	std::string strSql = m_pProducerDataBase->prepareSQLStatement(ENQ_SQL_INSERT_ADD_MSGSID_7003,  
						m_strQName, ulSID, strMsgID, nPriority, strData);

	try
	{
		m_pProducerDataBase->connect();
		m_pProducerDataBase->executeModification(strSql);
	}
	catch (const CDBException& e)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
			"Database exception, Error Code: %d; Message: %s", e.getNativeCode(), e.getReason().c_str() );
	}	
	catch (...)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
			"Error Code: %d; Message: %s", Err_Unknown, defErrMsg(Err_Unknown) );
	}
}

int	CMysqlSrvEnqWorker::_EnqToDB(ENQREQEX* pRcvFrame, ulong64 uLLastMsgSID)
{
	int					nFunRes = 0;
	int					nEnqMsgCount = 0;
	lstDBMsgConIterT    iterDBMsg;
	CMessage*			pMessage = NULL;
	ulong64				uMsgSIDGet = 0;
	ulong64				uMsgSIDForInsert = 0;
	mapSIDMIDConIterT   iterMapSIDMID;
	ulong64				uMsgSIDInMapForCheck = 0;
	std::string         strGetRecipientList;
	std::string			strGetMSGID;
		
	iterDBMsg = pRcvFrame->m_lstDBMsg.begin();
	nEnqMsgCount = 0;
	pMessage = *iterDBMsg;
	m_ulMsgSeqIDFirst = pMessage->getMsgSeqID();

	while (iterDBMsg != pRcvFrame->m_lstDBMsg.end() && !m_toTerminate)
	{
		pMessage = NULL;
		pMessage = *iterDBMsg;
		uMsgSIDGet = pMessage->getMsgSeqID();
		m_nMsgOServerID = pMessage->getOServerID();

		iterMapSIDMID = m_mapSIDMIDForEnq.find(m_nMsgOServerID);
		if (iterMapSIDMID == m_mapSIDMIDForEnq.end())
		{
			uMsgSIDForInsert = 0;
			//insert one record	to db   //TODO.
			nFunRes = _EnqInsert(m_pDBForEnqCheck, m_strClientAgtName, m_nMsgOServerID, m_strQName, uMsgSIDForInsert);
			if (0 != nFunRes)
			{
				LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Err: insert one record	to Oracle table AQ_ENQ_DUPLICATE_CHECK");
				nFunRes = -1;
				return nFunRes;
			}
			//insert client info to	 m_mapSIDMIDForEnq
			m_mapSIDMIDForEnq.insert(mapSIDMIDValueT(m_nMsgOServerID, uMsgSIDForInsert));
			LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Oracle table AQ_ENQ_DUPLICATE_CHECK not have [SRV_ID=%d]", m_nMsgOServerID);
			_Write2ExceptionQ(pMessage);		
			continue;
		}//if (iterMapSIDMID

		uMsgSIDInMapForCheck = iterMapSIDMID->second; 	
		if (uMsgSIDGet <= uMsgSIDInMapForCheck)
		{
			strGetMSGID = pMessage->getMsgID();

			LOG3(SourceInfo, DebugUtil::DebugError,
				"[MsgSIDGet=%llu] <= [MsgSIDInMapForCheck=%llu] [MSGID=%s]", 
				uMsgSIDGet, uMsgSIDInMapForCheck, strGetMSGID.c_str());

			_Write2ExceptionQ(pMessage); 				
			iterDBMsg++;
			continue;
		}
		 
		
		if (false == m_pComWorkerCopy->isTCPConnected())
		{
			LOG2(SourceInfo, DebugUtil::DebugError,	"[%s][EnqWorkIndex=%d] lost socket",
				m_strClientID.c_str(), m_nEnqWorkIndex);
			nFunRes = -1;
			return nFunRes;
		}		 

		//enq data to Mysql
		nFunRes = m_pCServerProducer->enqueueMsg(pMessage);	 
		if (Ret_Success == nFunRes)
		{
			iterMapSIDMID->second = uMsgSIDGet;
			m_ulMsgSeqIDLast = uMsgSIDGet;
			iterDBMsg++;
			nEnqMsgCount++;	
		}
		else	
		{
			switch ( nFunRes )
			{
			case ER_MYSQL_GONE_AWAY:
				LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "ER_MYSQL_GONE_AWAY(2006) == EnqRes");
				TA_Base_Core::Thread::sleep(TIME_BASE_MIN);
				_UnInitEnqWork();
				_InitEnqWork();
				nFunRes = -1;
				return nFunRes;	
			case ER_MYSQL_READ_ONLY:
				sleep(TIME_SECOND_FIVE);
				LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "ER_MYSQL_READ_ONLY(1290) == EnqRes");
				g_sys_status_mask.setCodeFlag(STATE_CODE_RUNNING_MODE);
				nFunRes = -1;
				return nFunRes;	
			default:
				LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Warn! [EnqRes=%d]", nFunRes);
				sleep(TIME_BASE_MIN);
				break;
			}				
		} 
		
		if (TA_Base_Core::DebugUtil::DebugMsg == g_system_variables.m_LogLevel )
		{
			std::string strMsgData;
			pMessage->getPayload(strMsgData);
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugMsg,
				"[Message ID=%s][strMsgData:", pMessage->getMsgID().c_str());
			LOGLARGESTRING(SourceInfo, TA_Base_Core::DebugUtil::DebugMsg, strMsgData);
		}				
				

	}//while (iter != vecMsg.end())	
	
	
	return nFunRes;
}



int	CMysqlSrvEnqWorker::_EnqUpdate(IDatabase* pINDataBase, std::string& strINLocationKey, 
    int& nINServerID, std::string& strINQueueName, mapSIDMIDConT& mapINSIDMID)
{

	int					nFunRes = 0;
	std::string			strOracleSQL;
	mapSIDMIDConIterT   iterMap;

	if (mapINSIDMID.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}

	iterMap = mapINSIDMID.begin();
	while (iterMap != mapINSIDMID.end())
	{	
		//update AQ_ENQ_DUPLICATE_CHECK set MSG_SID=%s where UPPER(LOCATION)=UPPER('%s') and SRV_ID=%s and UPPER(Q_NAME)=UPPER('%s')
		strOracleSQL = pINDataBase->prepareSQLStatement(ENQ_SQL_UPDATE_SET_NEWMSGSID_7001,
			iterMap->second, strINLocationKey, iterMap->first, strINQueueName);

		try
		{
			pINDataBase->connect();
			pINDataBase->executeModification(strOracleSQL);
		}
		catch (const CDBException& e)
		{
			nFunRes = -1;

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
				"Database exception, Error Code: %d; Message: %s", e.getNativeCode(), e.getReason().c_str() );
			if (ER_MYSQL_GONE_AWAY == e.getNativeCode())
			{
				sleep(TIME_SECOND_TWO);
				_UnInitEnqWork();
				_InitEnqWork();
			}
		}	
		catch (...)
		{
			nFunRes = -1;

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
				"Error Code: %d; Message: %s", Err_Unknown, defErrMsg(Err_Unknown) );	
			sleep(TIME_SECOND_TWO);
			_UnInitEnqWork();
			_InitEnqWork();
		}
	
		iterMap++;
	} 	

	return nFunRes;
}
int	CMysqlSrvEnqWorker::_EnqInsert(IDatabase* pINDataBase, std::string& strINLocationKey, 
    int& nINServerID, std::string& strINQueueName, ulong64 nINMsgSID)
{

	int					nFunRes = 0;
	std::string			strOracleSQL;
	
	//insert into AQ_ENQ_DUPLICATE_CHECK(LOCATION, SRV_ID, Q_NAME, MSG_SID) values(UPPER('%s'), %s, UPPER('%s'), %s)
	strOracleSQL = pINDataBase->prepareSQLStatement(ENQ_SQL_INSERT_ADD_MSGSID_7004,
		strINLocationKey, nINServerID, strINQueueName, nINMsgSID);

	try
	{
		pINDataBase->connect();
		pINDataBase->executeModification(strOracleSQL);
		//not commit here
	}
	catch (...)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError,"insert AQ_ENQ_DUPLICATE_CHECK error! [strSQL=%s]", strOracleSQL.c_str());
		nFunRes = -1;
		return nFunRes;
	}


	return nFunRes;
}

int	CMysqlSrvEnqWorker::_EnqSelectForUpdate(IDatabase* pINDataBase, 
    std::string& strINLocationKey, int& nINServerID, std::string& strINQueueName, 
    mapSIDMIDConT& mapOUTSIDMID)
{  
	int					nFunRes = 0;
	std::string			strOracleSQL;
	std::vector<std::string> columnNames;
	IDbData*			pData = NULL;
	unsigned long       nIndex = 0;
	unsigned long       nRowsCount = 0;	
	int					nGetServerIDValue = -1000;
	ulong64				nGetMsgSIDValue = 0;

	//check	 pINDataBase is not NULL
	if (NULL == pINDataBase)
	{
		nFunRes = -1;
		return nFunRes;
	}

	//select SRV_ID,MSG_SID from AQ_ENQ_DUPLICATE_CHECK where UPPER(LOCATION)=UPPER('%s') and UPPER(Q_NAME)=UPPER('%s') for update
	strOracleSQL = pINDataBase->prepareSQLStatement(ENQ_SQL_SUPDATE_GET_MSGSID_7000, 
		strINLocationKey.c_str(), strINQueueName.c_str());


	columnNames.clear();
	columnNames.push_back("SRV_ID");
	columnNames.push_back("MSG_SID");
	try
	{
		pINDataBase->connect();
		pINDataBase->beginTransaction();
		pData = pINDataBase->executeQuery(strOracleSQL, columnNames);
		//not commit here
	} 
	catch (const CDBException& e)
	{
		nFunRes = -1;

 		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
			"Database exception, Error Code: %d; Message: %s", e.getNativeCode(), e.getReason().c_str() );
		if (ER_MYSQL_GONE_AWAY == e.getNativeCode())
		{
			sleep(TIME_SECOND_TWO);
			_UnInitEnqWork();
			_InitEnqWork();
		}
	}	
	catch (...)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, 	"cannot access duplicated check table [SQL=%s]", strOracleSQL.c_str());
		nFunRes = -1;
		DEF_DELETE(pData);
		_UnInitEnqWork();
		_InitEnqWork();	
		return nFunRes;
	}
	
	
	if (NULL == pData && pData->getNumRows() <= 0)
	{	
		nFunRes = Err_EXESQLAckCode_NotGetData;
		DEF_DELETE(pData);
		return nFunRes;
	}

	mapOUTSIDMID.clear();	

	nRowsCount = pData->getNumRows();
	for (nIndex = 0; nIndex < nRowsCount; nIndex++)
	{ 	
		//get data
		nGetServerIDValue = pData->getUnsignedLongData(nIndex, columnNames[0]);
		nGetMsgSIDValue = pData->getUnsignedLongData(nIndex, columnNames[1]);
		mapOUTSIDMID.insert(mapSIDMIDValueT(nGetServerIDValue, nGetMsgSIDValue));
		LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "[mapSIDMIDForEnq.size()=%d]", 
			mapOUTSIDMID.size());	
		nFunRes = 0; 

	}//for
	
	
	DEF_DELETE(pData);
	return nFunRes;
} 


int CMysqlSrvEnqWorker::processRcvFrame(ENQREQEX* pEnqReq)
{
	int nFunRes = 0;
	_ProceesCtoSEnqReq(pEnqReq);
	return nFunRes;
}



int CMysqlSrvEnqWorker::processTcpEvent( int nStateCode )
{
	int nFunRes = 0;
	return nFunRes;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void CMysqlSrvEnqWorker::run()
{
	LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, " CMysqlSrvEnqWorker  begin run()");
	
	m_nEnqWorkState = server_state_enq_EnqTokenCheck;
	bool receivedMsg=false;
	while( false == m_toTerminate ) 
	{ 		 
		_CheckIntValue(); 
		switch (m_nEnqWorkState)
		{
		case server_state_enq_EnqTokenCheck:
			_ServerPocessTokenCheck(); 
			break;
		case server_state_enq_InitEnq:
			_ServerPocessEnqInitEnq(); 
			break;
		case server_state_enq_WaitForRequest:	
			// wait
			{
				TA_Base_Core::ThreadGuard guard(m_lockLstTcpRecvFrames);
				if (!m_LstTcpRecvFrames.empty())
				{
					receivedMsg = true;
				}
				else
				{
					receivedMsg = false;
				}
			}
			if (false == m_toTerminate && false == receivedMsg)	
			{				 
				m_TcpRecvFramesSemaphore.wait();
			}
			m_nEnqWorkState = server_state_enq_RecvRequestEnq;
			break;
		case server_state_enq_RecvRequestEnq:
			_ServerHandleEnqMessage();  //enqueue , commit and send reply			 
			break;		 
		case server_state_enq_FinishWork:
			//Enq job stopped, so notify TcpConnectionActor for closing client connection;
			_updateActorEnqueueStatusBad(); 
			m_toTerminate = true;
			break;
		default:
			break;	
		}
		//TA_Base_Core::Thread::sleep( m_sleepTime );
	}//while

	_UnInitEnqWork(); 
	LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "CMysqlSrvEnqWorker  end run()");

} 

int CMysqlSrvEnqWorker::_ServerPocessEnqInitEnq()
{
	int nFunRes = 0;

	LOG2(SourceInfo, DebugUtil::DebugInfo,  "Init Enq [%s][EnqWorkIndex=%d] ", 	m_strClientID.c_str(), m_nEnqWorkIndex);

	nFunRes = _InitEnqWork();
	if (0 == nFunRes)
	{
		m_nEnqWorkState =server_state_enq_WaitForRequest;//   server_state_enq_EngMsgs;
		LOG2(SourceInfo, DebugUtil::DebugInfo,  "Init Enq successfully,[%s][EnqWorkIndex=%d] ", 	m_strClientID.c_str(), m_nEnqWorkIndex);
	}
	else
	{
		m_nEnqWorkState = server_state_enq_FinishWork;
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "[%s][EnqWorkIndex=%d] cannot init queue", 	m_strClientID.c_str(), m_nEnqWorkIndex);

	}
	return nFunRes;
}


int CMysqlSrvEnqWorker::_ServerHandleEnqMessage() //it is better to return a status flag
{
	int nFunRes = 0;
	
	//MonitorEnqTime begin time here
	m_pMonitorEnqTime->workBegin();

	//get an enqueue message;
	{  //only hold lock when retriving data.
		TA_Base_Core::ThreadGuard guard(m_lockLstTcpRecvFrames);
		lstEnqReqExConIterT iterLstFrame;
		if (!m_LstTcpRecvFrames.empty())		 
		{
			iterLstFrame = m_LstTcpRecvFrames.begin();
			if (iterLstFrame != m_LstTcpRecvFrames.end())
			{
				m_pCurrentRequestFrame = (*iterLstFrame);					 			 
				m_LstTcpRecvFrames.erase(iterLstFrame);	
			}
		}
	}

	//status can be only return by the first level functions;
	if (NULL == m_pCurrentRequestFrame)
	{
		//MonitorEnqTime end time here
		m_pMonitorEnqTime->workEnd();

		m_nEnqWorkState = server_state_enq_WaitForRequest;
		nFunRes = -1;				
		return nFunRes;
	}
	LOG2(SourceInfo, DebugUtil::DebugDebug, "Received Enq message [%s][EnqWorkIndex=%d]", m_strClientID.c_str(), m_nEnqWorkIndex);


	//Enqueue Data
	nFunRes = _ServerPocessEnqData(m_pCurrentRequestFrame);		
	if (0 != nFunRes)
	{
		//MonitorEnqTime end time here
		m_pMonitorEnqTime->workEnd();

		m_nEnqWorkState = server_state_enq_FinishWork;
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "[%s][EnqWorkIndex=%d] cannot enqueue msg to DB", 
			m_strClientID.c_str(), m_nEnqWorkIndex);
		nFunRes = -1;
		return nFunRes;
	}		     

	//Commit;
	nFunRes = _ServerPocessCommit();
	if (0 != nFunRes)
	{
		//MonitorEnqTime end time here
		m_pMonitorEnqTime->workEnd();

		m_nEnqWorkState =server_state_enq_FinishWork;
		nFunRes = -1;
		return nFunRes;
	}     

	//Send Reply  
	nFunRes =_ServerPocessSendReply();
	if (0 != nFunRes)
	{	  
		m_nEnqWorkState = server_state_enq_FinishWork;
		nFunRes = -1;
	}
	else
	{	 
		m_nEnqWorkState = server_state_enq_WaitForRequest;
	} 
	
	//MonitorEnqTime end time here
	m_pMonitorEnqTime->workEnd();

	return nFunRes;
}





int	CMysqlSrvEnqWorker::_ServerPocessEnqData(ENQREQEX* pRcvFrame)
{
	int					nFunRes = 0;
	ulong64				uLLastMsgSID = 0;
	lstDBMsgConIterT    iterDBMsg;
	CMessage*			pMsgGet = NULL; 
	std::string			strQName;
	int                 nServerID = 0;
	uint32             nEnqEnqWorkTime;

	if (0 != _CheckEnqRequest(pRcvFrame))
	{
		nFunRes = -1;
		return nFunRes;
	}
	
	//lock records in AQ_ENQ_DUPLICATE_CHECK table before EnqData to AQTable

	iterDBMsg = pRcvFrame->m_lstDBMsg.begin();
	pMsgGet = *iterDBMsg;

	strQName = pMsgGet->getQName();
	//TODO : pRcvFrame->m_lstDBMsg have many Msg, each Msg have OServerID, they are same now
	nServerID = pMsgGet->getOServerID();
	uLLastMsgSID = 0;

	//we no just get one OServerID, so we just can lock one record in AQ_ENQ_DUPLICATE_CHECK table
	//but pRcvFrame->m_lstDBMsg have many Msg, each Msg have OServerID, they my be not same 
	 
	//get duplicated check token, select * from XXX for update.
	nFunRes=_EnqSelectForUpdate(m_pDBForEnqCheck, m_strClientAgtName, nServerID, strQName, m_mapSIDMIDForEnq);
	if (Err_EXESQLAckCode_NotGetData == nFunRes)
	{
		//select * from XXX for update. not data 
		//then insert into AQ_ENQ_DUPLICATE_CHECK 
		nFunRes = _EnqInsert(m_pDBForEnqCheck, m_strClientAgtName, m_nEnqInitOServerID, m_strQName, 0);
		if (0 != nFunRes)
		{
			LOG2(SourceInfo, DebugUtil::DebugError, "insert oracle table AQ_ENQ_DUPLICATE_CHECK error! [%s][EnqWorkIndex=%d]", 
				m_strClientID.c_str(), m_nEnqWorkIndex); 
			return nFunRes;
		}
		m_pDBForEnqCheck->commitTransaction();
		//get duplicated check token, select * from XXX for update. again
		nFunRes = _EnqSelectForUpdate(m_pDBForEnqCheck, m_strClientAgtName, nServerID, strQName, m_mapSIDMIDForEnq);	
	}
		
	//check lock records in AQ_ENQ_DUPLICATE_CHECK table before EnqData to AQTable   Res
	if (0 != nFunRes)
	{
		LOG2(SourceInfo, DebugUtil::DebugError, "[%s][EnqWorkIndex=%d]_OracleEnqSelectForUpdate() error!", 
			m_strClientID.c_str(), m_nEnqWorkIndex);
		return nFunRes;
	}	

	//check m_mapSIDMIDForEnq
	if (m_mapSIDMIDForEnq.empty())
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, 	"m_mapSIDMIDForEnq is empty");
		nFunRes = -1;  
		return nFunRes;
	}
	
	//lock records in AQ_ENQ_DUPLICATE_CHECK table succeed then EnqData to AQTable   
	//get EnqRequest Frame  UUID32	
	m_strRcvEnqRequestUUID32 = pRcvFrame->m_strPkgID;//->getMsgFramePUUID();
	//get EnqRequest Frame Msgs Count
	m_nRcvEnqRequestCount  = (int)pRcvFrame->m_lstDBMsg.size();	 

	LOG4(SourceInfo, DebugUtil::DebugDebug, "Enq to DB, [%s][EnqWorkIndex=%d][PkgID=%s][Msg.size()=%d]", 
		m_strClientID.c_str(), m_nEnqWorkIndex, m_strRcvEnqRequestUUID32.c_str(), m_nRcvEnqRequestCount);

	//enq data begin
	m_pTimer->getTimeDiffByMilSecond(PROPAGATE_TIMER_LOG_ENQDATA);		 	

	nFunRes = _EnqToDB(pRcvFrame, uLLastMsgSID);
	if (0 != nFunRes)
	{
		LOG4(SourceInfo, DebugUtil::DebugError, "Enq to DB error, [%s][EnqWorkIndex=%d][PkgID=%s][Msg.size()=%d]", 
			m_strClientID.c_str(), m_nEnqWorkIndex, m_strRcvEnqRequestUUID32.c_str(), m_nRcvEnqRequestCount);
	}
	else
	{
		//enq data end
		nEnqEnqWorkTime = m_pTimer->getTimeDiffByMilSecond(PROPAGATE_TIMER_LOG_ENQDATA);
		m_pTimer->reset(PROPAGATE_TIMER_LOG_ENQDATA);		 	
	
		LOG4(SourceInfo, DebugUtil::DebugDebug, "Enq to DB OK, [%s][EnqWorkIndex=%d][Msg.size=%d][StepEnqDataTime=%d ms]", 
			m_strClientID.c_str(), m_nEnqWorkIndex, m_nRcvEnqRequestCount, nEnqEnqWorkTime);
	} 

	return nFunRes;
}





int CMysqlSrvEnqWorker::_ServerPocessCommit()
{
	int nFunRes = 0;		 
	std::string strQName;
	uint32 nEnqComitWorkTime;

	if (NULL == m_pProducerDataBase)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "error! [NULL == m_pCServerProducer]");
		nFunRes = -1;
		return nFunRes;
	}

	LOG2(SourceInfo, DebugUtil::DebugDebug, 	"commit, [%s][EnqWorkIndex=%d]", 
		m_strClientID.c_str(), m_nEnqWorkIndex);

	//get ComitWork start time
	m_pTimer->getTimeDiffByMilSecond(PROPAGATE_TIMER_LOG_ENQCOMMIT);		 	

    
	//update MSG_sid in AQ_ENQ_DUPLICATE_CHECK
	nFunRes = _EnqUpdate(m_pDBForEnqCheck, m_strClientAgtName, m_nMsgOServerID, m_strQName, m_mapSIDMIDForEnq);
	if (0 != nFunRes)
	{			
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, 	"err! update MSG_sid in AQ_ENQ_DUPLICATE_CHECK [%s][EnqWorkIndex=%d]", 
						m_strClientID.c_str(), m_nEnqWorkIndex);
		nFunRes = -1;
		return nFunRes;
	}	
	
	try
	{
		m_pDBForEnqCheck->commitTransaction();
	}
	catch (...)
	{
		nFunRes = -1;
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, 	"err! m_pDBForEnqCheck->commitTransaction [%s][EnqWorkIndex=%d]", 
			m_strClientID.c_str(), m_nEnqWorkIndex);

	}

	if (0 != nFunRes)
	{			
		_UnInitEnqWork();
		_InitEnqWork();		
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, 	"[%s][EnqWorkIndex=%d][cannot enqueue]", m_strClientID.c_str(), m_nEnqWorkIndex);
		nFunRes = -1;
		return nFunRes;
	}	    

	//calculate ComitWork time
	nEnqComitWorkTime = m_pTimer->getTimeDiffByMilSecond(PROPAGATE_TIMER_LOG_ENQCOMMIT);
	m_pTimer->reset(PROPAGATE_TIMER_LOG_ENQCOMMIT);		 	
		 	

	LOG6(SourceInfo, DebugUtil::DebugDebug, 
		"[%s][EnqWorkIndex=%d][FirstSeqID=%llu][LastSeqID=%llu][Msg.size=%d][StepEnqCommitTime=%d ms]", 
		m_strClientID.c_str(), m_nEnqWorkIndex, m_ulMsgSeqIDFirst, 
		m_ulMsgSeqIDLast, m_nRcvEnqRequestCount, nEnqComitWorkTime);


	m_ulMsgSeqIDFirst = 0;
	m_ulMsgSeqIDLast = 0;


	return nFunRes;
}

int CMysqlSrvEnqWorker::_ServerPocessSendReply()
{
	int		nFunRes = 0;
	int		nSendRes = -1;		
	PENQACK pEnqAck = NULL;


	LOG3(SourceInfo, DebugUtil::DebugDebug, "Send Reply begin,[%s][EnqWorkIndex=%d][RcvEnqRequestUUID32=%s] ",	
		m_strClientID.c_str(), m_nEnqWorkIndex,m_strRcvEnqRequestUUID32.c_str());
  	
	pEnqAck = new ENQACK();
	pEnqAck->m_nEnqReqACK = m_nRcvEnqRequestCount; 
	pEnqAck->m_strPkgID = m_strRcvEnqRequestUUID32;


	nSendRes = _ServerAffairsSendReply(pEnqAck);

	if (0 != nSendRes)
	{
		//send data error
		LOG2(SourceInfo, DebugUtil::DebugError, "Send Reply Error [%s](Msg.size=[%d]) ", 
			m_strClientID.c_str(), m_nRcvEnqRequestCount);
		nFunRes = -1;		 
	}
	else
	{
		m_nEnqTotal += m_nRcvEnqRequestCount;
		LOG4(SourceInfo, DebugUtil::DebugDebug, 
			"sent reply finished,[%s][EnqWorkIndex=%d][Msg.size=%d]][TotalMsg=%d]", 
			m_strClientID.c_str(), m_nEnqWorkIndex, m_nRcvEnqRequestCount, m_nEnqTotal);	

		m_nEnqWorkIndex++; 
	}
	
	DEF_DELETE(pEnqAck);
	
	if (NULL != m_pCurrentRequestFrame)
	{		
		LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, 
			"[%s][strRcvEnqRequestUUID32=%s]sever delete one Frame ", 
			m_strClientID.c_str(), m_strRcvEnqRequestUUID32.c_str());

		_FreeEnqReqEx(&m_pCurrentRequestFrame);
		m_pCurrentRequestFrame = NULL;
	}

	return nFunRes;
}

int	 CMysqlSrvEnqWorker::getWorkTimeInfo(WorkTimeInfo& workTimeInfoGet)
{
	int nFunRes = 0;
	
	if (NULL != m_pMonitorEnqTime)
	{
		workTimeInfoGet.m_nEnqNotWork = m_pMonitorEnqTime->getNotworkTime();
	}
	else
	{
		workTimeInfoGet.m_nEnqNotWork = 0;
	}

	
	return nFunRes;	
}

int CMysqlSrvEnqWorker::_ServerPocessTokenCheck()
{
	int					nFunRes = 0;
	std::string			strSQL;
	std::vector<std::string> columnNames;
	IDbData*			pData = NULL;
	std::string strSrcDbConStr = g_system_variables.m_SrcDbConStr;
	
	//MonitorEnqTime begin time here
	m_pMonitorEnqTime->workBegin();

	
	//check m_pDataBaseProducer is not NULL
	if (NULL == m_pDBForEnqCheck)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "err: pProducerDataBase is NULL,[%s] [EnqWorkIndex=%d]", 
			m_strClientID.c_str(), m_nEnqWorkIndex);

		//MonitorEnqTime begin time here
		m_pMonitorEnqTime->workEnd();
		m_nEnqWorkState = server_state_enq_FinishWork;
		nFunRes = -1;
		return nFunRes;
	}

	//("select SRV_ID,MSG_SID from AQ_ENQ_DUPLICATE_CHECK where UPPER(LOCATION)=UPPER('%s') and UPPER(Q_NAME)=UPPER('%s') for update"),
	strSQL = m_pProducerDataBase->prepareSQLStatement(ENQ_SQL_SUPDATE_GET_MSGSID_7000, 
		m_strClientAgtName, m_strQName); 

	columnNames.clear();
	columnNames.push_back("SRV_ID");
	columnNames.push_back("MSG_SID");

	try
	{
		m_pDBForEnqCheck->connect();
		pData = m_pDBForEnqCheck->executeQuery(strSQL, columnNames);
		m_pDBForEnqCheck->commitTransaction();
	}
	catch (...)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, 	"Enq token,error! [strSQL=%s]", strSQL.c_str());

		nFunRes = -1;
		//MonitorEnqTime begin time here
		m_pMonitorEnqTime->workEnd();

		m_nEnqWorkState = server_state_enq_FinishWork;
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Retrieve Duplicated Check Table,[%s] [EnqWorkIndex=%d]", 
			m_strClientID.c_str(), m_nEnqWorkIndex);
	
		DEF_DELETE(pData); 
		return nFunRes;
	}
	
	if (NULL != pData && pData->getNumRows() > 0)
	{
		//MonitorEnqTime begin time here
		m_pMonitorEnqTime->workEnd();

		m_nEnqWorkState = server_state_enq_InitEnq;
		DEF_DELETE(pData); 
		return nFunRes;
	}

	//if not record then insert a new one
	//insert one record	to db  
	nFunRes = _EnqInsert(m_pProducerDataBase, m_strClientAgtName, m_nEnqInitOServerID, m_strQName, 0);
	if (0 != nFunRes)
	{
		nFunRes = -1;
		m_nEnqWorkState = server_state_enq_FinishWork;
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "[%s] [EnqWorkIndex=%d] _OracleEnqInsert() error!", 
			m_strClientID.c_str(), m_nEnqWorkIndex);
	}
	else
	{
		m_pProducerDataBase->commitTransaction();
		nFunRes = 0;
		//insert one record is ok, select for update again
		m_nEnqWorkState = server_state_enq_EnqTokenCheck;

	}
	//MonitorEnqTime begin time here
	m_pMonitorEnqTime->workEnd();

	DEF_DELETE(pData); 
	return nFunRes;
}		


void   CMysqlSrvEnqWorker::_updateActorEnqueueStatusBad()
{
	LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "set Enqueue Status Bad [m_ID=%d][%s][EnqWorkIndex=%d]", 
		m_ID, m_strClientID.c_str(), m_nEnqWorkIndex);

	CTcpConnectionPool::getInstance()->setEnqueueStatusBad(m_ID);
}

NS_END(TA_DB_Sync)

























