//
//  NTFSFactory.cpp
//  ipw2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#include "NTFSFactory.h"
#include "IVFS.h"
#include "NTFS.h"
#include "NTFSDirectory.h"
#include "NTFSFile.h"

IVFS *NTFSFactory::CreateVFS() {
	return &NTFS::GetInstance();
}


IDirectory* NTFSFactory::CreateDir() {
	return new NTFSDirectory();
}

IFile* NTFSFactory::CreateFile() {
	return new NTFSFile();
}
