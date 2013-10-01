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

#include "LambdaEncoder.h"
#include "Common.h"

namespace lambda
{
	class CPatchFile
	{
	public:
		CPatchFile(const tstring& strPatchFile, ILambdaEncoder* pEncoder);
		virtual ~CPatchFile(void);

		void Create(const tstring& strOriginalFile, const tstring& strModifiedFile);
		void Patch(const tstring& strOriginalFile, const tstring& strModifiedFile);
	};
};

#endif