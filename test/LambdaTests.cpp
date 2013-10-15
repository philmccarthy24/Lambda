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

///////////////////////////////////////////////////////////////////////////////
/// Utility function to convert std::string to CDataBuffer. Assumes STL string
/// implementation uses contiguous memory (very safe bet!)
///
/// \param [in] const std::string& stringRef - the string to convert to CDataBuffer
/// \return CDataBuffer - the converted string
///////////////////////////////////////////////////////////////////////////////
CDataBuffer StringToDataBuffer(const std::string& stringRef)
{
	return CDataBuffer((const lambda::PBYTE)&stringRef[0], stringRef.size());
}

///////////////////////////////////////////////////////////////////////////////
/// Utility function to convert CDataBuffer to std::string. Assumes STL string
/// implementation uses contiguous memory (very safe bet!)
///
/// \param [in] const CDataBuffer& dataBuffer - the buffer to convert to std::string
/// \return std::string - the converted buffer
///////////////////////////////////////////////////////////////////////////////
std::string DataBufferToString(const CDataBuffer& dataBuffer)
{
	return std::string((char*)dataBuffer.Buffer(), dataBuffer.Size());
}

///////////////////////////////////////////////////////////////////////////////
/// Utility function containing the boilerplate for taking a reference and test
/// string, creating lambda data from them, and then applying the lambda to the
/// reference string to get the result. Used repeatedly to test that strings can
/// be modified in different ways, and lambda encode/decode still works.
///
/// \param [in] const std::string& refString - the reference (original) string
/// \param [in] const std::string& testString - reString modified in some way
/// \return std::string - the result of the lambda decode.
///////////////////////////////////////////////////////////////////////////////
std::string TestEncodeDecode(const std::string& refString, const std::string& testString)
{
	CDataBuffer refStringBuf = StringToDataBuffer(refString);
    CDataBuffer testStringBuf = StringToDataBuffer(testString);
    
	// do the difference encode
    CMLZ03Codec codec;
    CDataBuffer lambdaBuf = codec.EncodeBuffer(refStringBuf, testStringBuf);
    
    // apply the lambda to the original buffer to get back the modified buffer
    CDataBuffer modifiedBuf = codec.DecodeBuffer(refStringBuf, lambdaBuf);
	return DataBufferToString(modifiedBuf);
}

TEST_CASE( "Test encoding string with inserted text at the beginning", "[CMLZ03Encoder]" )
{
	std::cout << std::endl << "Test encoding string with inserted text at the beginning" << std::endl;
    std::string decodedString = TestEncodeDecode(REFERENCE_STRING, INSERTION_AT_BEGINNING);
    REQUIRE( decodedString == INSERTION_AT_BEGINNING );
}

TEST_CASE( "Test encoding string with inserted text at the end", "[CMLZ03Encoder]" )
{
	std::cout << std::endl << "Test encoding string with inserted text at the end" << std::endl;
	std::string decodedString = TestEncodeDecode(REFERENCE_STRING, INSERTION_AT_END);
    REQUIRE( decodedString == INSERTION_AT_END );
}

TEST_CASE( "Test encoding string with end section moved to beginning", "[CMLZ03Encoder]" )
{
	std::cout << std::endl << "Test encoding string with end section moved to beginning" << std::endl;
	std::string decodedString = TestEncodeDecode(REFERENCE_STRING, SECTION_MOVE_TO_BEGINNING);
    REQUIRE( decodedString == SECTION_MOVE_TO_BEGINNING );
}

TEST_CASE( "Test encoding string with beginning part deleted", "[CMLZ03Encoder]" )
{
	std::cout << std::endl << "Test encoding string with beginning part deleted" << std::endl;
	std::string decodedString = TestEncodeDecode(REFERENCE_STRING, BEGINNING_DELETED);
    REQUIRE( decodedString == BEGINNING_DELETED );
}

TEST_CASE( "Test encoding string with end part deleted", "[CMLZ03Encoder]" )
{
	std::cout << std::endl << "Test encoding string with end part deleted" << std::endl;
	std::string decodedString = TestEncodeDecode(REFERENCE_STRING, END_DELETED);
    REQUIRE( decodedString == END_DELETED );
}

TEST_CASE( "Test encoding string with different insertions, rewrites and deletions", "[CMLZ03Encoder]" )
{
	std::cout << std::endl << "Test encoding string with different insertions, rewrites and deletions" << std::endl;
	std::string decodedString = TestEncodeDecode(REFERENCE_STRING, DELETIONS_AND_INSERTIONS);
    REQUIRE( decodedString == DELETIONS_AND_INSERTIONS );
}

TEST_CASE( "Lambda size is less than modified string", "[CMLZ03Encoder]" )
{
	std::cout << std::endl << "Lambda size is less than modified string" << std::endl;

    CDataBuffer refStringBuf = StringToDataBuffer(REFERENCE_STRING);
    CDataBuffer testStringBuf = StringToDataBuffer(DELETIONS_AND_INSERTIONS);
    
	// do the difference encode
    CMLZ03Codec codec;
    CDataBuffer lambdaBuf = codec.EncodeBuffer(refStringBuf, testStringBuf);
    
    // assert that lambda compressed form has actually saved space compared to the data being encoded
    REQUIRE( lambdaBuf.Size() < DELETIONS_AND_INSERTIONS.size() );
}

//TODO: Add tests for CPatchFile here