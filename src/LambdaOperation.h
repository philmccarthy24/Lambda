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
        ///////////////////////////////////////////////////////////////////////////////
        /// Apply the lambda operation using specified dictionary buffer, writing to
        /// the output buffer
        ///
        /// \param [in] const BYTEBUF& originalBuffer - the buffer to use as dictionary
        /// \param [out] PBYTEBUF pOutputBuffer - the buffer to apply the lambda op to
        ///////////////////////////////////////////////////////////////////////////////
		virtual void ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer) = 0;
        
        ///////////////////////////////////////////////////////////////////////////////
        /// Serialise the operation to byte buffer
        ///
        /// \param [out] PBYTEBUF pLambdaBuffer - the buffer to serialise to
        ///////////////////////////////////////////////////////////////////////////////
		virtual void Serialise(PBYTEBUF pLambdaBuffer) = 0;
        
        ///////////////////////////////////////////////////////////////////////////////
        /// Gets the serialised size
        ///
        /// \return ULONG - the serialised size
        ///////////////////////////////////////////////////////////////////////////////
		virtual ULONG GetSerialisedSize() = 0;
        
        ///////////////////////////////////////////////////////////////////////////////
        /// Prints the internal state of the operation to std out
        ///
        ///////////////////////////////////////////////////////////////////////////////
        virtual void Print() = 0;
	};
};

#endif