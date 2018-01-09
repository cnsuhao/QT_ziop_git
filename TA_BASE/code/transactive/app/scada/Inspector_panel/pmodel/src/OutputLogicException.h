#pragma  once

#include <string>
#include <vector>
#include "core/exceptions/src/TransactiveException.h"

//low level
#define Ex_Code_ScadaProxyException				20000
#define Ex_Code_NotAvailableException			20001
#define Ex_Code_BadParameterException			20002
#define Ex_Code_AccessDeniedException			20003
#define Ex_Code_ObjectResolutionException		20004
#define Ex_Code_OperationModeException			20005
#define Ex_Code_CORBA_Exception					20006
#define Ex_Code_UnknownException				20007

//
#define Ex_Code_InvalidValue1_1					30008	//"Invalid Value[%1]"
#define Ex_Code_InvalidValue2_3					30009	//"Value is not within range, Valid range is from [%1] to [%2] [%3]"
#define Ex_Code_InvalidValue3_1					30010	//"Value [] is not a valid number.";



#define CONTROL_STATUS_SENT_0 					50000//"Sent"
#define CONTROL_STATUS_PENDING_0				50001//"Pending"
#define CONTROL_STATUS_SUCCESSFUL_0				50002//"Successful"
#define CONTROL_STATUS_FAIL_0					50003//"Failed"
#define CONTROL_STATUS_LCC_FAIL_4				50004//"[%1]  [%2]:[%3]- Failed due to [%4].;
#define CONTROL_STATUS_RCC_FAIL_3				50005//"[%1]  [%2]:[%3]- Failed due to Bad Return Condition
#define CONTROL_STATUS_COMMS_FAIL_0				50006//"Failed due to Comms Error"


namespace TA_Base_App
{
	class OutputLogicException : public TA_Base_Core::TransactiveException
	{
	public:
		explicit OutputLogicException(const std::string& msg) throw();
		explicit OutputLogicException(int code) throw();
		~OutputLogicException() throw();
	public:
		void	setExceptionCode(int exceptionCode);
		void	setParameters(const std::vector<std::string>& parameters);
		void	appendParameter(const std::string& parameter);
		int		getExceptionCode() const;
		std::vector<std::string>	getParameters();

	private:
		int							m_exceptionCode;
		std::vector<std::string>	m_parameters;
	};
}; 