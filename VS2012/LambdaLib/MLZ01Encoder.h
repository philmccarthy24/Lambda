#pragma once
#include "ilambdaencoder.h"

// McCarthy Lambda Compression Algorithm 01
// Maximal Symbol Length Dictionary Encoder (without output cost optimisation)
class CMLZ01Encoder : public ILambdaEncoder
{
public:
	CMLZ01Encoder(void);
	virtual ~CMLZ01Encoder(void);

	// Operations on buffers
	virtual const BYTEBUF& Encode(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer) override;
	virtual const BYTEBUF& ApplyPatch(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer) override;

private:
	bool FindSymbolInBuffer(const BYTEBUF& symbol, const BYTEBUF& originalBuffer, PULONG pFoundSymbolPosInOriginalBuf);

	BYTEBUF m_LambdaBuffer;
	BYTEBUF m_UpdatedBuffer;
};