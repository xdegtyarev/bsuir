//
//  main.cpp
//  Lab2
//
//  Created by Alexander Degtyarev on 11/21/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#include <iostream>
#include "UFS.h"

int main(int argc, const char * argv[])
{
    std::cout << "Hello, World!\n";
	std::cout << UFS::GetInstance().GetDriverName();
	std::cout << UFS::GetInstance().GetDriverName();
    return 0;
}

