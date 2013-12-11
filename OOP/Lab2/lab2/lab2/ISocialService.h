// Created by Alexander Degtyarev on 12/9/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.

#include "IPerson.h"

#ifndef __ISocialService_H_
#define __ISocialService_H_

class ISocialService {
public:
	virtual IPerson* CreateUser(std::string name)=0;
	virtual void RemoveUser(IPerson* person)=0;
	virtual IPerson* TryGetUser(std::string name)=0;
};
#endif //__ISocialService_H_
