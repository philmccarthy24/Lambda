//
//  MLZ02Codec.cpp
//  StaticLib
//
//  Created by Phil McCarthy on 05/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#include "MLZ02Codec.h"
#include "LambdaOperation.h"
#include "CopyOperation.h"
#include "InsertOperation.h"
#include <iostream>

namespace lambda
{
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Constructor
    ///
    ///////////////////////////////////////////////////////////////////////////////
    CMLZ02Codec::CMLZ02Codec(void)
    {
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ///////////////////////////////////////////////////////////////////////////////
    CMLZ02Codec::~CMLZ02Codec(void)
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
    const BYTEBUF& CMLZ02Codec::EncodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& modifiedBuffer)
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
        
        // Optimise lambda encoding by merging space inefficient lambda ops into single data insertion block.
        // Note this could be optimised a hell of a lot more (do the below in-line above for starters, and stop all the data copying)
        BYTEBUF mergedInsertData;
        ULONG nInsertOpHdrSz = sizeof(BYTE) + sizeof(ULONG);
        for (auto i = lambdaOps.begin(); i != lambdaOps.end(); i++)
        {
            ULONG nMergedInsertOpSz = mergedInsertData.empty() ? 0 : nInsertOpHdrSz + mergedInsertData.size();
            ULONG nFirstInsertOpHdr = mergedInsertData.empty() ? nInsertOpHdrSz : 0;
            // compare prev merged inserts plus op coded lambda versus everything as a merged insert
            ULONG nMergedPlusCodedSz = nMergedInsertOpSz + (*i)->Size();
            ULONG nAllMergedAsInsertSz = nFirstInsertOpHdr + nMergedInsertOpSz + (*i)->Size(lambda::E_CTX_ORIGINAL_DATA);
            if (nMergedPlusCodedSz > nAllMergedAsInsertSz)
            {
                // the insert is more space efficient, so put data in merge buffer
                (*i)->ApplyLambda(originalBuffer, &mergedInsertData);
            } else {
                // the original lambda coding is more space efficient: emit rolling merged insert
                // if there is one,
                if (!mergedInsertData.empty())
                {
                    CInsertOperation insertOp(mergedInsertData);
                    insertOp.Serialise(&m_LambdaBuffer);
                    mergedInsertData.clear();
                }
                // now emit the current operation
                (*i)->Serialise(&m_LambdaBuffer);
            }
        }
        if (!mergedInsertData.empty())
        {
            // edge case - it's more efficient to code the entire
            // data set as a single insert operation!
            CInsertOperation insertOp(mergedInsertData);
            insertOp.Serialise(&m_LambdaBuffer);
            mergedInsertData.clear();
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
    bool CMLZ02Codec::FindSymbolInBuffer(const BYTEBUF& symbol, const BYTEBUF& originalBuffer, PULONG pFoundSymbolPosInOriginalBuf)
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
    const BYTEBUF& CMLZ02Codec::DecodeBuffer(const BYTEBUF& originalBuffer, const BYTEBUF& lambdaBuffer)
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