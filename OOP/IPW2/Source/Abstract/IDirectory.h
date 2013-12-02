//
//  IDirectory.h
//  Lab2
//
//  Created by Alexander Degtyarev on 11/21/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __Lab2__IDirectory__
#define __Lab2__IDirectory__
#include "IVFSObject.h"
#include <vector>
class IDirectory: public IVFSObject{
public:
    virtual std::vector<IVFSObject*> GetDirectoryContent()=0;
    virtual void AddChild(IVFSObject* child)=0;
	virtual void RemoveChild(IVFSObject* child)=0;
};
#endif /* defined(__Lab2__IDirectory__) */
