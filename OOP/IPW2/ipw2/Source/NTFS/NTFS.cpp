// Created by Alexander Degtyarev on 12/1/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "NTFS.h"
#include "NTFSDirectory.h"

const char* NTFS::GetDriverName(){
	return "NTFS file system";
}

const char* NTFS::GetFileNameEncodingInfo(){
	return "UTF-16";
}

IDirectory * NTFS::GetRoot(){
	return root;
}

NTFS& NTFS::GetInstance() {
	static NTFS instance;
	return instance;
}

void NTFS::Mount(IMountablePoint *mountablePoint) {
	mountRoot->AddChild(mountablePoint);
	mounted.push_back(mountablePoint);
}

void NTFS::Unmount(IMountablePoint *mountablePoint) {
	mounted.erase(std::find(mounted.begin(),mounted.end(),mountablePoint));
	mountRoot->RemoveChild(mountablePoint);
}

std::vector<IMountablePoint*> *NTFS::GetMountedPoints() {
	return &mounted;
}

NTFS::NTFS() {
	root = new NTFSDirectory((char *) "C:", nullptr);
}