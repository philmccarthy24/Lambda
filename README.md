![Lambda logo](lambda.png)
# Lambda v0.91 - Difference coding compression library for C++

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
To be added

## <a id="LinuxBuild">Building on Linux and Unix-like OSes</a>
To be added

## To do:
- Add output cost optimisation to roll multiple inefficient copies into single insert ops
- Add multiple lambda buffer support in CPatchFile for arbitrarily huge input file handling
- Add patch file header data including sha1 hashes of original and modified file so we know
the provided dictionary file is correct, and we can validate the result of the decode
- Get Linux makefile working
- Update VS project
- Add more unit tests with edge cases
- Add lots more commenting
- Update this documentation and add api doc

Phil McCarthy, October 2013
