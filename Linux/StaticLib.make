
#
#----------------------------------------------------------------------- 
#
# This is the lambda makefile. It builds lambda as a
# static library. See the documentation for more information.
#
# Type 'make -f StaticLib.make' to build the lib.
# Type 'make -f StaticLib.make install' to build and install the lib.
# Type 'make -f StaticLib.make clean' to clean the object files
#
# Note for FreeBSD users:
# use gmake from "/usr/ports/devel/gmake"
#
# Phil McCarthy
# 20 Oct 2013
#
#----------------------------------------------------------------------- 
# Edit the following lines to meet your needs

# Path to install the headerfiles
INCLUDE_PATH = /usr/include/lambda

# Path to install the library
LIB_PATH = /usr/local/lib/

# Compiler to use
COMPILER = g++

# Global options for the compiler
COPTIONS = -ansi -Wall -Wextra -std=c++11

# include paths
INC = -I../src -I../lib/sha1

#----------------------------------------------------------------------- 
# DON'T CHANGE ANYTHING BELOW
#----------------------------------------------------------------------- 

ifdef DEBUG
	COPTIONS += -g
else
	COPTIONS += -O3 -fomit-frame-pointer
endif

GCC = $(COMPILER) $(COPTIONS) $(INC)

#----------------------------------------------------------------------- 
#Main-Target
all:		SHA1 LAMBDACORE LIB

#----------------------------------------------------------------------- 
#all header-files

		
HEADER = 	../src/Common.h \
		../src/DataBuffer.h \
		../src/LambdaCodec.h \
		../src/MLZ03Codec.h \
		../src/PatchFile.h

#----------------------------------------------------------------------- 
# LAMBDACORE Targets

LAMBDACORE = 	CopyOperation.o \
		InsertOperation.o \
		DataBuffer.o \
		MLZ03Codec.o \
		PatchFile.o

LAMBDACORE:	CopyOperation.o InsertOperation.o DataBuffer.o MLZ03Codec.o PatchFile.o

CopyOperation.o:   ../src/CopyOperation.cpp ../src/CopyOperation.h
			$(GCC) -c ../src/CopyOperation.cpp
			
InsertOperation.o: ../src/InsertOperation.cpp ../src/InsertOperation.h
			$(GCC) -c ../src/InsertOperation.cpp

DataBuffer.o:	   ../src/DataBuffer.cpp ../src/DataBuffer.h
			$(GCC) -c ../src/DataBuffer.cpp

MLZ03Codec.o:      ../src/MLZ03Codec.cpp ../src/MLZ03Codec.h ../src/LambdaIO.h
			$(GCC) -c ../src/MLZ03Codec.cpp

PatchFile.o:	   ../src/PatchFile.cpp ../lib/sha1/sha1.h
			$(GCC) -c ../src/PatchFile.cpp
		
#----------------------------------------------------------------------- 
# SHA1 Target

SHA1 = 		sha1.o

SHA1:		sha1.o

sha1.o:		../lib/sha1/sha1.cpp ../lib/sha1/sha1.h
			$(GCC) -c ../lib/sha1/sha1.cpp

#----------------------------------------------------------------------- 
# Creating a static lib using ar

LIB:		SHA1 LAMBDACORE		
		ar rs liblambda.a $(SHA) $(LAMBDACORE)

#----------------------------------------------------------------------- 
#Installing the lib
install:	all 
		mkdir -p $(INCLUDE_PATH); \
		mkdir -p $(LIB_PATH); \
		cp liblambda.a $(LIB_PATH) && \
		cp $(HEADER) $(INCLUDE_PATH)
		-@ echo ""
		-@ echo ""
		-@ echo "------------------------------"
		-@ echo ""
		-@ echo "Lambda has been installed to:"
		-@ echo "include files: $(INCLUDE_PATH)"
		-@ echo "library files: $(LIB_PATH)"
		-@ echo ""
		-@ echo "------------------------------"

#----------------------------------------------------------------------- 
# Cleaning object-files

clean:
		rm *.o
		-@ echo "cleaned up "
		-@ echo ""

#----------------------------------------------------------------------- 
#EOF
