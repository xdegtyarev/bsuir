//
//  XFS.h
//  Lab2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __Lab2__XFS__
#define __Lab2__XFS__
#include "IVFS.h"
#include <iostream>
class XFS: public IVFS{
public:
	static XFS& GetInstance();
	virtual IDirectory* GetRoot();
	virtual const char* GetDriverName();
	virtual const char* GetFileNameEncodingInfo();
	virtual void Mount(IMountablePoint *mountablePoint);
	virtual void Unmount(IMountablePoint *mountablePoint);
	virtual std::vector<IMountablePoint*> *GetMountedPoints();
private:
	static XFS* instance;
	IDirectory* root;
	IDirectory* mountRoot;
	std::vector<IMountablePoint*> mounted;
};
#endif /* defined(__Lab2__XFS__) */
