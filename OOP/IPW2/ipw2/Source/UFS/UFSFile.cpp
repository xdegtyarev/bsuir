// Created by Alexander Degtyarev on 11/26/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "UFSFile.h"
UFSFile::UFSFile():UFSFile((char*)"NewFile",(char*)"txt",UFS::GetInstance().GetRoot()){}

UFSFile::UFSFile(const UFSFile& copy){
	data = copy.data;
	extension = copy.extension;
}

UFSFile::UFSFile(char *_name, char *_extension, IDirectory* _parent){
	name = _name;
	parentDirectory = _parent;
	creationDate = time(0);
	modificationDate = time(0);
	extension = _extension;
}

char *UFSFile::GetData() {
	return data;
}

void UFSFile::SetData(char *_data) {
	modificationDate = time(0);
	data = _data;
}

char *UFSFile::GetExtension() {
	return extension;
}

long long UFSFile::GetSize() {
	return sizeof(*data);
}

char *UFSFile::GetObjectName() {
	return name;
}

IVFSObject *UFSFile::GetParent() {
	return parentDirectory;
}

void UFSFile::SetParent(IVFSObject *object) {
	modificationDate = time(0);
	parentDirectory = (IDirectory *) object;
}

time_t UFSFile::GetCreationDate() {
	return creationDate;
}

time_t UFSFile::GetModificationDate() {
	return modificationDate;
}
