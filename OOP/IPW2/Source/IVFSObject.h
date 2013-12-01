//
//  IVFSObject.h
//  Lab2
//
//  Created by Alexander Degtyarev on 11/21/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __Lab2__IVFSObject__
#define __Lab2__IVFSObject__

#include <ctime>
#include "IVFSObject.h"

class IVFSObject{
public:
    virtual char* GetObjectName()=0;
    virtual IVFSObject* GetParent()=0;
	virtual void SetParent(IVFSObject*)=0;
    virtual time_t GetCreationDate()=0;
    virtual time_t GetModificationDate()=0;
    virtual long long GetSize()=0;
};
#endif /* defined(__Lab2__IVFSObject__) */
