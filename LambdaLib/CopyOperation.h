#pragma once
#include "ilambdaoperation.h"

class CCopyOperation : public ILambdaOperation
{
public:
	CCopyOperation(ULONG nCopyFromOffset, ULONG nNumBytesToCopy);
	CCopyOperation();
	virtual ~CCopyOperation(void);

	virtual void Serialise(PBYTEBUF pLambdaBuffer) override;
	virtual void Deserialise(const BYTEBUF& lambdaBuffer, PULONG pLambdaBufPos) override;
	virtual ULONG GetSerialisedSize() override;
	virtual void ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer) override;

	static bool isCopyOperation(BYTE nByteToCheck);

private:
	static const BYTE COPY_OPERATION_TYPE;

	ULONG m_nCopyFromOffset;
	ULONG m_nNumBytesToCopy;
};

