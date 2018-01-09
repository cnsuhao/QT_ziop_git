
#include "PrinterJob.h"

namespace TA_Base_App
{
	CPrinterJob::CPrinterJob()
		: m_bTerminate(false)
	{

	}

	CPrinterJob::~CPrinterJob()
	{

	}

	void CPrinterJob::run()
	{

	}

	void CPrinterJob::terminate()
	{
		m_bTerminate = true;
	}
}