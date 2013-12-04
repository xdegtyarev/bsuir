// Created by Alexander Degtyarev on 11/26/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.





#include "UFS.h"

#ifndef __UFSDirectory_H_
#define __UFSDirectory_H_

class UFSDirectory: public IMountablePoint {

public:
	UFSDirectory();
	UFSDirectory(const UFSDirectory& copy);
	UFSDirectory(char *_name, IDirectory *parent);
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

private:
	bool isMounted;
	char *name;
	IDirectory *parentDirectory;
	std::vector<IVFSObject*> contents;
	UFSDirectory(char *string, IDirectory *pDirectory, std::vector<IVFSObject*> vector);
	time_t modificationDate;
	time_t creationDate;

	void RemoveChild(IVFSObject *child);

public:
	virtual void ListContentToConsole();
};



#endif //__UFSDirectory_H_
