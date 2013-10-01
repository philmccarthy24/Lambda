#pragma once
#include "LambdaLib.h"

class ILambdaEncoder
{
public:
	virtual const BYTEBUF& Encode(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer) = 0;
	virtual const BYTEBUF& ApplyPatch(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer) = 0;
};