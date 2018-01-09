#include "StdAfx.h"
#include "RCParserThread.h"
#include "Utility.h"
#include "LanguageFileGenerator.h"
#include "ResourceParserFactory.h"
#include "IResource.h"


void RCParserThread::operator()()
{
    try
    {
        const std::string& rc = m_parameter->m_rc;
        const std::string& rc_h = m_parameter->m_rc_h;
        const std::string& application_id = m_parameter->m_application_id;
        const std::string& language = m_parameter->m_language_file;

        std::string s = Utility::get_string_from_file( rc );

        if ( s.empty() )
        {
            std::cout << "open failed: " << rc << std::endl;
            return;
        }

        Utility::remove_comments( s );

        LanguageFileGenerator generator( application_id, rc_h, language );
        ResourceParserFactory resource_parser_factory( s );
        IResourcePtrList resources = resource_parser_factory.parse();

        for ( size_t i = 0; i < resources.size(); ++i )
        {
            resources[i]->accept( &generator );
        }
    }
    catch ( std::exception& ex )
    {
        std::cout << "std::exception: " << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "caught exception." << std::endl;
    }
}
