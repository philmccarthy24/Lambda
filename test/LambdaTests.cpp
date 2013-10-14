//
//  unittests.cpp
//  LambdaTest
//
//  Created by Phil McCarthy on 05/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#define CATCH_CONFIG_MAIN
#include "LambdaTests.h"
#include "catch/catch.hpp"
#include "MLZ03Codec.h"
#include "CopyOperation.h"
#include "InsertOperation.h"
#include "DataBuffer.h"

using namespace lambda;

void AnalyseLambdaBuffer(const BYTEVECTOR& lambdaBuffer);

TEST_CASE( "Lambda basic encode and decode", "[CMLZ03Encoder]" )
{
    CDataBuffer testStringBuf((const PBYTE)&TEST_STRING[0], TEST_STRING.size());
    CDataBuffer updatedStringBuf((const PBYTE)&UPDATED_STRING[0], UPDATED_STRING.size());
    
    CMLZ03Codec codec;
    // do the difference encode
    CDataBuffer lambdaBuf = codec.EncodeBuffer(testStringBuf, updatedStringBuf);
    
    // look at the diff buffer
    /*
    AnalyseLambdaBuffer(lambdaBuf);
    std::cout << std::endl;
    std::cout <<  "Modified buf=" << updatedBuf.size() << " bytes, Lambda buf=" << lambdaBuf.size() << " bytes, ";
    double dbCR = 100 - ((double)lambdaBuf.size() / (double)updatedBuf.size()) * 100;
    std::cout << "Compression ratio = " << dbCR << "%." << std::endl;
    */
    
    // apply the lambda to the original buffer to get back the modified buffer
    CDataBuffer modifiedBuf = codec.DecodeBuffer(testStringBuf, lambdaBuf);
    std::string strAppliedString((char*)modifiedBuf.Buffer(), modifiedBuf.Size());
    
    // assert that the input for the operation is the same as the output
    REQUIRE( UPDATED_STRING == strAppliedString );
}

TEST_CASE( "Lambda compression size", "[CMLZ03Encoder]" )
{
    CDataBuffer testStringBuf((const PBYTE)&TEST_STRING[0], TEST_STRING.size());
    CDataBuffer updatedStringBuf((const PBYTE)&UPDATED_STRING[0], UPDATED_STRING.size());
    
    CMLZ03Codec codec;
    // do the difference encode
    CDataBuffer lambdaBuf = codec.EncodeBuffer(testStringBuf, updatedStringBuf);
    
    // assert that lambda compressed form has actually saved space compared to the data being encoded
    REQUIRE( lambdaBuf.Size() < updatedStringBuf.Size() );
}

/*
// print out the deserialised contents of the lambda buffer
void AnalyseLambdaBuffer(const BYTEVECTOR& lambdaBuffer)
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
*/