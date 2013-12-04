// Created by Alexander Degtyarev on 12/2/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "XFSFactory.h"
#include "XFS.h"
#include "XFSDirectory.h"
#include "XFSFile.h"

IVFS *XFSFactory::CreateVFS() {
	return &XFS::GetInstance();
}


IDirectory *XFSFactory::CreateDir() {
	return new XFSDirectory();
}

IFile *XFSFactory::CreateFile() {
	return new XFSFile();
}

#include "XFS.h"
