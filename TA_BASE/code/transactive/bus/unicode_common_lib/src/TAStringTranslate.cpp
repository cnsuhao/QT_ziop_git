#include "TAStringTranslate.h"
#include <QCoreApplication>


std::string TAStringTranslate( const char* s )
{
    return qApp->translate( "", s ).toStdString();
}


std::string TAStringTranslate( const std::string& s )
{
    return TAStringTranslate( s.c_str() );
}
