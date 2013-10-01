//
//  main.cpp
//  Lambda
//
//  Created by Phil McCarthy on 01/10/2013.
//  Copyright (c) 2013 Phil McCarthy. All rights reserved.
//

#include <iostream>
#include "MLZ01Encoder.h"
#include "PatchFile.h"

int main(int argc, const char * argv[])
{
    if (argc < 4)
	{
        std::cout << "Lambda [filename] [modifiedfilename] [differencefile]" << std::endl;
        return 1;
	}
    
	// set up a patch file object
    lambda::CMLZ01Encoder bufferEncoder;
	lambda::CPatchFile patchFile(argv[3], &bufferEncoder);
    
	// create the patch file
	patchFile.Create(argv[1], argv[2]);
    
    return 0;
}

