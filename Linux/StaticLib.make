
#
#----------------------------------------------------------------------- 
#
# This is the lambda makefile. It builds lambda as a
# static library. See the documentation for more information.
#
# Type 'make -f StaticLib.make' to build the lib.
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
all:		BUILDSTATICLIB

#----------------------------------------------------------------------- 
#all header-files

		
HEADER = 	../src/Common.h \
		../src/DataBuffer.h \
		../src/LambdaCodec.h \
		../src/MLZ03Codec.h \
		../src/PatchFile.h

OUTPUT = 	build/liblambda.a

#----------------------------------------------------------------------- 
# LAMBDALIB Targets

LAMBDALIB = 	CopyOperation.o \
		InsertOperation.o \
		DataBuffer.o \
		MLZ03Codec.o \
		PatchFile.o

LAMBDALIB:	CopyOperation.o InsertOperation.o DataBuffer.o MLZ03Codec.o PatchFile.o

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

BUILDSTATICLIB:	SHA1 LAMBDALIB		
		ar rcs $(OUTPUT) $(SHA1) $(LAMBDALIB)

#----------------------------------------------------------------------- 
# Cleaning object-files

clean:
		rm $(LAMBDALIB)
		rm $(OUTPUT)
		-@ echo "cleaned up "
		-@ echo ""

#----------------------------------------------------------------------- 
#EOF
