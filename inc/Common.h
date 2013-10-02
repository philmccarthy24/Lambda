////////////////////////////////////////////////////////////////////////////////////////////////
/// Common definitions etc
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _COMMON_STUFF_H_
#define _COMMON_STUFF_H_

#include <vector>
#include <string>

typedef unsigned long ULONG, *PULONG;
typedef unsigned char BYTE, *PBYTE;
typedef std::vector<BYTE> BYTEBUF, *PBYTEBUF;

#ifdef UNICODE

#define tstring std::wstring
#define tout std::wcout
#define tchr wchar_t
#define STR_LIT(string) L##string

#else

#define tstring std::string
#define tout std::cout
#define tchr char
#define STR_LIT(string) string

#endif

#endif