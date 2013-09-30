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
	virtual void CreateLambdaBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer, PBYTEBUF pLambdaEncoding) override;
	virtual void ApplyLambdaToBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer, PBYTEBUF pModifiedBuffer) override;

private:
	bool FindSymbolInBuffer(const BYTEBUF& symbol, const BYTEBUF& originalBuffer, PULONG pFoundSymbolPosInOriginalBuf);
};