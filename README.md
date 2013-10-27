![Lambda logo](lambda.png)
# Lambda v0.2 - Difference coding compression library for C++

## Overview
This program / library provides C++ programs the ability to space efficiently encode the differences between two
files (or buffers). The compressed form (the "lambda" data) can then be used to patch the standalone
original file (or buffer) to the updated version.

## Why?
It could be used as the core of a remote backup system or as part of a version control system (such as git), if performace improvements can be made in the lambda coding engine.

As-is, it's an interesting proof of concept that demonstrates a space efficient difference encoding scheme, and is a demonstration of cross platform C++ code.

## Getting started
3 types of C++ build project have been included for convenience and portability:

- Visual Studio 2010 projects for building on Microsoft Windows (get going [here](#WinBuild))
- XCode 5 projects for building on Mac OSX (get going [here](#MacBuild))
- A Makefile for building on Linux (get going [here](#LinuxBuild))
 
 In each build folder 3 build options are available:
 
 - Build the core lambda code as a static library
 - Build a sample application dependent on the static lib, that lambda encodes/decodes files
 - Build a unit test application dependent on the static lib that tests the core lambda code

## <a id="WinBuild">Building on Microsoft Windows</a>
In the VS2010 directory, open the solution file VS2010.sln with Visual Studio 2010
or higher. Other versions of Visual Studio will most likely not work (but you can
probably use the source and create your own projects).

- Choose debug or release
- Do a "Build all" to build the static library, sample app and unit test harness
in the appropriate folder (either VS2010/Debug or VS2010/Release)

## <a id="MacBuild">Building on Apple Mac OSX</a>
In the XCode5 directory there are 3 project files:

- StaticLib: the lambda code set to build as a static library
- Lambda (includes/depends on StaticLib) The Lambda command-line sample program.
- LambdaTest: (includes/depends on StaticLib) A Catch based unit test harness app which exercises the encoder's
functionality and tests various edge cases

Double click on a project to bring it up in the XCode editor. Click Product->Build and Product->Run to build the source and launch the app.

## <a id="LinuxBuild">Building on Linux and Unix-like OSes</a>
In the Linux directory is a Makefile configured to use g++. From the linux shell you can run
the following commands:

- "make" (to build everything to a "build" subdirectory, and run the unit tests)
- "make staticlib" (to build only the liblambda.a static lib)
- "make lambda" (to build only the static lib and sample "lambda" app)
- "make tests" (to build only the static lib and "lambdatests" unit test harness app)

## Performance
Difference encoding to produce maximally space efficient patch files can be fairly slow. In release mode it 
takes ~10s to encode a 250kb word document. I've done a bit of performance testing, I think the main bottleneck
in the code is the number of linear buffer traversals required for matching less than 10 byte blocks (ie keying through
the dictionary buffer until a larger matching section is found).

Not sure what the solution is to this, there is a tradeoff between matching as many patterns in the original
file as possible (and therefore gaining optimal space efficiency in the lambda encoding) versus matching less but decreasing the encoding time.

Decoding is very fast since it is just following a set of copy/insert instructions to generate an output file.

## To do:
- Add multiple lambda buffer support in CPatchFile for arbitrarily huge input file handling (currently it just reads an entire file into memory)
- Add patch file unit tests
- Add more code commenting
- Update this documentation and add api doc
- Compress generated patch files (added small compression lib but not plumbed in yet)

Phil McCarthy, October 2013
