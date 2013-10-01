////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an operation to copy data from one buffer to another
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#include "PatchFile.h"
#include <fstream>

namespace lambda
{

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
void CPatchFile::Create(const tstring& strOriginalFile, const tstring& strModifiedFile)
{
	std::ifstream originalFile(strOriginalFile, std::ifstream::binary);
    if (!originalFile)
    {
        throw std::exception();
    }
	BYTEBUF originalFileBuf;
	FileStreamToVector(originalFile, &originalFileBuf);
	originalFile.close();

	std::ifstream updatedFile(strModifiedFile, std::ifstream::binary);
	BYTEBUF updatedFileBuf;
	FileStreamToVector(updatedFile, &updatedFileBuf);
	updatedFile.close();

	// get the difference encoding
	const BYTEBUF& patchBuffer = m_pEncoder->EncodeBuffer(originalFileBuf, updatedFileBuf);

	// save lambda to file m_strPatchFileName
	std::ofstream lambdaFile(m_strPatchFileName, std::ofstream::binary);
	lambdaFile.write(reinterpret_cast<const char*>(patchBuffer.data()), patchBuffer.size());
	lambdaFile.close();
}

void CPatchFile::Patch(const tstring& strOriginalFile, const tstring& strModifiedFile)
{
	std::ifstream originalFile(strOriginalFile, std::ios::binary);
	BYTEBUF originalFileBuf;
	FileStreamToVector(originalFile, &originalFileBuf);
	originalFile.close();

	std::ifstream patchFile(m_strPatchFileName, std::ios::binary);
	BYTEBUF patchFileBuf;
	FileStreamToVector(patchFile, &patchFileBuf);
	patchFile.close();

	const BYTEBUF& modifiedBuffer = m_pEncoder->DecodeBuffer(originalFileBuf, patchFileBuf);

	// write modified file out
	std::ofstream modifiedFile(strModifiedFile, std::ios::binary);
	modifiedFile.write(reinterpret_cast<const char*>(modifiedBuffer.data()), modifiedBuffer.size());
	modifiedFile.close();
}

void CPatchFile::FileStreamToVector(std::ifstream& fileStream, PBYTEBUF pBuffer)
{
	pBuffer->clear();
	// get stream size
	fileStream.seekg(0, fileStream.end);
    ULONG nFileStreamSz = fileStream.tellg();
	fileStream.seekg(0, fileStream.beg);
	// reserve space
	pBuffer->reserve(nFileStreamSz);
	// do the assignment
	pBuffer->assign(std::istreambuf_iterator<char>(fileStream),
					std::istreambuf_iterator<char>());
}

}; // namespace lambda