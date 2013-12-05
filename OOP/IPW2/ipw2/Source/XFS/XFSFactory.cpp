// Created by Alexander Degtyarev on 12/2/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "XFSFactory.h"
#include "XFS.h"
#include "XFSDirectory.h"
#include "XFSFile.h"

IVFS *XFSFactory::CreateVFS() {
	return &XFS::GetInstance();
}


IDirectory *XFSFactory::CreateDir(char *name) {
	return new XFSDirectory(name,XFS::GetInstance().GetRoot());
}

IFile *XFSFactory::CreateFile(char *name) {
	return new XFSFile(name,".xf",XFS::GetInstance().GetRoot());
}

#include "XFS.h"
