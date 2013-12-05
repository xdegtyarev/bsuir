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


IDirectory *NTFSFactory::CreateDir(char *name) {
	return new NTFSDirectory(name,NTFS::GetInstance().GetRoot());
}

IFile *NTFSFactory::CreateFile(char *name) {
	return new NTFSFile(name, (char *) ".bin",NTFS::GetInstance().GetRoot());
}

