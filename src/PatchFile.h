////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a lambda encoded patch file. Creates an updated (patched) file given the
/// original file.
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PATCHFILE_H_
#define _PATCHFILE_H_

#include "LambdaCodec.h"
#include "Common.h"

namespace lambda
{
	class CPatchFile
	{
	public:
		CPatchFile(const std::string& strPatchFile, ILambdaCodec* pCodec);

		void Create(const std::string& strOriginalFile, const std::string& strModifiedFile);
		void Patch(const std::string& strOriginalFile, const std::string& strModifiedFile);

	private:
		static void FileStreamToVector(std::ifstream& fileStream, PBYTEVECTOR pBuffer);
        

		ILambdaCodec* m_pCodec;
		std::string m_strPatchFileName;
        
        static const int SIZE_OF_SHA1;
	};
};

#endif