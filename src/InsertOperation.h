////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an operation to insert data into a buffer
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _INSERT_OPERATION_H_
#define _INSERT_OPERATION_H_

#include "LambdaOperation.h"

namespace lambda
{
	class CInsertOperation : public ILambdaOperation
	{
	public:
		CInsertOperation(const BYTEBUF& dataToInsert);
		
		virtual ~CInsertOperation(void);

		virtual void Serialise(PBYTEBUF pLambdaBuffer); 
		static std::unique_ptr<CInsertOperation> TryDeserialise(const BYTEBUF& lambdaBuffer, PULONG pLambdaBufPos);
		
		virtual ULONG GetSerialisedSize();
		virtual void ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer);

	private:
		CInsertOperation();
		
		static const BYTE INSERT_OPERATION_TYPE;

		BYTEBUF m_DataToInsert;
	};
};

#endif