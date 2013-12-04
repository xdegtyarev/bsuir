//
//  XFSDirectory.cpp
//  Lab2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#include "XFSDirectory.h"
#include "XFS.h"
XFSDirectory::XFSDirectory():XFSDirectory((char *) "New Folder",XFS::GetInstance().GetRoot()){
    
}

XFSDirectory::XFSDirectory(char *_name, IDirectory *pDirectory, std::vector<IVFSObject*> vector):XFSDirectory(_name,pDirectory){
	contents = vector;
}


XFSDirectory::XFSDirectory(const XFSDirectory &copy) {
	XFSDirectory(copy.name,copy.parentDirectory,copy.contents);
}

XFSDirectory::XFSDirectory(char *_name, IDirectory* _parent){
	name = _name;
	parentDirectory = _parent;
	creationDate = time(0);
	modificationDate = time(0);
    
}

long long XFSDirectory::GetSize() {
	long long size = 0;
	for(std::vector<IVFSObject*>::iterator it = contents.begin(); it!=contents.end();++it){
		size += (*it)->GetSize();
	}
	return size;
}

std::vector<IVFSObject*> XFSDirectory::GetDirectoryContent() {
	return contents;
}

void XFSDirectory::AddChild(IVFSObject* child) {
	child->SetParent(this);
	contents.push_back(child);
	modificationDate = time(0);
}

void XFSDirectory::RemoveChild(IVFSObject* child){
	child->SetParent(nullptr);
	contents.erase(std::find(contents.begin(),contents.end(),child));
	modificationDate = time(0);
}

char *XFSDirectory::GetObjectName() {
	return name;
}

IVFSObject *XFSDirectory::GetParent() {
	return parentDirectory;
}

void XFSDirectory::SetParent(IVFSObject *object) {
	modificationDate = time(0);
	parentDirectory = (IDirectory *) object;
}

time_t XFSDirectory::GetCreationDate() {
	return creationDate;
}

time_t XFSDirectory::GetModificationDate() {
	return modificationDate;
}

bool XFSDirectory::IsMounted() {
	return isMounted;
}


void XFSDirectory::ListContentToConsole() {
	std::cout << "Contents of: " << GetObjectName() << std::endl;
	for(std::vector<IVFSObject*>::iterator i = contents.begin();i<contents.end();++i){
		std::cout << (*i)->GetObjectName() << std::endl;
	}
}
