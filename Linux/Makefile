
#
#----------------------------------------------------------------------- 
#
# This is the Lambda project makefile.
#
# Type 'make' to build everything.
# Type 'make staticlib' to build the static library on its own.
# Type 'make lambda' to build the sample app.
# Type 'make tests' to build the unit test harness app and run the tests.
# Type 'make clean' to clean object files
#
# Phil McCarthy
# 20 Oct 2013
#
#----------------------------------------------------------------------- 

# Compiler to use
COMPILER = g++

# Global options for the compiler
COPTIONS = -ansi -Wall -Wextra -std=c++11

# Output directory
OUTDIR = build

# include paths
INC = -I../src -I../lib -I../lib/sha1 -I../lib/catch -I../test

# library paths
LIB = -L$(OUTDIR)

#----------------------------------------------------------------------- 
# DON'T CHANGE ANYTHING BELOW
#----------------------------------------------------------------------- 

ifdef DEBUG
	COPTIONS += -g
else
	COPTIONS += -O3 -fomit-frame-pointer
endif

GCC = $(COMPILER) $(COPTIONS) $(INC) $(LIB)

#----------------------------------------------------------------------- 
#Main Targets
all:		prep staticlib lambda tests runtests clean

#----------------------------------------------------------------------- 
# prep Targets - clean object files and recreate output dir

prep:		
		@rm -f *.o
		@rm -Rf $(OUTDIR)
		@mkdir $(OUTDIR)

#----------------------------------------------------------------------- 
# staticlib Targets

staticlib_objects = 	CopyOperation.o \
			InsertOperation.o \
			DataBuffer.o \
			MLZ03Codec.o \
			PatchFile.o \
			sha1.o

staticlib:	CopyOperation.o InsertOperation.o DataBuffer.o MLZ03Codec.o PatchFile.o sha1.o makelib

CopyOperation.o:   ../src/CopyOperation.cpp ../src/CopyOperation.h
			@$(GCC) -c ../src/CopyOperation.cpp
			
InsertOperation.o: ../src/InsertOperation.cpp ../src/InsertOperation.h
			@$(GCC) -c ../src/InsertOperation.cpp

DataBuffer.o:	   ../src/DataBuffer.cpp ../src/DataBuffer.h
			@$(GCC) -c ../src/DataBuffer.cpp

MLZ03Codec.o:      ../src/MLZ03Codec.cpp ../src/MLZ03Codec.h ../src/LambdaIO.h
			@$(GCC) -c ../src/MLZ03Codec.cpp

PatchFile.o:	   ../src/PatchFile.cpp ../lib/sha1/sha1.h
			@$(GCC) -c ../src/PatchFile.cpp

sha1.o:		   ../lib/sha1/sha1.cpp ../lib/sha1/sha1.h
			@$(GCC) -c ../lib/sha1/sha1.cpp
		
makelib:	  
		    @ar rcs $(OUTDIR)/liblambda.a $(staticlib_objects)
		    -@ echo "Lambda static library created."

#----------------------------------------------------------------------- 
# Lambda sample app targets.
# Link the obj and liblambda.a static library to create the sample app

lambda:			staticlib main.o lambdalink

main.o:   	   ../sample/main.cpp
			@$(GCC) -c ../sample/main.cpp

lambdalink:	
		   @$(GCC) main.o -l lambda -o $(OUTDIR)/lambda
		   -@ echo "Lambda sample app compiled and linked."

#----------------------------------------------------------------------- 
# Lambda unit test harness targets

tests:			staticlib LambdaTests.o testslink
			

LambdaTests.o:     ../test/LambdaTests.cpp
		   	@$(GCC) -c ../test/LambdaTests.cpp

testslink:			
		   @$(GCC) LambdaTests.o -l lambda -o $(OUTDIR)/lambdatests
		   -@ echo "Lambda unit tests compiled and linked."

#----------------------------------------------------------------------- 
# Run unit tests targets

runtests:		tests
		-@ echo "Running unit tests..."
		@$(OUTDIR)/lambdatests

#----------------------------------------------------------------------- 
# Cleaning object-files

clean:
		@rm -f *.o
		-@ echo "Cleaned up."
		-@ echo ""

#----------------------------------------------------------------------- 
#EOF
