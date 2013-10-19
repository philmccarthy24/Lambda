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
#include "sha1.h"

namespace lambda
{
    
    const int CPatchFile::SIZE_OF_SHA1 = 20;

    ///////////////////////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// \param [in] const std::string& strPatchFile - patch file path
    /// \param [out] ILambdaCodec* pCodec - delta codec to use
    ///////////////////////////////////////////////////////////////////////////////
    CPatchFile::CPatchFile(const std::string& strPatchFile, ILambdaCodec* pCodec) :
    m_strPatchFileName(strPatchFile),
    m_pCodec(pCodec)
    {
    }

    // simple version which encodes whole files at first.
    //TODO: - add multiple lambda buffer support per patch file so can support arbitrarily big files
    //		- add patch file zlib(?) compression
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
        
        // calculate sha1s
        BYTEVECTOR originalFileHash;
        originalFileHash.resize(SIZE_OF_SHA1);
        sha1::calc(originalFileBuf.data(), originalFileBuf.size(), originalFileHash.data());
        BYTEVECTOR modifiedFileHash;
        modifiedFileHash.resize(SIZE_OF_SHA1);
        sha1::calc(updatedFileBuf.data(), updatedFileBuf.size(), modifiedFileHash.data());

        // save lambda to file m_strPatchFileName
        std::ofstream lambdaFile(m_strPatchFileName, std::ofstream::binary);
        if (!lambdaFile)
        {
            std::stringstream strout;
            strout << "Cannot open patch file (" << m_strPatchFileName << ") output stream" << std::endl;
            THROW_EXCEPTION(FileIOException, strout.str());
        }
        
        // write original file hash
        lambdaFile.write(reinterpret_cast<const char*>(originalFileHash.data()), SIZE_OF_SHA1);
        
        // write modified file hash
        lambdaFile.write(reinterpret_cast<const char*>(modifiedFileHash.data()), SIZE_OF_SHA1);
        
        // write the patch data
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
        
        // get hash of original file
        BYTEVECTOR originalFileHash;
        originalFileHash.resize(SIZE_OF_SHA1);
        sha1::calc(originalFileBuf.data(), originalFileBuf.size(), originalFileHash.data());

        std::ifstream patchFile(m_strPatchFileName, std::ios::binary);
        if (!patchFile)
        {
            std::stringstream strout;
            strout << "Cannot open patch file (" << m_strPatchFileName << ") output stream" << std::endl;
            THROW_EXCEPTION(FileIOException, strout.str());
        }
        
        // read patch file
        BYTEVECTOR patchFileBuf;
        FileStreamToVector(patchFile, &patchFileBuf);
        patchFile.close();
        
        // check original hash
        if (memcmp(patchFileBuf.data(), originalFileHash.data(), SIZE_OF_SHA1) != 0)
        {
            std::stringstream strout;
            strout << "Source file specified (" << strOriginalFile << ") does not match file used to create patch." << std::endl;
            THROW_EXCEPTION(FileIOException, strout.str());
        }

        CDataBuffer modifiedBuffer = m_pCodec->DecodeBuffer(originalFileBuf,
                                                            CDataBuffer(patchFileBuf.data() + (SIZE_OF_SHA1 * 2),
                                                                        patchFileBuf.size() - (SIZE_OF_SHA1 * 2))
                                                            );
        
        // calc modified hash
        BYTEVECTOR modifiedFileHash;
        modifiedFileHash.resize(SIZE_OF_SHA1);
        sha1::calc(modifiedBuffer.Buffer(), modifiedBuffer.Size(), modifiedFileHash.data());
        
        // check modified hash
        if (memcmp(patchFileBuf.data() + SIZE_OF_SHA1, modifiedFileHash.data(), SIZE_OF_SHA1) != 0)
        {
            std::stringstream strout;
            strout << "Patched file (" << strModifiedFile << ") sha1 check failed: patching has failed." << std::endl;
            THROW_EXCEPTION(FileIOException, strout.str());
        }

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