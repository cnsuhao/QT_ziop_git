/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/modbus_comms/src/SerialPortException.cpp $
  * @author:  A. Ivankovic
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * An exception that is thrown as a result of a serial port set up or communications
  * problem.
  */

#include "SerialPortException.h"


namespace TA_Base_Bus
{
    SerialPortException::SerialPortException() throw()
    {
    }


    SerialPortException::SerialPortException( const std::string & message )  throw()
        : TransactiveException( message )
    {
    }


    SerialPortException::~SerialPortException() throw()
    {
    }
}
