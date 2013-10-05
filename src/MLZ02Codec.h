//
//  MLZ02Codec.h
//  StaticLib
//
//  Maximal Symbol Length Dictionary Coder/Decoder, without output cost optimisation
//
//  Created by Phil McCarthy on 05/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#ifndef __StaticLib__MLZ02Codec__
#define __StaticLib__MLZ02Codec__

#include <iostream>
#include "LambdaCodec.h"

namespace lambda
{
	class CMLZ02Codec : public ILambdaCodec
	{
	public:
		CMLZ02Codec(void);
		virtual ~CMLZ02Codec(void);
        
		// Operations on buffers
		virtual const BYTEBUF& EncodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer);
		virtual const BYTEBUF& DecodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer);
        
	private:
		bool FindSymbolInBuffer(const BYTEBUF& symbol, const BYTEBUF& originalBuffer, PULONG pFoundSymbolPosInOriginalBuf);
        
		BYTEBUF m_LambdaBuffer;
		BYTEBUF m_UpdatedBuffer;
	};
};

#endif /* defined(__StaticLib__MLZ02Codec__) */
