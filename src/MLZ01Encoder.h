////////////////////////////////////////////////////////////////////////////////////////////////
/// McCarthy Lambda Compression Algorithm 01
/// Maximal Symbol Length Dictionary Encoder, first attempt without any output cost optimisation
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MLZ01ENCODER_H_
#define _MLZ01ENCODER_H_

#include "LambdaEncoder.h"

namespace lambda
{
	class CMLZ01Encoder : public ILambdaEncoder
	{
	public:
		CMLZ01Encoder(void);
		virtual ~CMLZ01Encoder(void);

		// Operations on buffers
		virtual const BYTEBUF& EncodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer);
		virtual const BYTEBUF& DecodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer);

	private:
		bool FindSymbolInBuffer(const BYTEBUF& symbol, const BYTEBUF& originalBuffer, PULONG pFoundSymbolPosInOriginalBuf);

		BYTEBUF m_LambdaBuffer;
		BYTEBUF m_UpdatedBuffer;
	};
};
	
#endif