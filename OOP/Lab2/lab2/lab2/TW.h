// Created by Alexander Degtyarev on 12/11/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#import <map>
#include "ISocialService.h"

#ifndef __TW_H_
#define __TW_H_


class TW: public ISocialService {
public:
	virtual IPerson *CreateUser(char *name);
	virtual void RemoveUser(IPerson *person);
private:
	std::map<char*,IPerson*> userbase;
public:
	virtual IPerson *TryGetUser(char *name);
};


#endif //__TW_H_
