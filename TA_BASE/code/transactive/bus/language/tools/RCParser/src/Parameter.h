#pragma once


struct Parameter
{
    Parameter( const std::string& rc, const std::string& rc_h, const std::string& language, const std::string& appid )
        : m_rc( rc ),
          m_rc_h( rc_h ),
          m_language_file( language ),
          m_application_id( appid )
    {
    }

    std::string m_application_id;
    std::string m_rc;
    std::string m_rc_h;
    std::string m_language_file;
};

typedef boost::shared_ptr<Parameter> ParameterPtr;
typedef std::vector<ParameterPtr> ParameterPtrList;
