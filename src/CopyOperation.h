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
#include "DataBuffer.h"

namespace lambda
{
	class CCopyOperation : public ILambdaOperation
	{
	public:
		CCopyOperation(const CDataBuffer& copyFromBuffer, const CDataBuffer& dataToCopy);

		virtual void Serialise(const IDataWriter& lambdaWriter) const;
		static std::unique_ptr<CCopyOperation> TryDeserialise(const CDataBuffer& copyFromBuffer, IDataReader& lambdaReader);
        
        virtual ULONG ObjectSize() const;
		virtual ULONG WriteSize() const;
        
		virtual void Apply(const IDataWriter& modifiedWriter) const;

	private:
		CCopyOperation();

		static const BYTE COPY_OPERATION_TYPE;
        
        CDataBuffer m_dataToCopy;
        CDataBuffer m_copyFromBuffer;
	};
};

#endif