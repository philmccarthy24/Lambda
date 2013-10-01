#ifndef _COMMON_STUFF_H_
#define _COMMON_STUFF_H_

#include <vector>
#include <string>

typedef unsigned long ULONG, *PULONG;
typedef unsigned char BYTE, *PBYTE;
typedef std::vector<BYTE> BYTEBUF, *PBYTEBUF;

#ifdef UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif

#endif