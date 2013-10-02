
#
#----------------------------------------------------------------------- 
#
# This is the lambda makefile. It builds and installs lambda as a
# static library. See the documentation for more information.
#
# Type 'make' to build the lib.
# Type 'make install' to build and install the lib.
# Type 'make clean' to clean the object files
#
# Note for FreeBSD users:
# use gmake from "/usr/ports/devel/gmake"
#
# Phil McCarthy
# Wednesday 02 Oct 2013
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
COPTIONS = -ansi -Wall -Wextra

#----------------------------------------------------------------------- 
# DON'T CHANGE ANYTHING BELOW
#----------------------------------------------------------------------- 

ifdef DEBUG
	COPTIONS += -g
else
	COPTIONS += -O3 -fomit-frame-pointer
endif

GCC = $(COMPILER) $(COPTIONS)

#----------------------------------------------------------------------- 
#Main-Target
all:		LAMBDAOPS CODEC PATCHFILE LIB

#----------------------------------------------------------------------- 
#all header-files

		
HEADER = 	../src/Common.h \
		../src/CopyOperation.h \
		../src/InsertOperation.h \
		../src/LambdaEncoder.h \
		../src/LambdaOperation.h \
		../src/MLZ01Encoder.h

#----------------------------------------------------------------------- 
# LAMBDAOPS Targets

LAMBDAOPS = 	copyop.o \
				insertop.o

LAMBDAOPS:		copyop.o insertop.o

copyop.o:	../src/CopyOperation.cpp ../src/CopyOperation.h
			$(GCC) -c ../src/CopyOperation.cpp
			
insertop.o: ../src/InsertOperation.cpp ../src/InsertOperation.h
			$(GCC) -c ../src/InsertOperation.cpp
		
#----------------------------------------------------------------------- 
# CODEC Target

CODEC = 	codec.o

CODEC:		codec.o

codec.o:	../src/MLZ01Encoder.cpp ../src/MLZ01Encoder.h
		$(GCC) -c ../src/MLZ01Encoder.cpp

#----------------------------------------------------------------------- 
# PATCHFILE Target

PATCHFILE = 	patchfile.o

PATCHFILE:		patchfile.o

patchfile.o:	../src/PatchFile.cpp ../src/PatchFile.h
		$(GCC) -c ../src/PatchFile.cpp

#----------------------------------------------------------------------- 
# Creating a static lib using ar

LIB:		LAMBDAOPS CODEC PATCHFILE			
		ar rs libhl++.a $(LAMBDAOPS) $(CODEC) $(PATCHFILE)

#----------------------------------------------------------------------- 
#Installing the lib
install:	all 
		mkdir -p $(INCLUDE_PATH); \
		mkdir -p $(LIB_PATH); \
		cp libhl++.a $(LIB_PATH) && \
		cp $(HEADER) $(INCLUDE_PATH)
		-@ echo ""
		-@ echo ""
		-@ echo "------------------------------"
		-@ echo ""
		-@ echo "hashlib++ has been installed to:"
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
