// Created by Alexander Degtyarev on 11/26/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "UFSFile.h"
UFSFile::UFSFile():UFSFile("NewFile","txt",UFS::GetInstance().GetRoot()){}

UFSFile::UFSFile(const UFSFile& copy){
	data = copy.data;
	extension = copy.extension;
}

UFSFile::UFSFile(std::string _name, std::string _extension, IDirectory* _parent){
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

std::string UFSFile::GetExtension() {
	return extension;
}

long long UFSFile::GetSize() {
	return sizeof(*data);
}

std::string UFSFile::GetObjectName() {
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
