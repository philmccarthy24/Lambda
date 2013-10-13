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
#include "DataBuffer.h"

namespace lambda
{
	class ILambdaCodec
	{
	public:
		virtual const CDataBuffer EncodeBuffer(const CDataBuffer& originalBuffer, const CDataBuffer& modifiedBuffer) = 0;
		virtual const CDataBuffer DecodeBuffer(const CDataBuffer& originalBuffer, const CDataBuffer& lambdaBuffer) = 0;
	};
};

#endif