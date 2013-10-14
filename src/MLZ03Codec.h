//
//  MLZ03Codec.h
//  StaticLib
//
//  Maximal Symbol Length Dictionary Coder/Decoder, without output cost optimisation
//
//  Created by Phil McCarthy on 05/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#ifndef __StaticLib__MLZ03Codec__
#define __StaticLib__MLZ03Codec__

#include <iostream>
#include "LambdaCodec.h"

namespace lambda
{
	class CMLZ03Codec : public ILambdaCodec
	{
	public:
		// Operations on buffers
		virtual CDataBuffer EncodeBuffer(const CDataBuffer& originalBuffer, const CDataBuffer& modifiedBuffer);
		virtual CDataBuffer DecodeBuffer(const CDataBuffer& originalBuffer, const CDataBuffer& lambdaBuffer);
        
	private:
        static CDataBuffer FindLongestMatch(const CDataBuffer& sourceBuffer, const CDataBuffer& patternBuffer);
        static CDataBuffer QuickMatch(const CDataBuffer& sourceBuffer, const CDataBuffer& patternBuffer);
        
        static const ULONG LARGE_MATCH_THRESHOLD;
        
		BYTEVECTOR m_LambdaBuffer;
		BYTEVECTOR m_UpdatedBuffer;
	};
};

#endif /* defined(__StaticLib__MLZ02Codec__) */
