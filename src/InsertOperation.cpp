////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an operation to insert data into a buffer
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#include "InsertOperation.h"

namespace lambda
{

const BYTE CInsertOperation::INSERT_OPERATION_TYPE = 1;

///////////////////////////////////////////////////////////////////////////////
/// Main constructor
///
/// \param [in] const BYTEBUF& dataToInsert - the data to insert
///////////////////////////////////////////////////////////////////////////////
CInsertOperation::CInsertOperation(const BYTEBUF& dataToInsert) :
	m_DataToInsert(dataToInsert)
{
}

///////////////////////////////////////////////////////////////////////////////
/// Default constructor
///
///////////////////////////////////////////////////////////////////////////////
CInsertOperation::CInsertOperation()
{
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor
///
///////////////////////////////////////////////////////////////////////////////
CInsertOperation::~CInsertOperation(void)
{
}

///////////////////////////////////////////////////////////////////////////////
/// Serialises the insert operation to a buffer
///
/// \param [out] PBYTEBUF pLambdaBuffer - the buffer to serialise to
///////////////////////////////////////////////////////////////////////////////
void CInsertOperation::Serialise(PBYTEBUF pLambdaBuffer)
{
	// first output the type. We can hone this to be a bit.
	pLambdaBuffer->push_back(INSERT_OPERATION_TYPE);
	// then output length
	ULONG nDataInsertionLength = m_DataToInsert.size();
	size_t nLambdaSz = pLambdaBuffer->size();
	pLambdaBuffer->resize(nLambdaSz + sizeof(nDataInsertionLength));
	std::memcpy(pLambdaBuffer->data() + nLambdaSz, &nDataInsertionLength, sizeof(nDataInsertionLength));
	// then output data
	//pLambdaBuffer->reserve(pLambdaBuffer->size() + nDataInsertionLength);
	pLambdaBuffer->insert(pLambdaBuffer->end(), m_DataToInsert.begin(), m_DataToInsert.end());
}

///////////////////////////////////////////////////////////////////////////////
/// Reads the insert operation state from a buffer
///
/// \param [in] const BYTEBUF& lambdaBuffer - the buffer to read from
/// \param [in,out] PULONG pLambdaBufPos - the position in the buffer to read from
/// \return std::unique_ptr<CInsertOperation> - the deserialised insert operation,
///			or nullptr if lambdaBuffer did not contain a CInsertOperation at
///			position *pLambdaBufPos
///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<CInsertOperation> CInsertOperation::TryDeserialise(const BYTEBUF& lambdaBuffer, PULONG pLambdaBufPos)
{
	ULONG nPos = *pLambdaBufPos;
	
	if (lambdaBuffer[nPos] != INSERT_OPERATION_TYPE)
	{
		// not an insert operation
		return nullptr;
	}
	nPos++;
	
	// get length
	ULONG nNumBytesToInsert = *(reinterpret_cast<const ULONG*>(lambdaBuffer.data() + nPos));
	nPos += sizeof(ULONG);
	
	// get bytes to insert
	BYTEBUF dataToInsert;
	dataToInsert.assign(lambdaBuffer.begin() + nPos, lambdaBuffer.begin() + nPos + nNumBytesToInsert);
	nPos += nNumBytesToInsert;

	std::unique_ptr<CInsertOperation> pInserOp(new CInsertOperation(dataToInsert));
	*pLambdaBufPos = nPos;
	return pInserOp;
}

///////////////////////////////////////////////////////////////////////////////
/// Gets the size of the serialised state
///
/// \return ULONG
///////////////////////////////////////////////////////////////////////////////
ULONG CInsertOperation::GetSerialisedSize()
{
	// ToDo: Update if serialisation is made more efficient
	return sizeof(BYTE) + sizeof(ULONG) + m_DataToInsert.size();
}

///////////////////////////////////////////////////////////////////////////////
/// Applies the insert operation - appends data payload to the output buffer
///
/// \param [in] const BYTEBUF& originalBuffer - ignored
/// \param [out] PBYTEBUF pOutputBuffer - the buffer to insert data into
///////////////////////////////////////////////////////////////////////////////
void CInsertOperation::ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer)
{
	//pOutputBuffer->reserve(pOutputBuffer->size() + m_DataToInsert.size());
	pOutputBuffer->insert(pOutputBuffer->end(), m_DataToInsert.begin(), m_DataToInsert.end());
}

}; // namespace lambda