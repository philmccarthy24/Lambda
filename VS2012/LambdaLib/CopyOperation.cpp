////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an operation to copy data from one buffer to another
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CopyOperation.h"

const BYTE CCopyOperation::COPY_OPERATION_TYPE = 0;

///////////////////////////////////////////////////////////////////////////////
/// Main constructor
///
/// \param [in] ULONG nCopyFromOffset - offset to copy from
/// \param [in] ULONG nNumBytesToCopy - number of bytes to copy
///////////////////////////////////////////////////////////////////////////////
CCopyOperation::CCopyOperation(ULONG nCopyFromOffset, ULONG nNumBytesToCopy) :
	m_nCopyFromOffset(nCopyFromOffset),
	m_nNumBytesToCopy(nNumBytesToCopy)
{
}

///////////////////////////////////////////////////////////////////////////////
/// Default constructor
///
///////////////////////////////////////////////////////////////////////////////
CCopyOperation::CCopyOperation()
{
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor
///
///////////////////////////////////////////////////////////////////////////////
CCopyOperation::~CCopyOperation(void)
{
}

///////////////////////////////////////////////////////////////////////////////
/// Serialises the copy operation to a buffer
///
/// \param [out] PBYTEBUF pLambdaBuffer - the buffer to serialise to
///////////////////////////////////////////////////////////////////////////////
void CCopyOperation::Serialise(PBYTEBUF pLambdaBuffer)
{
	// first output the type. We can hone this to be a bit.
	pLambdaBuffer->push_back(COPY_OPERATION_TYPE);
	// then output length
	size_t nLambdaSz = pLambdaBuffer->size();
	pLambdaBuffer->resize(nLambdaSz + sizeof(m_nNumBytesToCopy));
	std::memcpy(pLambdaBuffer->data() + nLambdaSz, &m_nNumBytesToCopy, sizeof(m_nNumBytesToCopy));
	// then output offset
	nLambdaSz = pLambdaBuffer->size();
	pLambdaBuffer->resize(nLambdaSz + sizeof(m_nCopyFromOffset));
	std::memcpy(pLambdaBuffer->data() + nLambdaSz, &m_nCopyFromOffset, sizeof(m_nCopyFromOffset));
}

///////////////////////////////////////////////////////////////////////////////
/// Reads the copy operation state from a buffer
///
/// \param [in] const BYTEBUF& lambdaBuffer - the buffer to read from
/// \param [in,out] PULONG pLambdaBufPos - the position in the buffer to read from
///////////////////////////////////////////////////////////////////////////////
void CCopyOperation::Deserialise(const BYTEBUF& lambdaBuffer, PULONG pLambdaBufPos)
{
	ULONG nPos = *pLambdaBufPos;
	//TODO: throw exception if lambdaBuffer[*pLambdaBufPos] != COPY_OPERATION_TYPE
	nPos++;
	// get length
	m_nNumBytesToCopy = *(reinterpret_cast<const ULONG*>(lambdaBuffer.data() + nPos));
	nPos += sizeof(ULONG);
	// get dictionary buffer offset to copy from
	m_nCopyFromOffset = *(reinterpret_cast<const ULONG*>(lambdaBuffer.data() + nPos));
	nPos += sizeof(ULONG);
	*pLambdaBufPos = nPos;
}

///////////////////////////////////////////////////////////////////////////////
/// Gets the size of the serialised state
///
/// \return ULONG
///////////////////////////////////////////////////////////////////////////////
ULONG CCopyOperation::GetSerialisedSize()
{
	// ToDo: Update if serialisation is made more efficient
	return sizeof(BYTE) + sizeof(ULONG) + sizeof(ULONG);
}

///////////////////////////////////////////////////////////////////////////////
/// Gets whether the byte containing operation type information corresponds
/// to a copy operation
///
/// \param [in] BYTE nByteToCheck - the byte to check
/// \return bool - whether the byte represents a copy operation
///////////////////////////////////////////////////////////////////////////////
bool CCopyOperation::isCopyOperation(BYTE nByteToCheck)
{
	// TODO: update function to use bit operations if serialisation is made more efficient
	return nByteToCheck == COPY_OPERATION_TYPE;
}

///////////////////////////////////////////////////////////////////////////////
/// Applies the copy operation from the original buffer to the output buffer
///
/// \param [in] const BYTEBUF& originalBuffer - the buffer to copy from
/// \param [out] PBYTEBUF pOutputBuffer 0 the buffer to copy to
///////////////////////////////////////////////////////////////////////////////
void CCopyOperation::ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer)
{
	size_t nOutputBufSz = pOutputBuffer->size();
	pOutputBuffer->reserve(nOutputBufSz + m_nNumBytesToCopy);
	pOutputBuffer->insert(pOutputBuffer->end(), 
						  originalBuffer.begin() + m_nCopyFromOffset, 
						  originalBuffer.begin() + m_nCopyFromOffset + m_nNumBytesToCopy);
}