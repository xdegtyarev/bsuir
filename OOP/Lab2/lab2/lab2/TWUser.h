// Created by Alexander Degtyarev on 12/11/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#include "IPerson.h"

#ifndef __TWUser_H_
#define __TWUser_H_


class TWUser: public IPerson{
public:
	virtual std::vector<IPerson *> getConnections();
	virtual char * getName();
	virtual void AddConnection(IPerson* connection);
	virtual void RemoveConnection(IPerson* connection);
	virtual void AddContent(char * content);
	virtual void DisplayFeed();
	TWUser(char *name);
private:
	char *name;
	std::vector<IPerson*> following;
	std::vector<char*> tweets;
};


#endif //__TWUser_H_
