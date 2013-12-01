//
//  IFile.h
//  Lab2
//
//  Created by Alexander Degtyarev on 11/21/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __Lab2__IFile__
#define __Lab2__IFile__
#include "IVFSObject.h"
class IFile: public IVFSObject{
public:
    virtual char* GetExtension()=0;
	virtual char* GetData()=0;
	virtual void SetData(char* data)=0;
};
#endif /* defined(__Lab2__IFile__) */
