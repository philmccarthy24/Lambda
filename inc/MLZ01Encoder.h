#ifndef _MLZ01ENCODER_H_
#define _MLZ01ENCODER_H_

#include "LambdaEncoder.h"

namespace lambda
{
	// McCarthy Lambda Compression Algorithm 01
	// Maximal Symbol Length Dictionary Encoder (without output cost optimisation)
	class CMLZ01Encoder : public ILambdaEncoder
	{
	public:
		CMLZ01Encoder(void);
		virtual ~CMLZ01Encoder(void);

		// Operations on buffers
		virtual const BYTEBUF& EncodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer) override;
		virtual const BYTEBUF& DecodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer) override;
	};
};
	
#endif