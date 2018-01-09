#pragma once

#include <boost/scoped_array.hpp>
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#define TSTRING std::basic_string<TCHAR>
#define TSTRINGSTREAM std::basic_stringstream<TCHAR>
#define TCHAR_SEPARATOR boost::char_separator<TCHAR>
#define TCHAR_TOKENIZER boost::tokenizer<TCHAR_SEPARATOR, TSTRING::const_iterator, TSTRING>
#define WIDECHARTOMULTIBYTE(s) TAUnicodeString::wcharToMultiByteText(s, CP_UTF8)
#define SCOPEDWTOM(s) boost::scoped_array<char>(WIDECHARTOMULTIBYTE(s)).get()
#define MULTIBYTETOWIDECHAR(s) TAUnicodeString::mutliByteToWcharText(s, CP_UTF8)
#define SCOPEDMTOW(s) boost::scoped_array<wchar_t>(MULTIBYTETOWIDECHAR(s)).get()