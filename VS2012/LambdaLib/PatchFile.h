#pragma once
#include "ILambdaEncoder.h"
#include "LambdaLib.h"

class CPatchFile
{
public:
	CPatchFile(const tstring& strPatchFile, ILambdaEncoder* pEncoder);
	virtual ~CPatchFile(void);

	void Build(const tstring& strOriginalFile, const tstring& strModifiedFile);
	void Patch(const tstring& strOriginalFile, const tstring& strModifiedFile);

private:
	void FileStreamToVector(std::ifstream& fileStream, PBYTEBUF pBuffer);

	ILambdaEncoder* m_pEncoder;
	tstring m_strPatchFileName;
};

