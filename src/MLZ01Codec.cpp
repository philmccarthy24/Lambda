////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an operation to copy data from one buffer to another
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#include "MLZ01Codec.h"
#include "LambdaOperation.h"
#include "CopyOperation.h"
#include "InsertOperation.h"
#include <iostream>
#include <memory>

namespace lambda
{

///////////////////////////////////////////////////////////////////////////////
/// Constructor
///
///////////////////////////////////////////////////////////////////////////////
CMLZ01Codec::CMLZ01Codec(void)
{
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor
///
///////////////////////////////////////////////////////////////////////////////
CMLZ01Codec::~CMLZ01Codec(void)
{
}

///////////////////////////////////////////////////////////////////////////////
/// Creates a difference coding between two input buffers
///
/// \param [in] const BYTEBUF& originalBuffer - a buffer of data
/// \param [in] const BYTEBUF& modifiedBuffer - data in originalBuffer, but
///												updated in some way
/// \return const BYTEBUF& - the lambda encoding
///////////////////////////////////////////////////////////////////////////////
const BYTEBUF& CMLZ01Codec::EncodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer)
{
	// ToDo: param checks with exception throwing here
	m_LambdaBuffer.clear();
	std::vector<std::unique_ptr<ILambdaOperation>> lambdaOps;  
	ULONG nSymbolPosInModifiedBuf = 0;

	while (nSymbolPosInModifiedBuf < modifiedBuffer.size())
	{
		BYTEBUF symbol;
		symbol.push_back(modifiedBuffer[nSymbolPosInModifiedBuf]);

		ULONG nFoundSymbolPosInOriginalBuf = 0;
		bool bSymbolFound = false;
		while ((bSymbolFound = FindSymbolInBuffer(symbol, originalBuffer, &nFoundSymbolPosInOriginalBuf)) &&
				nSymbolPosInModifiedBuf + symbol.size() < modifiedBuffer.size())
		{
			symbol.push_back(modifiedBuffer[nSymbolPosInModifiedBuf + symbol.size()]);
		}

		if (bSymbolFound)
		{
			// we must have matched fully to the end of modifiedBuffer
			// create a copy op with full symbol len
			lambdaOps.push_back(
				std::unique_ptr<ILambdaOperation>(
					new CCopyOperation(nFoundSymbolPosInOriginalBuf, symbol.size())
				)
			);
		} else {
			// nFoundSymbolPosInOriginalBuf unmodified since last successful find, so we can use it
			// to get position of last (best) length match
			if (symbol.size() > 1)
			{
				// last byte didn't match so remove it
				symbol.pop_back();
				// create a copy op
				lambdaOps.push_back(
					std::unique_ptr<ILambdaOperation>(
						new CCopyOperation(nFoundSymbolPosInOriginalBuf, symbol.size())
					)
				);
			} else {
				// no instance of symbol found (this is unusual, means byte code missing from originalBuffer)
				// create an insert using new byte code in symbol
				lambdaOps.push_back(
					std::unique_ptr<ILambdaOperation>(
						new CInsertOperation(symbol)
					)
				);
			}
		}

		nSymbolPosInModifiedBuf += symbol.size();
	}

	// ToDo: cost optimisation goes here

	// serialise the lambda operations to the output buffer
	for (auto lambdaOpIter = lambdaOps.begin(); lambdaOpIter != lambdaOps.end(); lambdaOpIter++)
	{
		(*lambdaOpIter)->Serialise(&m_LambdaBuffer);
	}

	return m_LambdaBuffer;
}

///////////////////////////////////////////////////////////////////////////////
/// Attempts to find a sequence of bytes in a buffer
/// pFoundSymbolPosInOriginalBuf will be unmodified if symbol not found.
///
/// \param [in] const BYTEBUF& symbol - the data to find
/// \param [in] const BYTEBUF& originalBuffer - the buffer to find it in
/// \param [out] PULONG pFoundSymbolPosInOriginalBuf - position symbol is found
/// \return bool - whether symbol was found or not.
///////////////////////////////////////////////////////////////////////////////
bool CMLZ01Codec::FindSymbolInBuffer(const BYTEBUF& symbol, const BYTEBUF& originalBuffer, PULONG pFoundSymbolPosInOriginalBuf)
{
	// TODO: param checks here: ensure symbol.size < originalBuffer.size, originalBuffer.size > 0 etc

	bool bFound = false;
	ULONG nOriginalPos = 0;
	for (; nOriginalPos < (originalBuffer.size() - symbol.size()) + 1; nOriginalPos++)
	{
		if (memcmp(originalBuffer.data() + nOriginalPos, symbol.data(), symbol.size()) == 0)
		{
			bFound = true;
			*pFoundSymbolPosInOriginalBuf = nOriginalPos;
			break;
		}
	}
	return bFound;
}

///////////////////////////////////////////////////////////////////////////////
/// Applies difference coding to an input buffer to generate an updated
/// (modified) buffer. Equivalent to "patching" a data buffer
///
/// \param [in] const BYTEBUF& originalBuffer - the input buffer
/// \param [in] const BYTEBUF& lambdaBuffer - the difference coding
/// \return const BYTEBUF& - the modified buffer
///////////////////////////////////////////////////////////////////////////////
const BYTEBUF& CMLZ01Codec::DecodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer)
{
	m_UpdatedBuffer.clear();
	ULONG nLambdaBufferPos = 0;
	while (nLambdaBufferPos < lambdaBuffer.size())
	{
		do // do-while-false flat handling construct
		{
			std::unique_ptr<CCopyOperation> pCopyOp = CCopyOperation::TryDeserialise(lambdaBuffer, &nLambdaBufferPos);
			if (pCopyOp != nullptr)
			{
				pCopyOp->ApplyLambda(originalBuffer, &m_UpdatedBuffer);
				break;
			}
			
			std::unique_ptr<CInsertOperation> pInsertOp = CInsertOperation::TryDeserialise(lambdaBuffer, &nLambdaBufferPos);
			if (pInsertOp != nullptr)
			{
				pInsertOp->ApplyLambda(originalBuffer, &m_UpdatedBuffer);
				break;
			}
			
			// TODO: if we're here, then we have an error (not recognised op). throw exception

		} while (false);

	}
	return m_UpdatedBuffer;
}

}; // namespace lambda