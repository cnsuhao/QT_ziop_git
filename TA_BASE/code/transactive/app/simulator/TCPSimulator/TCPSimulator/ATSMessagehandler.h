#pragma once
#include "MessageHandler.h"
#include "HexStringHelper.h"
#define CODE_CMD_READ	0x04
#define CODE_CMD_WRITE	0x10
class ATSMessageHandler : public IMessageHandler
{
public:
	ATSMessageHandler(HWND hWnd,unsigned long msgNotification);
public:
	virtual ~ATSMessageHandler(void);
	
	void setInputData(const char* data,unsigned int length) ;
	void setOutputData(const char* data,unsigned int length);
	bool DoAnalyze();
	void NotifyUI(const string& caption,const string& msg);
	inline void setDataPointTable(const string& strData) { m_dataPointTable = strData;};
private:
	bool IsInputAvailable();
	bool handleReadPointTable();
	bool setReplySignal();
	bool setReplyPointTable();
	bool handleWriteTrainPower();
private:
	HWND m_hWnd;
	unsigned long m_msgNotification;
	unsigned char m_funcCode,m_startAddr[2],m_byteCount,m_wordCount[2];
	int m_iStartAddr,m_iByteCount,m_iWordCount;
	int m_atsStatuas;
	string m_dataPointTable;
	HexStringHelper m_hexStringHelper;
};