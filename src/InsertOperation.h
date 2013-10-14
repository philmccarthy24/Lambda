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
#include "DataBuffer.h"

namespace lambda
{
	class CInsertOperation : public ILambdaOperation
	{
	public:
		CInsertOperation(const CDataBuffer& dataToInsert);
        CInsertOperation();

		virtual void Serialise(const IDataWriter& lambdaWriter) const;
		static std::unique_ptr<CInsertOperation> TryDeserialise(IDataReader& lambdaReader);
		
		virtual ULONG ObjectSize() const;
		virtual ULONG WriteSize() const;
        
		virtual void Apply(const IDataWriter& outputWriter) const;
        
        void Merge(const ILambdaOperation& operationToMerge);
        int CalcMergeCost(const ILambdaOperation& operationToMerge) const;
        void Reset();

	private:
        static ULONG GetHdrSize();
        
		static const BYTE INSERT_OPERATION_TYPE;

		BYTEVECTOR m_DataToInsert;
	};
};

#endif