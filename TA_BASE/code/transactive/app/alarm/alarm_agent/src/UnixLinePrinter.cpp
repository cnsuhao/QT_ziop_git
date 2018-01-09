/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/alarm/alarm_agent/src/UnixLinePrinter.cpp $
  * @author:  Ross Tucker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "app/alarm/alarm_agent/src/UnixLinePrinter.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/exceptions/src/AlarmAgentException.h"

using TA_Base_Core::DebugUtil;
using TA_Base_Core::AlarmAgentException;


namespace TA_Base_App
{
    
    //
    // Constructor
    //
    UnixLinePrinter::UnixLinePrinter( const std::string& p_name, 
                                      const std::string& p_port, 
                                      const std::string& p_host )
    : AbstractPrinter(p_name, p_port, p_host)
    {
        m_printer = fopen(getPrinterName().c_str(), "a");
    }


    //
    // Destructor
    //
    UnixLinePrinter::~UnixLinePrinter()
    {
        if( m_printer != NULL )
        {
            fclose(m_printer);
            delete m_printer;
        }
    }


    //
    // print
    //
    void UnixLinePrinter::print( const std::string& p_stringToPrint )
    {
        if( m_printer != NULL )
        {
            fprintf(m_printer, p_stringToPrint.c_str());
            fflush(m_printer);
        }
        else
        {
            TA_THROW(AlarmAgentException("Failed to print to line printer"));
        }
    }
};
