// Created by Alexander Degtyarev on 12/1/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#include "IDirectory.h"
#include "IMountablePoint.h"
#include <string>
#ifndef __NTFSDirectory_H_
#define __NTFSDirectory_H_
class NTFSDirectory: public IMountablePoint {
public:
	NTFSDirectory();
	NTFSDirectory(const NTFSDirectory& copy);
	NTFSDirectory(std::string _name, IDirectory *parent);
	//IObj
	virtual std::string GetObjectName();
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
	std::string name;
	IDirectory *parentDirectory;
	std::vector<IVFSObject*> contents;
	NTFSDirectory(std::string string, IDirectory *pDirectory, std::vector<IVFSObject*> vector);
	time_t modificationDate;
	time_t creationDate;

	void RemoveChild(IVFSObject *child);

public:
	virtual void ListContentToConsole();
};




#endif //__NTFSDirectory_H_
