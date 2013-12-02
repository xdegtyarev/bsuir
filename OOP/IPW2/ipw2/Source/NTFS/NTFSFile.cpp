// Created by Alexander Degtyarev on 12/2/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "NTFSFile.h"
#include "NTFS.h"

NTFSFile::NTFSFile():NTFSFile((char*)"NewFile",(char*)"txt",NTFS::GetInstance().GetRoot()){}

NTFSFile::NTFSFile(const NTFSFile& copy){
	data = copy.data;
	extension = copy.extension;
}

NTFSFile::NTFSFile(char *_name, char *_extension, IDirectory* _parent){
	name = _name;
	parentDirectory = _parent;
	creationDate = time(0);
	modificationDate = time(0);
	extension = _extension;
}

char *NTFSFile::GetData() {
	return data;
}

void NTFSFile::SetData(char *_data) {
	modificationDate = time(0);
	data = _data;
}

char *NTFSFile::GetExtension() {
	return extension;
}

long long NTFSFile::GetSize() {
	return sizeof(*data);
}

char *NTFSFile::GetObjectName() {
	return name;
}

IVFSObject *NTFSFile::GetParent() {
	return parentDirectory;
}

void NTFSFile::SetParent(IVFSObject *object) {
	modificationDate = time(0);
	parentDirectory = (IDirectory *) object;
}

time_t NTFSFile::GetCreationDate() {
	return creationDate;
}

time_t NTFSFile::GetModificationDate() {
	return modificationDate;
}