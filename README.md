![Lambda logo](lambda.png)
# Lambda v0.1 - Difference coding compression library for C++

## Overview
This program / library provides C++ programs the ability to space efficiently encode the differences between two
files (or buffers). The compressed form (the "lambda" data) can then be used to patch the standalone
original file (or buffer) to the updated version.

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
To be added

## <a id="MacBuild">Building on Apple Mac OSX</a>
In the XCode5 directory there are 3 project files:

- StaticLib: the lambda code set to build as a static library
- Lambda (includes/depends on StaticLib) The Lambda command-line sample program.
- LambdaTest: (includes/depends on StaticLib) A Catch based unit test harness app which exercises the encoder's
functionaality and tests various edge cases

Double click on a project to bring it up in the XCode editor. Click Product->Build and Product->Run to build the source and lauuch the app.

## <a id="LinuxBuild">Building on Linux and Unix-like OSes</a>
To be added

## Performance
Difference encoding to produce maximally space efficient patch files can be fairly slow. In release mode it 
takes ~10s to encode a 250kb word document. I've done a bit of performance testing, I think the main bottleneck
in the code is the number of linear buffer traersals required for matching small <10 byte blocks (ie keying down
the dictionary buffer until a larger matching section is found).

Not sure what the solution is to this, there is a tradeoff between matching as many patterns in the original
file as we can (and therefore gaining optimal space efficiency in the lambda encoding) versus matching less
but speeding the encoding time.

Decoding is very fast since it is just following a set of copy/insert instructions to generate an output file.

## To do:
- Add multiple lambda buffer support in CPatchFile for arbitrarily huge input file handling
- compress generated patch files
- Get Linux makefile working
- Add patch file unit tests
- Add more commenting
- Update this documentation and add api doc

Phil McCarthy, October 2013
