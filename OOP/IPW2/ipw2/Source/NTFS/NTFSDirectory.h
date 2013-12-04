// Created by Alexander Degtyarev on 12/1/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#include "IDirectory.h"
#include "IMountablePoint.h"

#ifndef __NTFSDirectory_H_
#define __NTFSDirectory_H_
class NTFSDirectory: public IMountablePoint {
public:
	NTFSDirectory();
	NTFSDirectory(const NTFSDirectory& copy);
	NTFSDirectory(char *_name, IDirectory *parent);
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
	NTFSDirectory(char *string, IDirectory *pDirectory, std::vector<IVFSObject*> vector);
	time_t modificationDate;
	time_t creationDate;

	void RemoveChild(IVFSObject *child);

public:
	virtual void ListContentToConsole();
};




#endif //__NTFSDirectory_H_
