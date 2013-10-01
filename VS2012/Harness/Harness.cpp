// Harness.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MLZ01Encoder.h"
#include <fstream>
#include <iterator>

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 4)
	{
		_tprintf(_T("Lambda [filename] [modifiedfilename] [differencefile]\n"));
	}

	// do the difference encoding
	CMLZ01Encoder bufferEncoder;
	CPatchFile patchFile(argv[3], &bufferEncoder);
	patchFile.Build(argv[1], argv[2]);

	return 0;
}
