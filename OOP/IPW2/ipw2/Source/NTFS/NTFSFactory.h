//
//  NTFSFactory.h
//  ipw2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __ipw2__NTFSFactory__
#define __ipw2__NTFSFactory__
#include "IVFSFactory.h"
#include "IVFS.h"
#include <iostream>
class NTFSFactory: public IVFSFactory{
public:
	virtual IVFS* CreateVFS();
	virtual IDirectory *CreateDir(char *name);
	virtual IFile *CreateFile(char *name);
};
#endif /* defined(__ipw2__NTFSFactory__) */
