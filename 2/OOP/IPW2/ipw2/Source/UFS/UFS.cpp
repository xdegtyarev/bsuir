//
//  UFS.cpp
//  Lab2
//
//  Created by Alexander Degtyarev on 11/25/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#include "UFS.h"
const char* UFS::GetDriverName(){
    return "UNIX file system";
}

const char* UFS::GetFileNameEncodingInfo(){
    return "Unicode";
}

IDirectory * UFS::GetRoot(){
    return root;
}

UFS& UFS::GetInstance() {
	static UFS instance;
	return instance;
}

void UFS::Mount(IMountablePoint *mountablePoint) {
	mountRoot->AddChild(mountablePoint);
	mounted.push_back(mountablePoint);
}

void UFS::Unmount(IMountablePoint *mountablePoint) {
	mounted.erase(std::find(mounted.begin(),mounted.end(),mountablePoint));
	mountRoot->RemoveChild(mountablePoint);
}

std::vector<IMountablePoint*> *UFS::GetMountedPoints() {
	return &mounted;
}

UFS::UFS() {
	root = new UFSDirectory("~", nullptr);
}