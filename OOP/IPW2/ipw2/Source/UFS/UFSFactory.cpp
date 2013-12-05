// Created by Alexander Degtyarev on 12/2/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "UFSFactory.h"
#include "UFS.h"

IVFS *UFSFactory::CreateVFS() {
	return &UFS::GetInstance();
}

IDirectory *UFSFactory::CreateDir(char *name) {
	return new UFSDirectory(name,UFS::GetInstance().GetRoot());
}

IFile *UFSFactory::CreateFile(char *name) {
	return new UFSFile(name, (char *) ".sh",UFS::GetInstance().GetRoot());
}
