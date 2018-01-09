/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/sockets/src/Datagram.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
#include "Datagram.h"

namespace TA_Base_Core
{
    Datagram::Datagram() : m_bufferSize(0)
    {
    }
    
    Datagram::Datagram(const Datagram &copy)
    {
        SetBuffer(&(copy.m_buffer[0]), copy.m_bufferSize);
    }
    
    Datagram::~Datagram()
    {
    }
    
    unsigned int
    Datagram::GetBuffer(char** buf)
    {
        *buf = m_buffer;
        return m_bufferSize;
    }
    
    void 
    Datagram::SetBuffer(const char* buf, unsigned int numBytes)
    {
        m_bufferSize = numBytes;
        if (m_bufferSize > MaxDatagramSize)
            m_bufferSize = MaxDatagramSize;
    
        if (buf != 0 && buf != m_buffer)
        {
            for (unsigned int i = 0; i < m_bufferSize; i++)
                m_buffer[i] = buf[i];
        }
    }
}
