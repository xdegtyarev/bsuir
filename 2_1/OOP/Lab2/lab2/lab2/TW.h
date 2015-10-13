// Created by Alexander Degtyarev on 12/11/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#import <map>
#include "ISocialService.h"

#ifndef __TW_H_
#define __TW_H_


class TW: public ISocialService {
public:
	virtual IPerson *CreateUser(std::string name);
	virtual void RemoveUser(IPerson *person);
private:
	std::map<std::string,IPerson*> userbase;
public:
	virtual IPerson *TryGetUser(std::string name);
};


#endif //__TW_H_
