
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
INC = -I/usr/include/lambda -I../lib -I../src
LIB = -L/usr/local/lib

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

#----------------------------------------------------------------------- 
#Main-Target
all:		APP LINK

#----------------------------------------------------------------------- 
# APP Targets

APP = 	main.o

APP:	main.o

main.o:   ../sample/main.cpp
		$(GCC) -c ../sample/main.cpp

#----------------------------------------------------------------------- 
# Link the obj and static lib to create the sample app

LINK:	lambda		
		
lambda:	main.o  liblambda.a
		gcc -static -o lambda $(LIB) -lc -llambda main.o

#-Wl,-Bdynamic
#----------------------------------------------------------------------- 
# Cleaning object-files

clean:
		rm *.o
		-@ echo "cleaned up "
		-@ echo ""

#----------------------------------------------------------------------- 
#EOF
