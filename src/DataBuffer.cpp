//
//  DataBuffer.cpp
//  StaticLib
//
//  Created by Phil McCarthy on 10/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#include "Common.h"
#include "DataBuffer.h"

namespace lambda
{
    CDataBuffer::CDataBuffer(BYTEVECTOR& dataBuffer)
    {
        m_pBuffer = dataBuffer.data();
        m_nLength = dataBuffer.size();
    }
    
    CDataBuffer::CDataBuffer(PBYTE pBuffer, ULONG nBufLen)
    {
        m_pBuffer = pBuffer;
        m_nLength = nBufLen;
    }

    CDataBuffer::CDataBuffer(const CDataBuffer& other)
    {
        operator=(other);
    }

    CDataBuffer::CDataBuffer() :
    m_pBuffer(nullptr),
    m_nLength(0)
    {
    }

    CDataBuffer& CDataBuffer::operator =(const CDataBuffer& other)
    {
        m_pBuffer = other.m_pBuffer;
        m_nLength = other.m_nLength;
        return *this;
    }

    ULONG CDataBuffer::CalcOffset(const PBYTE pInBuffer) const
    {
        return m_pBuffer - pInBuffer;
    }
    
    CDataBuffer CDataBuffer::GetSection(ULONG nOffset, ULONG nLength) const
    {
        if (nOffset > m_nLength)
        {
            THROW_EXCEPTION(InvalidParamException, "Specified offset exeeds buffer limits");
            
        }
        if (nLength > m_nLength - nOffset)
        {
            THROW_EXCEPTION(InvalidParamException, "Specified length exeeds buffer limits");
        }
        return CDataBuffer(m_pBuffer + nOffset, nLength);
    }
    
    CDataBuffer CDataBuffer::GetSection(ULONG nOffset) const
    {
        return GetSection(nOffset, m_nLength - nOffset);
    }

    PBYTE CDataBuffer::Buffer() const
    {
        return m_pBuffer;
    }

    ULONG CDataBuffer::Size() const
    {
        return m_nLength;
    }
    
    bool CDataBuffer::IsEmpty() const
    {
        return m_nLength == 0;
    }
    
    bool CDataBuffer::IsEqualBytes(const CDataBuffer& dataBuffer) const
    {
        if (m_nLength != dataBuffer.Size())
        {
            THROW_EXCEPTION(InvalidParamException, "Specified buffer has a different length");
        }
        return memcmp(m_pBuffer, dataBuffer.Buffer(), m_nLength) == 0;
    }
};