// Created by Alexander Degtyarev on 12/1/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#ifndef __NTFS_H_
#define __NTFS_H_



#include <iostream>
#include "IVFS.h"
class NTFS: public IVFS{
public:
	static NTFS& GetInstance();
	virtual IDirectory* GetRoot();
	virtual const char* GetDriverName();
	virtual const char* GetFileNameEncodingInfo();
	virtual void Mount(IMountablePoint *mountablePoint);
	virtual void Unmount(IMountablePoint *mountablePoint);
	virtual std::vector<IMountablePoint*> *GetMountedPoints();
private:
	static NTFS* instance;
	IDirectory* root;
	IDirectory* mountRoot;
	std::vector<IMountablePoint*> mounted;
public:

};


#endif //__NTFS_H_
