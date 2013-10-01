#ifndef _PATCHFILE_H_
#define _PATCHFILE_H_

#include "ILambdaEncoder.h"
#include "LambdaLib.h"

namespace lambda
{
	class CPatchFile
	{
	public:
		CPatchFile(const tstring& strPatchFile, ILambdaEncoder* pEncoder);
		virtual ~CPatchFile(void);

		void Build(const tstring& strOriginalFile, const tstring& strModifiedFile);
		void Patch(const tstring& strOriginalFile, const tstring& strModifiedFile);
	};
};

#endif