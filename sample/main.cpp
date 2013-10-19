//
//  main.cpp
//  Lambda
//
//  Created by Phil McCarthy on 01/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#include <iostream>
#include "MLZ03Codec.h"
#include "PatchFile.h"
#include "Common.h"
#include "tclap/CmdLine.h"   // for the tclap library

using namespace lambda;

// Command line arguments can be:
// lambda -c -i infile -m modifiedfile -p patchfile
// lambda -u -i infile -p patchfile -m modifiedfile
int main(int argc, const char* argv[])
{
    try
    {
        TCLAP::CmdLine cmd("Lambda file difference encoder (patch generator) and file updater", ' ', "0.1");
        
        TCLAP::SwitchArg createPatchSwitch("c", "create", "Create patch file from infile and modifiedfile", true);
        TCLAP::SwitchArg updateSwitch("u", "update", "Update infile to modifiedfile from patch file", true);
        
        TCLAP::ValueArg<std::string> infileArg("i", "infile", "Input file", true, "", "string");
        TCLAP::ValueArg<std::string> modifiedfileArg("m", "modifiedfile", "Updated input file", true, "", "string");
        TCLAP::ValueArg<std::string> patchfileArg("p", "patchfile", "Difference patch file", true, "", "string");
        
        cmd.add(infileArg);
        cmd.add(modifiedfileArg);
        cmd.add(patchfileArg);
        cmd.xorAdd(createPatchSwitch, updateSwitch);
        
        // Parse the argv array.
        cmd.parse(argc, argv);
        
        // Get the value parsed by each arg.
        std::string strOriginalFile = infileArg.getValue();
        std::string strModifiedFile = modifiedfileArg.getValue();
        std::string strPatchFile = patchfileArg.getValue();
        
        // set up a patch file object
        CMLZ03Codec bufferEncoder;
        CPatchFile patchFile(strPatchFile, &bufferEncoder);
        
        // Check what mode we're in
        if (createPatchSwitch.isSet())
        {
            // create the patch file
            patchFile.Create(strOriginalFile, strModifiedFile);
        }
        else if (updateSwitch.isSet())
        {
            // update the input file
            patchFile.Patch(strOriginalFile, strModifiedFile);
        }
        
	}
    catch (const TCLAP::ArgException &e)  // catch any exceptions
	{
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    catch (const LambdaBaseException& le)
    {
        std::cerr << "error: " << le.what() << " at location " << le.GetLocation() << " (" << le.GetTimestamp() << ")" << std::endl;
    }
    
    return 0;
}

