////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a single patching operation in a batch transform to convert one buffer
/// state to another
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LAMBDA_OPERATION_H_
#define _LAMBDA_OPERATION_H_

#include "Common.h"

namespace lambda
{
	class ILambdaOperation
	{
	public:
		virtual void ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer) = 0;
		virtual void Serialise(PBYTEBUF pLambdaBuffer) = 0;
		virtual ULONG GetSerialisedSize() = 0;
	};
};

#endif