//
//  IVFS.h
//  Lab2
//
//  Created by Alexander Degtyarev on 11/21/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __Lab2__IVFS__
#define __Lab2__IVFS__

#include "IDirectory.h"
#include "IFile.h"
#import "IMountablePoint.h"

class IVFS{
    public:
    virtual const char* GetDriverName()=0;
    virtual const char* GetFileNameEncodingInfo()=0;
    virtual IDirectory* GetRoot()=0;
	virtual void Mount(IMountablePoint* mountablePoint)=0;
	virtual void Unmount(IMountablePoint* mountablePoint)=0;
	virtual std::vector<IMountablePoint*>* GetMountedPoints()=0;
};
#endif /* defined(__Lab2__IVFS__) */
