#include "stdafx.h"
#include "InsertOperation.h"

const BYTE CInsertOperation::INSERT_OPERATION_TYPE = 1;

CInsertOperation::CInsertOperation(const BYTEBUF& dataToInsert) :
	m_DataToInsert(dataToInsert)
{
}

CInsertOperation::CInsertOperation()
{
}

CInsertOperation::~CInsertOperation(void)
{
}

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

void CInsertOperation::Deserialise(const BYTEBUF& lambdaBuffer, PULONG pLambdaBufPos)
{
	ULONG nPos = *pLambdaBufPos;
	//TODO: throw exception if lambdaBuffer[*pLambdaBufPos] != INSERT_OPERATION_TYPE
	nPos++;
	// get length
	ULONG nNumBytesToInsert = *(reinterpret_cast<const ULONG*>(lambdaBuffer.data() + nPos));
	nPos += sizeof(ULONG);
	// get bytes to insert
	m_DataToInsert.clear();
	m_DataToInsert.assign(lambdaBuffer.begin() + nPos, lambdaBuffer.begin() + nPos + nNumBytesToInsert);
	nPos += nNumBytesToInsert;
	*pLambdaBufPos = nPos;
}

ULONG CInsertOperation::GetSerialisedSize()
{
	// ToDo: Update if serialisation is made more efficient
	return sizeof(BYTE) + sizeof(ULONG) + m_DataToInsert.size();
}

bool CInsertOperation::isInsertOperation(BYTE nByteToCheck)
{
	return nByteToCheck == INSERT_OPERATION_TYPE;
}

void CInsertOperation::ApplyLambda(const BYTEBUF& originalBuffer, PBYTEBUF pOutputBuffer)
{
	//pOutputBuffer->reserve(pOutputBuffer->size() + m_DataToInsert.size());
	pOutputBuffer->insert(pOutputBuffer->end(), m_DataToInsert.begin(), m_DataToInsert.end());
}