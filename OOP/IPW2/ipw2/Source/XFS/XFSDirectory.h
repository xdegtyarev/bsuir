//
//  XFSDirectory.h
//  Lab2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __Lab2__XFSDirectory__
#define __Lab2__XFSDirectory__
#include "IMountablePoint.h"
#include <iostream>
class XFSDirectory: public IMountablePoint{
public:
	XFSDirectory();
	XFSDirectory(const XFSDirectory& copy);
	XFSDirectory(char *_name, IDirectory *parent);
	//IObj
	virtual char *GetObjectName();
	virtual IVFSObject *GetParent();
	virtual void SetParent(IVFSObject *object);
	virtual time_t GetCreationDate();
	virtual time_t GetModificationDate();
	//IDir
	virtual std::vector<IVFSObject*> GetDirectoryContent();
	virtual void AddChild(IVFSObject *child);
	virtual long long GetSize();
private:
	virtual bool IsMounted();
    
	bool isMounted;
	char *name;
	IDirectory *parentDirectory;
	std::vector<IVFSObject*> contents;
	XFSDirectory(char *string, IDirectory *pDirectory, std::vector<IVFSObject*> vector);
	time_t modificationDate;
	time_t creationDate;
   
	void RemoveChild(IVFSObject *child);

public:
	virtual void ListContentToConsole();
};
#endif /* defined(__Lab2__XFSDirectory__) */
