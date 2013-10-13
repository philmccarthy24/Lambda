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
#include "LambdaIO.h"

namespace lambda
{
    
	class ILambdaOperation
	{
	public:
        ///////////////////////////////////////////////////////////////////////////////
        /// Apply the lambda operation using specified dictionary buffer, writing to
        /// the output buffer
        ///
        /// \param [out] CDataBuffer* pOutputBuffer - the buffer to apply the lambda op to
        ///////////////////////////////////////////////////////////////////////////////
		virtual void Apply(const IDataWriter& outputWriter) const = 0;
        
        ///////////////////////////////////////////////////////////////////////////////
        /// Serialise the operation to byte buffer
        ///
        /// \param [out] IDataWriter* pLambdaBuffer - the data writer object to use
        ///                                             for serialisation
        ///////////////////////////////////////////////////////////////////////////////
		virtual void Serialise(const IDataWriter& lambdaWriter) = 0;
        
        ///////////////////////////////////////////////////////////////////////////////
        /// Gets the lambda-serialised size
        ///
        /// \return ULONG - the serialised size
        ///////////////////////////////////////////////////////////////////////////////
		virtual ULONG ObjectSize() const = 0;
        
        ///////////////////////////////////////////////////////////////////////////////
        /// Gets the decoded size
        ///
        /// \return ULONG - the decoded block size
        ///////////////////////////////////////////////////////////////////////////////
		virtual ULONG WriteSize() const = 0;
	};
};

#endif