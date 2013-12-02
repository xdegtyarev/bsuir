//
//  XFS.cpp
//  Lab2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#include "XFS.h"
#include "XFSDirectory.h"

const char* XFS::GetDriverName(){
	return "XFS file system";
}

const char* XFS::GetFileNameEncodingInfo(){
	return "Unicode";
}

IDirectory * XFS::GetRoot(){
	return root;
}

XFS& XFS::GetInstance() {
	static XFS instance;
	return instance;
}

void XFS::Mount(IMountablePoint *mountablePoint) {
	mountRoot->AddChild(mountablePoint);
	mounted.push_back(mountablePoint);
}

void XFS::Unmount(IMountablePoint *mountablePoint) {
	mounted.erase(std::find(mounted.begin(),mounted.end(),mountablePoint));
	mountRoot->RemoveChild(mountablePoint);
}

std::vector<IMountablePoint*> *XFS::GetMountedPoints() {
	return &mounted;
}

XFS::XFS() {
	root = new XFSDirectory((char *) "~", nullptr);
}