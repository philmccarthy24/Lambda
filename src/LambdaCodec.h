////////////////////////////////////////////////////////////////////////////////////////////////
/// Interface for difference encoding two buffers
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LAMBDA_CODEC_H_
#define _LAMBDA_CODEC_H_

#include "Common.h"

namespace lambda
{
	class ILambdaCodec
	{
	public:
		virtual const BYTEBUF& EncodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer) = 0;
		virtual const BYTEBUF& DecodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer) = 0;
	};
};

#endif