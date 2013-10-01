#pragma once
#include <vector>
#include <string>

typedef unsigned long ULONG, *PULONG;
typedef unsigned char BYTE, *PBYTE;
typedef std::vector<BYTE> BYTEBUF, *PBYTEBUF;

#ifdef UNICODE
#define std::wstring tstring
#else
#define std::string tstring
#endif