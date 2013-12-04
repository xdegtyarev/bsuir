// Created by Alexander Degtyarev on 12/2/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#include "IVFSFactory.h"

#ifndef __UFSFactory_H_
#define __UFSFactory_H_


class UFSFactory: public IVFSFactory {

public:
	virtual IVFS *CreateVFS();

	virtual IDirectory *CreateDir();

	virtual IFile *CreateFile();
};




#endif //__UFSFactory_H_
