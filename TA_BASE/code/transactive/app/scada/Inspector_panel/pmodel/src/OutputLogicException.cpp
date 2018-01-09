#include "OutputLogicException.h"

namespace TA_Base_App
{

	OutputLogicException::OutputLogicException(const std::string& msg) throw()
	:TransactiveException(msg)
	{
		m_exceptionCode = 0;
	}

	OutputLogicException::OutputLogicException(int code) throw()
	:TransactiveException("OutputLogicException")
	{
		m_exceptionCode = code;
	}

	OutputLogicException::~OutputLogicException()
	{

	}

	void	OutputLogicException::setExceptionCode(int exceptionCode)
	{
		m_exceptionCode = exceptionCode;
	}

	void	OutputLogicException::setParameters(const std::vector<std::string>& parameters)
	{
		m_parameters = parameters;
	}


	void	OutputLogicException::appendParameter(const std::string& parameter)
	{
		m_parameters.push_back(parameter);
	}


	int		OutputLogicException::getExceptionCode() const
	{
		return m_exceptionCode;
	}


	std::vector<std::string>	OutputLogicException::getParameters()
	{
		return m_parameters;
	}
}