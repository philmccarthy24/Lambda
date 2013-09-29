#pragma once
#include "LambdaLib.h"

class ILambdaEncoder
{
public:
	virtual void CreateLambdaBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer, PBYTEBUF pLambdaEncoding) = 0;
	virtual void ApplyLambdaToBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer, PBYTEBUF pModifiedBuffer) = 0;

	// file based functions here to look at files as whole and compare sha1 hashes for validation
};