////////////////////////////////////////////////////////////////////////////////////////////////
/// Common definitions etc
///
/// \author Phil McCarthy
/// \date September 2013
/// \copyright 2013 Phil McCarthy
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _COMMON_STUFF_H_
#define _COMMON_STUFF_H_

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#define DEBUG_OUTPUT

namespace lambda
{

    typedef unsigned long ULONG, *PULONG;
    typedef unsigned char BYTE, *PBYTE;
    typedef std::vector<BYTE> BYTEVECTOR, *PBYTEVECTOR;
    
    #define THROW_EXCEPTION(ExceptionClass, causeString) \
        LambdaBaseException* p = (ExceptionClass*)0;p++;  /*enforce that ExceptionClass is derived from LambdaBaseException */  \
        throw ExceptionClass(causeString, __FILE__, __LINE__);
    
    class LambdaBaseException : public std::exception
    {
    public:
        LambdaBaseException(const std::string& strCause, const std::string& strFile, const int nLineNum)
        {
            m_strCause = strCause;
            
            std::stringstream ss;
            ss << strFile << ": " << nLineNum;
            m_strLocation = ss.str();
            
            std::stringstream tsss;
            tsss << __DATE__ << " : " << __TIME__;
            m_strTimestamp = tsss.str();
        }
        
        virtual const char* what() const throw()
        {
            return m_strCause.c_str();
        }
        
        virtual const std::string& GetLocation() const throw()
        {
            return m_strLocation;
        }
        
        virtual const std::string GetTimestamp() const throw()
        {
            return m_strTimestamp;
        }
        
    private:
        
        std::string m_strCause;
        std::string m_strLocation;
        std::string m_strTimestamp;
    };
    
    class InvalidParamException : public LambdaBaseException
    {
    public:
        InvalidParamException(const std::string& strCause, const std::string& strFile, const int nLineNum) :
        LambdaBaseException(strCause, strFile, nLineNum)
        {
        }
        
    };
    
    class SerialisationException : public LambdaBaseException
    {
    public:
        SerialisationException(const std::string& strCause, const std::string& strFile, const int nLineNum) :
        LambdaBaseException(strCause, strFile, nLineNum)
        {
        }
        
    };
    
    class FileIOException : public LambdaBaseException
    {
    public:
        FileIOException(const std::string& strCause, const std::string& strFile, const int nLineNum) :
        LambdaBaseException(strCause, strFile, nLineNum)
        {
        }
        
    };
    
};

#endif