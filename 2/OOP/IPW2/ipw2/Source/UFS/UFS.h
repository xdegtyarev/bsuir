//
//  UFS.h
//  Lab2
//
//  Created by Alexander Degtyarev on 11/25/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __Lab2__UFS__
#define __Lab2__UFS__

#include <iostream>
#include "IVFS.h"
#include "UFSFile.h"
#include "UFSDirectory.h"
class UFS: public IVFS{
public:
	UFS();
	static UFS& GetInstance();
	virtual IDirectory* GetRoot();
	virtual const char* GetDriverName();
    virtual const char* GetFileNameEncodingInfo();
	virtual void Mount(IMountablePoint *mountablePoint);
	virtual void Unmount(IMountablePoint *mountablePoint);
	virtual std::vector<IMountablePoint*> *GetMountedPoints();
private:
	IDirectory* root;
	IDirectory* mountRoot;
	std::vector<IMountablePoint*> mounted;
public:

};
#endif /* defined(__Lab2__UFS__) */
