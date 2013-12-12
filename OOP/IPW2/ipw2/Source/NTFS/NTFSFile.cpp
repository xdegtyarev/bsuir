// Created by Alexander Degtyarev on 12/2/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "NTFSFile.h"
#include "NTFS.h"

NTFSFile::NTFSFile():NTFSFile("NewFile","txt",NTFS::GetInstance().GetRoot()){}

NTFSFile::NTFSFile(const NTFSFile& copy){
	data = copy.data;
	extension = copy.extension;
}

NTFSFile::NTFSFile(std::string _name, std::string _extension, IDirectory* _parent){
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

std::string NTFSFile::GetExtension() {
	return extension;
}

long long NTFSFile::GetSize() {
	return sizeof(*data);
}

std::string NTFSFile::GetObjectName() {
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