////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an operation to copy data from one buffer to another
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _COPY_OPERATION_H_
#define _COPY_OPERATION_H_

#include "LambdaOperation.h"

namespace lambda
{
	class CCopyOperation : public ILambdaOperation
	{
	public:
		CCopyOperation(ULONG nCopyFromOffset, ULONG nNumBytesToCopy);
		
		virtual ~CCopyOperation(void);

		virtual void Serialise(PBYTEBUF pLambdaBuffer);
		static std::unique_ptr<CCopyOperation> TryDeserialise(const BYTEBUF& lambdaBuffer, PULONG pLambdaBufPos);

		virtual ULONG GetSerialisedSize();
		virtual void ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer);

	private:
		CCopyOperation();

		static const BYTE COPY_OPERATION_TYPE;

		ULONG m_nCopyFromOffset;
		ULONG m_nNumBytesToCopy;
	};
};

#endif