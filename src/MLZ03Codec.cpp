//
//  MLZ03Codec.cpp
//  StaticLib
//
//  Created by Phil McCarthy on 05/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#include "MLZ03Codec.h"
#include "LambdaOperation.h"
#include "CopyOperation.h"
#include "InsertOperation.h"
#include <iostream>
#include <assert.h>

namespace lambda
{
    
    // match threshold - if we get this number of matching bytes,
    // switch to binary search match algorithm. otherwise use linear
    // to find next match block
    const ULONG CMLZ03Codec::LARGE_MATCH_THRESHOLD = 10;
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Creates a difference coding between two input buffers
    ///
    /// \param [in] const CDataBuffer& originalBuffer - a buffer of data
    /// \param [in] const CDataBuffer& modifiedBuffer - data in originalBuffer, but
    ///												updated in some way
    /// \return CDataBuffer& - the lambda encoding
    ///////////////////////////////////////////////////////////////////////////////
    CDataBuffer CMLZ03Codec::EncodeBuffer(const CDataBuffer& originalBuffer, const CDataBuffer& modifiedBuffer)
    {
        // some param checking
        if (originalBuffer.Buffer() == modifiedBuffer.Buffer())
        {
            THROW_EXCEPTION(InvalidParamException, "Original buffer must be differnt from modified buffer");
        }
        if (originalBuffer.IsEmpty())
        {
            THROW_EXCEPTION(InvalidParamException, "Original buffer must be non-empty");
        }
        if (modifiedBuffer.IsEmpty())
        {
            THROW_EXCEPTION(InvalidParamException, "Modified buffer must be non-empty");
        }
        
        // create writer object to output to lambda vector
        m_LambdaBuffer.clear();
        CVectorWriter lambdaWriter(m_LambdaBuffer);
        
        // create an insert operation used to merge together inefficiently coded ops
        CInsertOperation mergedInsert;
        
        ULONG nEncodingOffset = 0;
        while (nEncodingOffset < modifiedBuffer.Size())
        {
            CDataBuffer matchBuffer = FindLongestMatch(modifiedBuffer.GetSection(nEncodingOffset), originalBuffer);
            //TODO: investigate limiting size of originalBuffer
            
            if (matchBuffer.IsEmpty())
            {
                // no match found (which should be fairly rare) - do a single byte insert
                matchBuffer = modifiedBuffer.GetSection(nEncodingOffset, 1);
                
                // don't need to calc cost - an insert of 1 is always better off merged
                mergedInsert.Merge(CInsertOperation(matchBuffer));
                nEncodingOffset++;
            }
            else
            {
                CCopyOperation copyOp(originalBuffer, matchBuffer);
                if (mergedInsert.CalcMergeCost(copyOp) > 0)
                {
                    // more space efficient to represent this operation as part of a merged insert
                    mergedInsert.Merge(copyOp);
                }
                else
                {
                    // the original lambda coding is more space efficient: emit rolling merged insert
                    // if there is one,
                    mergedInsert.Serialise(lambdaWriter);
                    mergedInsert.Reset();
                    
                    // now emit the current operation
                    copyOp.Serialise(lambdaWriter);
                }
                
                nEncodingOffset += matchBuffer.Size();
            }
        }
        
        // emit any merged operations left over if there are any
        mergedInsert.Serialise(lambdaWriter);
        
        return CDataBuffer(m_LambdaBuffer);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Find a sub-buffer in patternBuffer which is the longest match for
    /// sourceBuffer[0..n]
    ///
    /// \param [in] const CDataBuffer& sourceBuffer - the source (updated) buffer
    /// \param [out] const CDataBuffer& patternBuffer - the pattern (original) buffer
    /// \return CDataBuffer - the matching sub-buffer
    ///////////////////////////////////////////////////////////////////////////////
    CDataBuffer CMLZ03Codec::FindLongestMatch(const CDataBuffer& sourceBuffer, const CDataBuffer& patternBuffer)
    {
        assert(sourceBuffer.Buffer() != patternBuffer.Buffer());
        assert(!sourceBuffer.IsEmpty());
        assert(!patternBuffer.IsEmpty());
        
        ULONG nPatternPos = 0;
        ULONG nTryMatchLen = 1;
        CDataBuffer longestPattern;
        
        while (nPatternPos + nTryMatchLen <= patternBuffer.Size() &&
               nTryMatchLen <= sourceBuffer.Size()) // if not, longestPattern matches the entirety of sourceBuffer
        {
            CDataBuffer tryPattern = sourceBuffer.GetSection(0, nTryMatchLen);
            CDataBuffer matchPattern = patternBuffer.GetSection(nPatternPos, nTryMatchLen);
            if (tryPattern.IsEqualBytes(matchPattern))
            {
                if (longestPattern.IsEmpty())
                {
                    longestPattern = matchPattern;
                }
                else if (longestPattern.Size() < matchPattern.Size())
                {
                    longestPattern = matchPattern;
                }
                
                if (nTryMatchLen > LARGE_MATCH_THRESHOLD)
                {
                    CDataBuffer quickMatch = QuickMatch(sourceBuffer, patternBuffer.GetSection(nPatternPos));
                    if (quickMatch.Size() > longestPattern.Size())
                    {
                        longestPattern = quickMatch;
                    }
                    nTryMatchLen += quickMatch.Size() - LARGE_MATCH_THRESHOLD;  // not right
                }
                else
                {
                    nTryMatchLen++;
                }
                
            } else {
                // failed to match
                if (nTryMatchLen > 1)
                {
                    // move the pattern pos to the end of the last match
                    nPatternPos += nTryMatchLen - 1;
                }
                else
                {
                    // no last match - increment to next byte
                    nPatternPos++;
                }
                nTryMatchLen = 1;
            }
        }
        
        return longestPattern;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Find the largest number of matching bytes of sourceBuffer in
    /// patternBuffer
    ///
    /// \param [in] const CDataBuffer& sourceBuffer
    /// \param [out] const CDataBuffer& patternBuffer
    /// \return CDataBuffer - the matching sub-buffer
    ///////////////////////////////////////////////////////////////////////////////
    CDataBuffer CMLZ03Codec::QuickMatch(const CDataBuffer& sourceBuffer, const CDataBuffer& patternBuffer)
    {
        assert(sourceBuffer.Buffer() != patternBuffer.Buffer());
        assert(!sourceBuffer.IsEmpty());
        assert(!patternBuffer.IsEmpty());
        
        ULONG nMaxBytesMatchable = sourceBuffer.Size() < patternBuffer.Size() ? sourceBuffer.Size() : patternBuffer.Size();
        
        CDataBuffer tryMatch;
        ULONG nLastMatchLen = 0;
        ULONG nLastFailLen = nMaxBytesMatchable;
        ULONG nMatchAttemptLen = nMaxBytesMatchable;
        
        while (nMatchAttemptLen - nLastMatchLen > 1)  // not right
        {
            tryMatch = sourceBuffer.GetSection(0, nMatchAttemptLen);
            if (tryMatch.IsEqualBytes(patternBuffer.GetSection(0, nMatchAttemptLen)))
            {
                // we've matched. try to go higher
                nLastMatchLen = nMatchAttemptLen;
                nMatchAttemptLen += (nLastFailLen - nMatchAttemptLen) / 2;
                
            } else {
                // failed to match. go lower
                nLastFailLen = nMatchAttemptLen;
                nMatchAttemptLen -= (nMatchAttemptLen - nLastMatchLen) / 2;
            }
        }
        
        return tryMatch;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Applies difference coding to an input buffer to generate an updated
    /// (modified) buffer. Equivalent to "patching" a data buffer
    ///
    /// \param [in] const BYTEVECTOR& originalBuffer - the input buffer
    /// \param [in] const BYTEVECTOR& lambdaBuffer - the difference coding
    /// \return const BYTEVECTOR& - the modified buffer
    ///////////////////////////////////////////////////////////////////////////////
    CDataBuffer CMLZ03Codec::DecodeBuffer(const CDataBuffer& originalBuffer, const CDataBuffer& lambdaBuffer)
    {
        m_UpdatedBuffer.clear();
        CVectorWriter modifiedWriter(m_UpdatedBuffer);
        
        CDataBufferReader lambdaReader(lambdaBuffer);
        while (lambdaReader.BytesToRead() > 0)
        {
            do // do-while-false flat handling construct
            {
                std::unique_ptr<CCopyOperation> pCopyOp = CCopyOperation::TryDeserialise(originalBuffer, lambdaReader);
                if (pCopyOp != nullptr)
                {
                    pCopyOp->Apply(modifiedWriter);
                    break;
                }
                
                std::unique_ptr<CInsertOperation> pInsertOp = CInsertOperation::TryDeserialise(lambdaReader);
                if (pInsertOp != nullptr)
                {
                    pInsertOp->Apply(modifiedWriter);
                    break;
                }
                
                // error (not recognised op).
                THROW_EXCEPTION(SerialisationException, "Could not deserialise next lambda operation : parse error.");
                
            } while (false);
            
        }
        return CDataBuffer(m_UpdatedBuffer);
    }
    
}; // namespace lambda