#pragma once
#include "ilambdaoperation.h"

class CInsertOperation : public ILambdaOperation
{
public:
	CInsertOperation(const BYTEBUF& dataToInsert);
	CInsertOperation()
	virtual ~CInsertOperation(void);

	virtual void Serialise(PBYTEBUF pLambdaBuffer) override;
	virtual void Deserialise(const BYTEBUF& lambdaBuffer, PULONG pLambdaBufPos) override;
	virtual ULONG GetSerialisedSize() override;
	virtual void ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer) override;

	static bool isInsertOperation(BYTE nByteToCheck);

private:
	static const BYTE INSERT_OPERATION_TYPE;

	BYTEBUF m_DataToInsert;
};

