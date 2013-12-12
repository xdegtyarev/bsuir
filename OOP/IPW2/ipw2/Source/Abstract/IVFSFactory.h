//
//  IVFSFactory.h
//  ipw2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#include <string>
#include "IVFS.h"

#ifndef ipw2_IVFSFactory_h
#define ipw2_IVFSFactory_h
class IVFSFactory{
public:
	virtual IVFS* CreateVFS() = 0;
	virtual IDirectory* CreateDir(std::string name="NewDir")=0;
	virtual IFile* CreateFile(std::string name = "NewFile")=0;
};


#endif
