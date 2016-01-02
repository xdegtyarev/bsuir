//
//  XFSFile.h
//  Lab2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __Lab2__XFSFile__
#define __Lab2__XFSFile__
#include "IFile.h"
#include "IDirectory.h"
#include <iostream>
class XFSFile: public IFile{
public:
	XFSFile();
    
	XFSFile(std::string  _name,std::string  _extension,IDirectory* directory);
	XFSFile(XFSFile const &copy);
	//IFile
	virtual std::string  GetExtension();
	virtual void SetData(char *data);
	virtual char* GetData();
	//IObject
	virtual long long GetSize();
protected:
	char* data;
	std::string extension;
	std::string name;
	IDirectory *parentDirectory;
	time_t modificationDate;
	time_t creationDate;
    
	//IObject
public:
	virtual std::string GetObjectName();
	virtual IVFSObject *GetParent();
	virtual void SetParent(IVFSObject *object);
	virtual time_t GetCreationDate();
	virtual time_t GetModificationDate();
   
};
#endif /* defined(__Lab2__XFSFile__) */
