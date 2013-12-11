// Created by Alexander Degtyarev on 12/10/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#import "ISocialService.h"

#ifndef __FB_H_
#define __FB_H_


class FB: public ISocialService {
public:
	virtual IPerson *CreateUser(std::string name);
	virtual void RemoveUser(IPerson *person);
	virtual IPerson *TryGetUser(std::string name);
private:
	int currentGUID = 0;
	std::vector<IPerson*> users;
};

#endif //__FB_H_
