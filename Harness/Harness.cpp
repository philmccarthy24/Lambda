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

	// load first file
	std::ifstream originalFile(argv[1], std::ios::binary);
	BYTEBUF originalFileBuf;
	FileStreamToVector(originalFile, GetFileStreamSize(originalFile), &originalFileBuf);

	// load second file
	std::ifstream updatedFile(argv[1], std::ios::binary);
	BYTEBUF updatedFileBuf;
	FileStreamToVector(updatedFile, GetFileStreamSize(updatedFile), &updatedFileBuf);

	BYTEBUF lambdaBuffer;

	// do the difference encoding
	CMLZ01Encoder encoder;
	encoder.CreateLambdaBuffer(originalFileBuf, updatedFileBuf, &lambdaBuffer);

	// output lambda to file
	//... here...
	return 0;
}

size_t GetFileStreamSize(std::ifstream& fileStream)
{
	fileStream.seekg(0, std::ifstream::end);
    size_t nFileStreamSz = fileStream.tellg();
	fileStream.seekg(0, std::ifstream::beg);
	return nFileStreamSz;
}

void FileStreamToVector(std::ifstream& fileStream, size_t nFileStreamSize, PBYTEBUF pBuffer)
{
	pBuffer->clear();
	pBuffer->reserve(nFileStreamSize);
	pBuffer->assign(std::istreambuf_iterator<char>(fileStream),
					std::istreambuf_iterator<char>());
}