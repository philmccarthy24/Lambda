//
//  DataBuffer.h
//  StaticLib
//
//  Created by Phil McCarthy on 10/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#ifndef __StaticLib__DataBuffer__
#define __StaticLib__DataBuffer__

#include <iostream>

namespace lambda
{
    class CDataBuffer
    {
    public:
        CDataBuffer(BYTEVECTOR& dataBuffer);
        CDataBuffer(const PBYTE pBuffer, ULONG nBufLen);
        CDataBuffer(const CDataBuffer& other);
        CDataBuffer();
        
        CDataBuffer& operator =(const CDataBuffer& other);
        
        ULONG CalcOffset(const PBYTE pInBuffer) const;
        
        CDataBuffer GetSection(ULONG nOffset, ULONG nLength) const;
        CDataBuffer GetSection(ULONG nOffset) const;
        
        PBYTE Buffer() const;
        ULONG Size() const;
        
        bool IsEmpty() const;
        bool IsEqualBytes(const CDataBuffer& dataBuffer) const;
        
    private:        
        PBYTE m_pBuffer;
        ULONG m_nLength;
    };
};

#endif /* defined(__StaticLib__DataBuffer__) */
