#ifndef TASTRING_TRANSLATE_H_INCLUDED
#define TASTRING_TRANSLATE_H_INCLUDED
#include <string>


std::string TAStringTranslate( const char* s );
std::string TAStringTranslate( const std::string& s );

#define ta_tr(s) TAStringTranslate(s)


#endif
