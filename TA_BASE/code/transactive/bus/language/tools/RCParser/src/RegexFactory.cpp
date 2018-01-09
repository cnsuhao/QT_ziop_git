#include "StdAfx.h"
#include "RegexFactory.h"


const boost::regex& RegexFactory::create_regex( const char* s )
{
    boost::lock_guard<boost::mutex> lock(m_mutex);

    std::map<const char*, boost::regex>::iterator it = m_regex_map.find( s );

    if ( it == m_regex_map.end() )
    {
        it = m_regex_map.insert( std::map<const char*, boost::regex>::value_type( s, boost::regex(s) ) ).first;
    }

    return it->second;
}
