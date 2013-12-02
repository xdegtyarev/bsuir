// Created by Alexander Degtyarev on 12/2/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#include "IFile.h"
#include "IDirectory.h"

#ifndef __NTFSFile_H_
#define __NTFSFile_H_


class NTFSFile: public IFile {
public:
	NTFSFile();

	NTFSFile(char* _name,char* _extension,IDirectory* directory);
	NTFSFile(NTFSFile const &copy);
	//IFile
	virtual char* GetExtension();
	virtual void SetData(char *data);
	virtual char* GetData();
	//IObject
	virtual long long GetSize();
protected:
	char* data;
	char* extension;
	char *name;
	IDirectory *parentDirectory;
	time_t modificationDate;
	time_t creationDate;

	//IObject
public:
	virtual char *GetObjectName();
	virtual IVFSObject *GetParent();
	virtual void SetParent(IVFSObject *object);
	virtual time_t GetCreationDate();
	virtual time_t GetModificationDate();
};


#endif //__NTFSFile_H_
