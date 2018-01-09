#include "StdAfx.h"
#include "bus/language/src/CharacterCodeConverter.h"


namespace TA_Base_Bus
{

    std::string CharacterCodeConverter::convertUTF8toANSI( const char* strUTF8 )
    {
        // 1. UTF-8 to UTF-16
        int length = strlen(strUTF8) + 1;
        int wlength = MultiByteToWideChar( CP_UTF8, 0, strUTF8, length, 0, 0 );
        wchar_t* wsz = new wchar_t[wlength];
        MultiByteToWideChar( CP_UTF8, 0, strUTF8, length, wsz, wlength );

        // 2. UTF-16 to ANSI
        length = WideCharToMultiByte( CP_ACP, 0, wsz, wlength, 0, 0, 0, 0 );
        std::string strANSI( length, '\0' );
        WideCharToMultiByte( CP_ACP, 0, wsz, wlength, const_cast<char*>(strANSI.c_str()), length, 0, 0 );

        delete[] wsz;
        wsz = NULL;

        return strANSI;
    }


    CString CharacterCodeConverter::convertUTF8toCString( const char* strUTF8 )
    {
        // 1. UTF-8 to UTF-16
        int length = strlen(strUTF8) + 1;
        int wlength = MultiByteToWideChar( CP_UTF8, 0, strUTF8, length, 0, 0 );
        wchar_t* wsz = new wchar_t[wlength];
        MultiByteToWideChar( CP_UTF8, 0, strUTF8, length, const_cast<wchar_t*>(wsz), wlength );

#ifdef UNICODE
        CString ret( wsz );

        delete[] wsz;
        wsz = NULL;

        return ret;
#else
        // 2. UTF-16 to ANSI
        length = WideCharToMultiByte( CP_ACP, 0, wsz, wlength, 0, 0, 0, 0 );

        CString ret( '\0', length );

        WideCharToMultiByte( CP_ACP, 0, wsz, wlength, ret.GetBuffer(), length, 0, 0 );

        delete[] wsz;
        wsz = NULL;

        return ret;
#endif
    }


    std::string CharacterCodeConverter::convertCStringtoUTF8( const CString& cs )
    {
#ifdef UNICODE
        int wlength = cs.GetLength();
        int length = WideCharToMultiByte( CP_UTF8, 0, cs, wlength, 0, 0, 0, 0 );
        std::string strUTF8( length, '\0' );
        WideCharToMultiByte( CP_UTF8, 0, cs, wlength, const_cast<char*>(strUTF8.c_str()), length, 0, 0 );
        return strUTF8;
#else
        return cs.GetBuffer();
#endif
    }

}
