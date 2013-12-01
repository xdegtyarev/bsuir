// Created by Alexander Degtyarev on 11/26/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "UFSDirectory.h"

UFSDirectory::UFSDirectory():UFSDirectory((char *) "New Folder",UFS::GetInstance().GetRoot()){

}

UFSDirectory::UFSDirectory(char *_name, IDirectory *pDirectory, std::vector<IVFSObject*> vector):UFSDirectory(_name,pDirectory){
	contents = vector;
}


UFSDirectory::UFSDirectory(const UFSDirectory &copy) {
	UFSDirectory(copy.name,copy.parentDirectory,copy.contents);
}

UFSDirectory::UFSDirectory(char *_name, IDirectory* _parent){
	name = _name;
	parentDirectory = _parent;
	creationDate = time(0);
	modificationDate = time(0);

}

long long UFSDirectory::GetSize() {
	long long size = 0;
	for(std::vector<IVFSObject*>::iterator it = contents.begin(); it!=contents.end();++it){
		size += (*it)->GetSize();
	}
	return size;
}

std::vector<IVFSObject*> UFSDirectory::GetDirectoryContent() {
	return contents;
}

void UFSDirectory::AddChild(IVFSObject* child) {
	child->SetParent(this);
	contents.push_back(child);
	modificationDate = time(0);
}

void UFSDirectory::RemoveChild(IVFSObject* child){
	child->SetParent(nullptr);
	contents.erase(std::find(contents.begin(),contents.end(),child));
	modificationDate = time(0);
}

char *UFSDirectory::GetObjectName() {
	return name;
}

IVFSObject *UFSDirectory::GetParent() {
	return parentDirectory;
}

void UFSDirectory::SetParent(IVFSObject *object) {
	modificationDate = time(0);
	parentDirectory = (IDirectory *) object;
}

time_t UFSDirectory::GetCreationDate() {
	return creationDate;
}

time_t UFSDirectory::GetModificationDate() {
	return modificationDate;
}

bool UFSDirectory::IsMounted() {
	return isMounted;
}
