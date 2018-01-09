#include "StdAfx.h"
#include "bus/language/src/RawResourceSplitter.h"
#include "bus/language/src/CharacterCodeConverter.h"


namespace TA_Base_Bus
{

    RawResourceSplitter::RawResourceSplitter( const std::string& rawResource )
    {
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep("\t");
        tokenizer tokens(rawResource, sep);

        for ( tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it )
        {
            m_columns.push_back( *it );

            removeOutermostDoubleQuotationMarks( m_columns.back() );
            escapeCharacter( m_columns.back() );
        }
    }


    UINT RawResourceSplitter::getUnsignedInteger( UINT index )
    {
        ASSERT( index < m_columns.size() );
        return boost::lexical_cast<UINT>( m_columns[index] );
    }


    int RawResourceSplitter::getInteger( UINT index )
    {
        ASSERT( index < m_columns.size() );
        return boost::lexical_cast<int>( m_columns[index] );
    }


    CString RawResourceSplitter::getCString(UINT index)
    {
        ASSERT( index < m_columns.size() );
        return CharacterCodeConverter::convertUTF8toCString( m_columns[index].c_str() );
    }


    const std::string& RawResourceSplitter::getString(UINT index)
    {
        ASSERT( index < m_columns.size() );
        return m_columns[index];
    }


    void RawResourceSplitter::removeOutermostDoubleQuotationMarks( std::string& str )
    {
        if ( 2 <= str.size() )
        {
            if ( '\"' == *str.begin() && '\"' == *str.rbegin() )
            {
                str = str.substr( 1, str.size() - 2 );
            }
        }
    }


    void RawResourceSplitter::escapeCharacter( std::string& str )
    {
        static std::map<char, std::string> ESCAPE_CHARACTER_MAP;

        if ( ESCAPE_CHARACTER_MAP.empty() )
        {
            ESCAPE_CHARACTER_MAP['a'] = "\a";
            ESCAPE_CHARACTER_MAP['b'] = "\b";
            ESCAPE_CHARACTER_MAP['f'] = "\f";
            ESCAPE_CHARACTER_MAP['n'] = "\n";
            ESCAPE_CHARACTER_MAP['r'] = "\r";
            ESCAPE_CHARACTER_MAP['t'] = "\t";
            ESCAPE_CHARACTER_MAP['v'] = "\v";
            ESCAPE_CHARACTER_MAP['\''] = "\'";
            ESCAPE_CHARACTER_MAP['\"'] = "\"";
            ESCAPE_CHARACTER_MAP['\\'] = "\\";
        }

        size_t backslashPos = str.find( '\\' );

        while ( backslashPos != std::string::npos )
        {
            if ( backslashPos < str.size() - 1 )
            {
                char escapeChar = str[backslashPos + 1];

                std::map<char, std::string>::iterator findIt = ESCAPE_CHARACTER_MAP.find( escapeChar );

                if ( findIt != ESCAPE_CHARACTER_MAP.end() )
                {
                    str.replace(backslashPos, 2, findIt->second);
                }
            }

            backslashPos = str.find( '\\', backslashPos + 1 );
        }
    }

}
