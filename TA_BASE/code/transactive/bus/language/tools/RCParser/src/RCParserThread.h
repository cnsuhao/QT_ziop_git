#pragma once
#include "Parameter.h"


struct RCParserThread
{
    RCParserThread( ParameterPtr param )
        : m_parameter( param )
    {
        m_thread = new boost::thread( boost::ref(*this) );
    }

    virtual ~RCParserThread()
    {
        m_thread->join();
        delete m_thread;
        m_thread = NULL;
    }

    void operator()();

private:

    boost::thread* m_thread;
    ParameterPtr m_parameter;
};

typedef boost::shared_ptr<RCParserThread> RCParserThreadPtr;
typedef std::vector<RCParserThreadPtr> RCParserThreadPtrList;
