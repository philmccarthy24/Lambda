//
//  LambdaIO.h
//  StaticLib
//
//  Two input/output interfaces and some concrete implementations
//  for convenience of writing to and reading from different
//  sorts of buffers in different contexts
//
//  Created by Phil McCarthy on 12/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#ifndef StaticLib_DataReader_h
#define StaticLib_DataReader_h

#include "Common.h"
#include "DataBuffer.h"

namespace lambda
{
    // generic data reader
    class IDataReader
    {
    public:
        virtual BYTE PeekByte() const = 0;
        virtual BYTE ReadByte() = 0;
        virtual ULONG PeekULONG() const = 0;
        virtual ULONG ReadULONG() = 0;
        virtual CDataBuffer ReadData(ULONG nNumBytes) = 0;
    };
    
    // generic data writer
    class IDataWriter
    {
    public:
        virtual void WriteByte(BYTE nValue) const = 0;
        virtual void WriteULONG(ULONG nValue) const = 0;
        virtual void WriteData(const CDataBuffer& dataBuffer) const = 0;
    };
    
    // class to read data from a CDataBuffer buffer wrapper object
    class CDataBufferReader : public IDataReader
    {
    public:
        CDataBufferReader(const CDataBuffer& bufferToRead) :
        m_dataBuffer(bufferToRead),
        m_nReadPos(0)
        {
        }
        
        virtual BYTE PeekByte() const
        {
            return *(m_dataBuffer.Buffer() + m_nReadPos);
        }
        
        virtual BYTE ReadByte()
        {
            BYTE nByte = PeekByte();
            m_nReadPos++;
            return nByte;
        }
        
        virtual ULONG PeekULONG() const
        {
            return *reinterpret_cast<PULONG>((m_dataBuffer.Buffer() + m_nReadPos));
        }
        
        virtual ULONG ReadULONG()
        {
            ULONG nValue = PeekULONG();
            m_nReadPos += sizeof(ULONG);
            return nValue;
        }
        
        virtual CDataBuffer ReadData(ULONG nNumBytes)
        {
            CDataBuffer readData = m_dataBuffer.GetSection(m_nReadPos, nNumBytes);
            m_nReadPos += nNumBytes;
            return readData;
        }
        
    private:
        CDataBuffer m_dataBuffer;
        ULONG m_nReadPos;
    };
    
    // class to write specifically to an underlying byte vector
    class CVectorWriter : public IDataWriter
    {
    public:
        CVectorWriter(BYTEVECTOR& dataBuffer) :
        m_dataBuffer(dataBuffer)
        {
        }
        
        virtual void WriteByte(BYTE nValue) const
        {
            m_dataBuffer.push_back(nValue);
        }
        
        virtual void WriteULONG(ULONG nValue) const
        {
            size_t nVecSz = m_dataBuffer.size();
            m_dataBuffer.resize(nVecSz + sizeof(ULONG));
            std::memcpy(m_dataBuffer.data() + nVecSz, &nValue, sizeof(nValue));
        }
        
        virtual void WriteData(const CDataBuffer& dataBuffer) const
        {
            m_dataBuffer.insert(m_dataBuffer.end(),
                                dataBuffer.Buffer(),
                                dataBuffer.Buffer() + dataBuffer.Size());
        }
        
    private:
        BYTEVECTOR& m_dataBuffer;
    };
    
    
};

#endif
