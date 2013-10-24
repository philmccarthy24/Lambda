
#
#----------------------------------------------------------------------- 
#
# This is the lambda (sample) application makefile.
# See the documentation for more information.
#
# Type 'make -f Lambda.make' to build the lib.
# Type 'make -f Lambda.make clean' to clean the object files
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
INC = -I../lib -I../src
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

OUTPUT =	build/lambda

LIBRARIES = 	lambda

#----------------------------------------------------------------------- 
#Main-Target
all:		BUILD

#----------------------------------------------------------------------- 
# APP Targets

APP = 	main.o

APP:	main.o

main.o:   ../sample/main.cpp
		$(GCC) -c ../sample/main.cpp

#----------------------------------------------------------------------- 
# Link the obj and liblambda.a static library to create the sample app

BUILD:	APP		
	$(GCC) main.o -l $(LIBRARIES) -o $(OUTPUT)

#----------------------------------------------------------------------- 
# Cleaning object-files

clean:
		rm $(APP)
		rm $(OUTPUT)
		-@ echo "cleaned up "
		-@ echo ""

#----------------------------------------------------------------------- 
#EOF
