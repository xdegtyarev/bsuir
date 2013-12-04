// Created by Alexander Degtyarev on 12/1/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "NTFSDirectory.h"
#include "NTFS.h"

NTFSDirectory::NTFSDirectory():NTFSDirectory((char *) "New Folder",NTFS::GetInstance().GetRoot()){

}

NTFSDirectory::NTFSDirectory(char *_name, IDirectory *pDirectory, std::vector<IVFSObject*> vector):NTFSDirectory(_name,pDirectory){
	contents = vector;
}


NTFSDirectory::NTFSDirectory(const NTFSDirectory &copy) {
	NTFSDirectory(copy.name,copy.parentDirectory,copy.contents);
}

NTFSDirectory::NTFSDirectory(char *_name, IDirectory* _parent){
	name = _name;
	parentDirectory = _parent;
	creationDate = time(0);
	modificationDate = time(0);

}

long long NTFSDirectory::GetSize() {
	long long size = 0;
	for(std::vector<IVFSObject*>::iterator it = contents.begin(); it!=contents.end();++it){
		size += (*it)->GetSize();
	}
	return size;
}

std::vector<IVFSObject*> NTFSDirectory::GetDirectoryContent() {
	return contents;
}

void NTFSDirectory::AddChild(IVFSObject* child) {
	child->SetParent(this);
	contents.push_back(child);
	modificationDate = time(0);
}

void NTFSDirectory::RemoveChild(IVFSObject* child){
	child->SetParent(nullptr);
	contents.erase(std::find(contents.begin(),contents.end(),child));
	modificationDate = time(0);
}

char *NTFSDirectory::GetObjectName() {
	return name;
}

IVFSObject *NTFSDirectory::GetParent() {
	return parentDirectory;
}

void NTFSDirectory::SetParent(IVFSObject *object) {
	modificationDate = time(0);
	parentDirectory = (IDirectory *) object;
}

time_t NTFSDirectory::GetCreationDate() {
	return creationDate;
}

time_t NTFSDirectory::GetModificationDate() {
	return modificationDate;
}

bool NTFSDirectory::IsMounted() {
	return isMounted;
}


void NTFSDirectory::ListContentToConsole() {
	std::cout << "Contents of: " << GetObjectName() << std::endl;
	for(std::vector<IVFSObject*>::iterator i = contents.begin();i<contents.end();++i){
		std::cout << (*i)->GetObjectName() << std::endl;
	}
}
