// Lambda.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MLZ01Encoder.h"
#include "PatchFile.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 4)
	{
		_tprintf(_T("Lambda [filename] [modifiedfilename] [differencefile]\n"));
	}

	// set up a patch file object
	lambda::CMLZ01Encoder bufferEncoder;
	lambda::CPatchFile patchFile(argv[3], &bufferEncoder);

	// create the patch file
	patchFile.Create(argv[1], argv[2]);

	return 0;
}

