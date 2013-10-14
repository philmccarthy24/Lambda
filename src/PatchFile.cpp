////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an operation to copy data from one buffer to another
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#include "PatchFile.h"
#include <fstream>
#include "Common.h"

namespace lambda
{

    CPatchFile::CPatchFile(const std::string& strPatchFile, ILambdaCodec* pCodec) :
    m_strPatchFileName(strPatchFile),
    m_pCodec(pCodec)
    {
    }


    CPatchFile::~CPatchFile(void)
    {
    }

    // simple version which encodes whole files at first.
    //TODO: - add sha1 hashes for validation
    //		- add multiple lambda buffer support per patch file so can support arbitrarily big files
    //		- add patch file 7zlib(?) compression
    void CPatchFile::Create(const std::string& strOriginalFile, const std::string& strModifiedFile)
    {
        std::ifstream originalFile(strOriginalFile, std::ifstream::binary);
        if (!originalFile)
        {
            std::stringstream strout;
            strout << "Cannot open original file (" << strOriginalFile << ") input stream" << std::endl;
            THROW_EXCEPTION(FileIOException, strout.str());
        }
        BYTEVECTOR originalFileBuf;
        FileStreamToVector(originalFile, &originalFileBuf);
        originalFile.close();

        std::ifstream updatedFile(strModifiedFile, std::ifstream::binary);
        if (!updatedFile)
        {
            std::stringstream strout;
            strout << "Cannot open updated file (" << strModifiedFile << ") input stream" << std::endl;
            THROW_EXCEPTION(FileIOException, strout.str());
        }
        BYTEVECTOR updatedFileBuf;
        FileStreamToVector(updatedFile, &updatedFileBuf);
        updatedFile.close();

        // get the difference encoding
        CDataBuffer patchBuffer = m_pCodec->EncodeBuffer(originalFileBuf, updatedFileBuf);

        // save lambda to file m_strPatchFileName
        std::ofstream lambdaFile(m_strPatchFileName, std::ofstream::binary);
        if (!lambdaFile)
        {
            std::stringstream strout;
            strout << "Cannot open patch file (" << m_strPatchFileName << ") output stream" << std::endl;
            THROW_EXCEPTION(FileIOException, strout.str());
        }
        lambdaFile.write(reinterpret_cast<const char*>(patchBuffer.Buffer()), patchBuffer.Size());
        lambdaFile.close();
    }

    void CPatchFile::Patch(const std::string& strOriginalFile, const std::string& strModifiedFile)
    {
        std::ifstream originalFile(strOriginalFile, std::ios::binary);
        if (!originalFile)
        {
            std::stringstream strout;
            strout << "Cannot open original file (" << strOriginalFile << ") input stream" << std::endl;
            THROW_EXCEPTION(FileIOException, strout.str());
        }
        BYTEVECTOR originalFileBuf;
        FileStreamToVector(originalFile, &originalFileBuf);
        originalFile.close();

        std::ifstream patchFile(m_strPatchFileName, std::ios::binary);
        if (!patchFile)
        {
            std::stringstream strout;
            strout << "Cannot open patch file (" << m_strPatchFileName << ") output stream" << std::endl;
            THROW_EXCEPTION(FileIOException, strout.str());
        }
        BYTEVECTOR patchFileBuf;
        FileStreamToVector(patchFile, &patchFileBuf);
        patchFile.close();

        CDataBuffer modifiedBuffer = m_pCodec->DecodeBuffer(originalFileBuf, patchFileBuf);

        // write modified file out
        std::ofstream modifiedFile(strModifiedFile, std::ios::binary);
        modifiedFile.write(reinterpret_cast<const char*>(modifiedBuffer.Buffer()), modifiedBuffer.Size());
        modifiedFile.close();
    }

    void CPatchFile::FileStreamToVector(std::ifstream& fileStream, PBYTEVECTOR pBuffer)
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