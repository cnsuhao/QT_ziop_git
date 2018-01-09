#pragma once


namespace TA_Base_Bus
{

    class CharacterCodeConverter
    {
    public:

        static std::string convertUTF8toANSI( const char* strUTF8 );
        static CString convertUTF8toCString( const char* strUTF8 );

        static std::string convertCStringtoUTF8( const CString& cstr );
    };

}
