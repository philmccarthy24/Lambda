#pragma once
#include "LambdaLib.h"

class ILambdaOperation
{
public:
	virtual void Serialise(PBYTEBUF pLambdaBuffer) = 0;
	virtual void Deserialise(const BYTEBUF& lambdaBuffer, PULONG pLambdaBufPos) = 0;
	virtual ULONG GetSerialisedSize() = 0;
	virtual void ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer) = 0;
};