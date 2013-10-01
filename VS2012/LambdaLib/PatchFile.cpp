#include "StdAfx.h"
#include "PatchFile.h"


CPatchFile::CPatchFile(const tstring& strPatchFile, ILambdaEncoder* pEncoder) :
m_strPatchFileName(strPatchFile),
m_pEncoder(pEncoder)
{
}


CPatchFile::~CPatchFile(void)
{
}

// simple version which encodes whole files at first.
//TODO: - add sha1 hashes for validation
//		- add multiple lambda buffer support per patch file so can support arbitrarily big files
//		- add patch file 7zlib(?) compression
void CPatchFile::Build(const tstring& strOriginalFile, const tstring& strModifiedFile)
{
	std::ifstream originalFile(strOriginalFile, std::ios::binary);
	BYTEBUF originalFileBuf;
	FileStreamToVector(originalFile, &originalFileBuf);

	std::ifstream updatedFile(strModifiedFile, std::ios::binary);
	BYTEBUF updatedFileBuf;
	FileStreamToVector(updatedFile, &updatedFileBuf);

	const BYTEBUF& patchBuffer = m_pEncoder->Encode(originalFileBuf, updatedFileBuf);

	//TODO: save lambda to file m_strPatchFileName
}

void CPatchFile::Patch(const tstring& strOriginalFile, const tstring& strModifiedFile)
{
	std::ifstream originalFile(strOriginalFile, std::ios::binary);
	BYTEBUF originalFileBuf;
	FileStreamToVector(originalFile, &originalFileBuf);

	std::ifstream patchFile(m_strPatchFileName, std::ios::binary);
	BYTEBUF patchFileBuf;
	FileStreamToVector(patchFile, &patchFileBuf);

	const BYTEBUF& modifiedBuffer = m_pEncoder->ApplyPatch(originalFileBuf, &patchFileBuf);

	// TODO: write modified file out here
}

void CPatchFile::FileStreamToVector(std::ifstream& fileStream, PBYTEBUF pBuffer)
{
	pBuffer->clear();
	// get stream size
	fileStream.seekg(0, std::ifstream::end);
    size_t nFileStreamSz = fileStream.tellg();
	fileStream.seekg(0, std::ifstream::beg);
	// reserve space
	pBuffer->reserve(nFileStreamSz);
	// do the assignment
	pBuffer->assign(std::istreambuf_iterator<char>(fileStream),
					std::istreambuf_iterator<char>());
}