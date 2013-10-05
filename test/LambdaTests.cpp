//
//  unittests.cpp
//  LambdaTest
//
//  Created by Phil McCarthy on 05/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"
#include "MLZ01Codec.h"
#include "CopyOperation.h"
#include "InsertOperation.h"

void AnalyseLambdaBuffer(const BYTEBUF& lambdaBuffer);

TEST_CASE( "Lambda basic encode and decode", "[CMLZ01Encoder]" )
{
    std::string strTestString = "the quick brown fox jumps over the lazy dog.";
    std::string strUpdatedString = "the faster brown fox devours the lazy chicken.";
    
    BYTEBUF testBuf(strTestString.begin(), strTestString.end());
    BYTEBUF updatedBuf(strUpdatedString.begin(), strUpdatedString.end());
    
    lambda::CMLZ01Codec codec;
    // do the difference encode
    const BYTEBUF& lambdaBuf = codec.EncodeBuffer(testBuf, updatedBuf);
    
    // look at the diff buffer
    AnalyseLambdaBuffer(lambdaBuf);
    std::cout << "Lambda buffer is " << lambdaBuf.size() << " bytes long." << std::endl;
    
    // apply the lambda to the original buffer to get back the modified buffer
    const BYTEBUF& modifiedBuf = codec.DecodeBuffer(testBuf, lambdaBuf);
    std::string strAppliedString(modifiedBuf.begin(), modifiedBuf.end());
    
    // assert that the input for the operation is the same as the output
    REQUIRE( strUpdatedString == strAppliedString );
}

TEST_CASE( "Lambda compression size", "[CMLZ01Encoder]" )
{
    std::string strTestString = "the quick brown fox jumps over the lazy dog.";
    std::string strUpdatedString = "the faster brown fox devours the lazy chicken.";
    
    BYTEBUF testBuf(strTestString.begin(), strTestString.end());
    BYTEBUF updatedBuf(strUpdatedString.begin(), strUpdatedString.end());
    
    lambda::CMLZ01Codec codec;
    // do the difference encode
    const BYTEBUF& lambdaBuf = codec.EncodeBuffer(testBuf, updatedBuf);
    
    // assert that lambda compressed form has actually saved space compared to the data being encoded
    REQUIRE( lambdaBuf.size() < updatedBuf.size() );
}

// print out the deserialised contents of the lambda buffer
void AnalyseLambdaBuffer(const BYTEBUF& lambdaBuffer)
{
	ULONG nLambdaBufferPos = 0;
	while (nLambdaBufferPos < lambdaBuffer.size())
	{
		do // do-while-false flat handling construct
		{
			std::unique_ptr<lambda::CCopyOperation> pCopyOp = lambda::CCopyOperation::TryDeserialise(lambdaBuffer, &nLambdaBufferPos);
			if (pCopyOp != nullptr)
			{
				pCopyOp->Print();
				break;
			}
			
			std::unique_ptr<lambda::CInsertOperation> pInsertOp = lambda::CInsertOperation::TryDeserialise(lambdaBuffer, &nLambdaBufferPos);
			if (pInsertOp != nullptr)
			{
				pInsertOp->Print();
				break;
			}
            
		} while (false);
        
	}
}