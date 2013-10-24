
#
#----------------------------------------------------------------------- 
#
# This is a makefile to build the unit test harness for the lambda library.
# See the documentation for more information.
#
# Type 'make -f LambdaTest.make' to build the lib.
# Type 'make -f LambdaTest.make clean' to clean the object files
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
INC = -I../lib -I../src -I../lib/catch -I../test
LIB = -L. -Lbuild

#----------------------------------------------------------------------- 
# DON'T CHANGE ANYTHING BELOW
#----------------------------------------------------------------------- 

ifdef DEBUG
	COPTIONS += -g
else
	COPTIONS += -O3 -fomit-frame-pointer
endif

GCC = $(COMPILER) $(COPTIONS) $(INC) $(LIB)

HEADER = 	../src/Common.h \
		../src/DataBuffer.h \
		../src/LambdaCodec.h \
		../src/MLZ03Codec.h \
		../src/PatchFile.h

OUTPUT = 	build/lambdatests

LIBRARIES = 	lambda

#----------------------------------------------------------------------- 
#Main-Target
all:		BUILD

#----------------------------------------------------------------------- 
# APP Targets

APP = 	LambdaTests.o

APP:	LambdaTests.o

LambdaTests.o:   ../test/LambdaTests.cpp
		$(GCC) -c ../test/LambdaTests.cpp

#----------------------------------------------------------------------- 
# Link the obj and static lib to create the sample app

BUILD:	APP		
	$(GCC) LambdaTests.o -l $(LIBRARIES) -o $(OUTPUT)

#-Wl,-Bdynamic
#----------------------------------------------------------------------- 
# Cleaning object-files

clean:
		rm $(APP)
		rm $(OUTPUT)
		-@ echo "cleaned up "
		-@ echo ""

#----------------------------------------------------------------------- 
#EOF
