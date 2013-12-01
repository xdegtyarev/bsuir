//
//  XFSFile.cpp
//  Lab2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#include "XFSFile.h"
#include "XFS.h"
XFSFile::XFSFile():XFSFile((char*)"NewFile",(char*)"txt",XFS::GetInstance().GetRoot()){}

XFSFile::XFSFile(const XFSFile& copy){
	data = copy.data;
	extension = copy.extension;
}

XFSFile::XFSFile(char *_name, char *_extension, IDirectory* _parent){
	name = _name;
	parentDirectory = _parent;
	creationDate = time(0);
	modificationDate = time(0);
	extension = _extension;
}

char *XFSFile::GetData() {
	return data;
}

void XFSFile::SetData(char *_data) {
	modificationDate = time(0);
	data = _data;
}

char *XFSFile::GetExtension() {
	return extension;
}

long long XFSFile::GetSize() {
	return sizeof(*data);
}

char *XFSFile::GetObjectName() {
	return name;
}

IVFSObject *XFSFile::GetParent() {
	return parentDirectory;
}

void XFSFile::SetParent(IVFSObject *object) {
	modificationDate = time(0);
	parentDirectory = (IDirectory *) object;
}

time_t XFSFile::GetCreationDate() {
	return creationDate;
}

time_t XFSFile::GetModificationDate() {
	return modificationDate;
}