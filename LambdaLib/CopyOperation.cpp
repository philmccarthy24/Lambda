#include "stdafx.h"
#include "CopyOperation.h"

const BYTE CCopyOperation::COPY_OPERATION_TYPE = 0;

CCopyOperation::CCopyOperation(ULONG nCopyFromOffset, ULONG nNumBytesToCopy) :
	m_nCopyFromOffset(nCopyFromOffset),
	m_nNumBytesToCopy(nNumBytesToCopy)
{
}

CCopyOperation::CCopyOperation()
{
}

CCopyOperation::~CCopyOperation(void)
{
}

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

ULONG CCopyOperation::GetSerialisedSize()
{
	// ToDo: Update if serialisation is made more efficient
	return sizeof(BYTE) + sizeof(ULONG) + sizeof(ULONG);
}

bool CCopyOperation::isCopyOperation(BYTE nByteToCheck)
{
	// TODO: update function to use bit operations if serialisation is made more efficient
	return nByteToCheck == COPY_OPERATION_TYPE;
}

void CCopyOperation::ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer)
{
	size_t nOutputBufSz = pOutputBuffer->size();
	pOutputBuffer->reserve(nOutputBufSz + m_nNumBytesToCopy);
	pOutputBuffer->insert(pOutputBuffer->end(), 
						  originalBuffer.begin() + m_nCopyFromOffset, 
						  originalBuffer.begin() + m_nCopyFromOffset + m_nNumBytesToCopy);
}