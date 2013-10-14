////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an operation to insert data into a buffer
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#include "LambdaIO.h"
#include "InsertOperation.h"

namespace lambda
{

    const BYTE CInsertOperation::INSERT_OPERATION_TYPE = 1;

    ///////////////////////////////////////////////////////////////////////////////
    /// Main constructor
    ///
    /// \param [in] const CDataBuffer& dataToInsert - the data to insert
    ///////////////////////////////////////////////////////////////////////////////
    CInsertOperation::CInsertOperation(const CDataBuffer& dataToInsert)
    {
        m_DataToInsert.insert(m_DataToInsert.end(),
                              dataToInsert.Buffer(),
                              dataToInsert.Buffer() + dataToInsert.Size());
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Default Constructor
    ///
    ///////////////////////////////////////////////////////////////////////////////
    CInsertOperation::CInsertOperation()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Serialises the insert operation to a buffer. If the insert operation contains
    /// no data to insert, this function returns without doing anything
    ///
    /// \param [out] PBYTEBUF pLambdaBuffer - the buffer to serialise to
    ///////////////////////////////////////////////////////////////////////////////
    void CInsertOperation::Serialise(const IDataWriter& lambdaWriter) const
    {
        if (!m_DataToInsert.empty())
        {
            // first output the type. We can hone this to be a bit.
            lambdaWriter.WriteByte(INSERT_OPERATION_TYPE);
            
            // then output length
            lambdaWriter.WriteULONG(m_DataToInsert.size());
            
            // then output data
            const CDataBuffer lambdaBuf(const_cast<const PBYTE>(m_DataToInsert.data()), m_DataToInsert.size());
            lambdaWriter.WriteData(lambdaBuf);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Reads the insert operation state from a buffer
    ///
    /// \param [in] const BYTEVECTOR& lambdaBuffer - the buffer to read from
    /// \param [in,out] PULONG pLambdaBufPos - the position in the buffer to read from
    /// \return std::unique_ptr<CInsertOperation> - the deserialised insert operation,
    ///			or nullptr if lambdaBuffer did not contain a CInsertOperation at
    ///			position *pLambdaBufPos
    ///////////////////////////////////////////////////////////////////////////////
    std::unique_ptr<CInsertOperation> CInsertOperation::TryDeserialise(IDataReader& lambdaReader)
    {
        if (lambdaReader.PeekByte() != INSERT_OPERATION_TYPE)
        {
            // not an insert operation
            return nullptr;
        }
        lambdaReader.ReadByte();
        
        // get length
        ULONG nNumBytesToInsert = lambdaReader.ReadULONG();
        
        // get bytes to insert and use to create a new InsertOperation
        std::unique_ptr<CInsertOperation> pInserOp(
            new CInsertOperation(
                lambdaReader.ReadData(nNumBytesToInsert)
            )
        );
        
#ifdef DEBUG_OUTPUT
        std::cout << "<InsertOp><Length>" << nNumBytesToInsert << "</Length></InsertOp>" << std::endl;
#endif
        
        return pInserOp;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Gets the lambda-serialised size
    ///
    /// \return ULONG - the serialised size
    ///////////////////////////////////////////////////////////////////////////////
    ULONG CInsertOperation::ObjectSize() const
    {
        // ToDo: Update if serialisation is made more efficient
        return GetHdrSize() + m_DataToInsert.size();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Gets the decoded size
    ///
    /// \return ULONG - the decoded block size
    ///////////////////////////////////////////////////////////////////////////////
    ULONG CInsertOperation::WriteSize() const
    {
        return m_DataToInsert.size();
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Applies the insert operation - appends data payload to the output buffer
    ///
    /// \param [in] const BYTEVECTOR& originalBuffer - ignored
    /// \param [out] PBYTEBUF pOutputBuffer - the buffer to insert data into
    ///////////////////////////////////////////////////////////////////////////////
    void CInsertOperation::Apply(const IDataWriter& outputWriter) const
    {
        const CDataBuffer insertWriteBuf(const_cast<const PBYTE>(m_DataToInsert.data()), m_DataToInsert.size());
        outputWriter.WriteData(insertWriteBuf);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Gets the serialised operation header size
    ///
    /// \return ULONG - header size
    ///////////////////////////////////////////////////////////////////////////////
    ULONG CInsertOperation::GetHdrSize()
    {
        return sizeof(BYTE) + sizeof(ULONG);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Calculates the cost of combining specified operation into this insert.
    ///
    /// \return ULONG - cost of merging. > 0 means bytes saved by merging
    ///////////////////////////////////////////////////////////////////////////////
    int CInsertOperation::CalcMergeCost(const ILambdaOperation& operationToMerge) const
    {
        ULONG nMergedInsertOpSz = m_DataToInsert.empty() ? 0 : ObjectSize();
        ULONG nFirstInsertOpHdr = m_DataToInsert.empty() ? GetHdrSize() : 0;
        // compare [prev merged ops] plus [op coded lambda], versus [everything as a merged insert]
        ULONG nMergedPlusCodedSz = nMergedInsertOpSz + operationToMerge.ObjectSize();
        ULONG nAllMergedAsInsertSz = nFirstInsertOpHdr + nMergedInsertOpSz + operationToMerge.WriteSize();
        int nMergeCost = static_cast<int>(nMergedPlusCodedSz) - static_cast<int>(nAllMergedAsInsertSz);
        return nMergeCost;
    }
    
    void CInsertOperation::Merge(const ILambdaOperation& operationToMerge)
    {
        CVectorWriter insertVecWriter(m_DataToInsert);
        operationToMerge.Apply(insertVecWriter);
    }
    
    void CInsertOperation::Reset()
    {
        m_DataToInsert.clear();
    }


}; // namespace lambda