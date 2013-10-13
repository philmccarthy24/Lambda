////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an operation to copy data from one buffer to another
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#include "CopyOperation.h"

namespace lambda
{

    const BYTE CCopyOperation::COPY_OPERATION_TYPE = 0;

    ///////////////////////////////////////////////////////////////////////////////
    /// Main constructor
    ///
    /// \param [in] ULONG nCopyFromOffset - offset to copy from
    /// \param [in] ULONG nNumBytesToCopy - number of bytes to copy
    ///////////////////////////////////////////////////////////////////////////////
    CCopyOperation::CCopyOperation(const CDataBuffer& copyFromBuffer, const CDataBuffer& dataToCopy) :
        m_dataToCopy(dataToCopy),
        m_copyFromBuffer(copyFromBuffer)
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Serialises the copy operation to a buffer
    ///
    /// \param [out] PBYTEBUF pLambdaBuffer - the buffer to serialise to
    ///////////////////////////////////////////////////////////////////////////////
    void CCopyOperation::Serialise(const IDataWriter& lambdaWriter)
    {
        // first output the type. We can hone this to be a bit.
        lambdaWriter.WriteByte(COPY_OPERATION_TYPE);
        
        // then output length
        lambdaWriter.WriteULONG(m_dataToCopy.Size());
        
        // then output offset
        lambdaWriter.WriteULONG(m_dataToCopy.CalcOffset(m_copyFromBuffer.Buffer()));
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Reads the copy operation state from a buffer
    ///
    /// \param [in] const BYTEVECTOR& lambdaBuffer - the buffer to read from
    /// \param [in,out] PULONG pLambdaBufPos - the position in the buffer to read from
    /// \return std::unique_ptr<CCopyOperation> - the deserialised copy operation,
    ///			or nullptr if lambdaBuffer did not contain a CCopyOperation at
    ///			position *pLambdaBufPos
    ///////////////////////////////////////////////////////////////////////////////
    std::unique_ptr<CCopyOperation> CCopyOperation::TryDeserialise(const CDataBuffer& copyFromBuffer, IDataReader& lambdaReader)
    {
        // check the type of the operation
        if (lambdaReader.PeekByte() != COPY_OPERATION_TYPE)
        {
            // not a copy op
            return nullptr;
        }
        lambdaReader.ReadByte();
        
        // get length
        ULONG nNumBytesToCopy = lambdaReader.ReadULONG();

        // get dictionary buffer offset to copy from
        ULONG nOffset = lambdaReader.ReadULONG();

        std::unique_ptr<CCopyOperation> pCopyOp(
            new CCopyOperation(
                copyFromBuffer,
                CDataBuffer(copyFromBuffer.Buffer() + nOffset, nNumBytesToCopy)
            )
        );
        return pCopyOp;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Gets the lambda-serialised or decoded size
    ///
    /// \return ULONG - the serialised size
    ///////////////////////////////////////////////////////////////////////////////
    ULONG CCopyOperation::ObjectSize() const
    {
        // ToDo: Update if serialisation is made more efficient
        return sizeof(BYTE) + sizeof(ULONG) + sizeof(ULONG);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// Gets the decoded size
    ///
    /// \return ULONG - the decoded block size
    ///////////////////////////////////////////////////////////////////////////////
    ULONG CCopyOperation::WriteSize() const
    {
        return m_dataToCopy.Size();
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Applies the copy operation from the original buffer to the output buffer
    ///
    /// \param [in] const BYTEVECTOR& originalBuffer - the buffer to copy from
    /// \param [out] PBYTEBUF pOutputBuffer 0 the buffer to copy to
    ///////////////////////////////////////////////////////////////////////////////
    void CCopyOperation::Apply(const IDataWriter& outputWriter)
    {
        outputWriter.WriteData(m_dataToCopy);
    }

}; // namespace lambda