#include "stdafx.h"
#include "MLZ01Encoder.h"
#include "ILambdaOperation.h"
#include "CopyOperation.h"
#include "InsertOperation.h"
#include <memory>

CMLZ01Encoder::CMLZ01Encoder(void)
{
}


CMLZ01Encoder::~CMLZ01Encoder(void)
{
}

const BYTEBUF& CMLZ01Encoder::Encode(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer)
{
	// ToDo: param checks with exception throwing here

	m_LambdaBuffer->clear();
	std::vector<std::unique_ptr<ILambdaOperation>> lambdaOps;  
	ULONG nSymbolPosInModifiedBuf = 0;

	while (nSymbolPosInModifiedBuf < modifiedBuffer.size())
	{
		BYTEBUF symbol;
		symbol.push_back(modifiedBuffer[nSymbolPosInModifiedBuf]);

		ULONG nFoundSymbolPosInOriginalBuf = 0;
		bool bSymbolFound = false;
		while (bSymbolFound = FindSymbolInBuffer(symbol, originalBuffer, &nFoundSymbolPosInOriginalBuf) && 
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

//pFoundSymbolPosInOriginalBuf will be unmodified if symbol not found
bool CMLZ01Encoder::FindSymbolInBuffer(const BYTEBUF& symbol, const BYTEBUF& originalBuffer, PULONG pFoundSymbolPosInOriginalBuf)
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

virtual const BYTEBUF& CMLZ01Encoder::ApplyPatch(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer)
{
	// first deserialise lambda operations
	std::vector<std::unique_ptr<ILambdaOperation>> lambdaOps;
	ULONG nLambdaBufferPos = 0;
	while (nLambdaBufferPos < lambdaBuffer.size())
	{
		if (CCopyOperation::isCopyOperation(lambdaBuffer[nLambdaBufferPos]))
		{
			std::unique_ptr<ILambdaOperation> copyOp(new CCopyOperation());
			copyOp->Deserialise(lambdaBuffer, &nLambdaBufferPos);
			lambdaOps.push_back(std::move(copyOp));
		} 
		else if (CInsertOperation::isInsertOperation(lambdaBuffer[nLambdaBufferPos]))
		{
			std::unique_ptr<ILambdaOperation> insertOp(new CInsertOperation());
			insertOp->Deserialise(lambdaBuffer, &nLambdaBufferPos);
			lambdaOps.push_back(std::move(insertOp));
		}
		else
		{
			// TODO: error, type not recognised. throw exception
		}
	}

	// now iterate over each lambda op, applying it
	for (auto lambdaOpIter = lambdaOps.begin(); lambdaOpIter != lambdaOps.end(); lambdaOpIter++)
	{
		(*lambdaOpIter)->ApplyLambda(originalBuffer, &m_UpdatedBuffer);
	}
}