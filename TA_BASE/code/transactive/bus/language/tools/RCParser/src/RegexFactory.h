#pragma once


struct RegexFactory
{
    static RegexFactory& instance()
    {
        static RegexFactory singleton;
        return singleton;
    }

    const boost::regex& create_regex( const char* s );

private:

    RegexFactory() {}
    boost::mutex m_mutex;
    std::map<const char*, boost::regex> m_regex_map;
};
