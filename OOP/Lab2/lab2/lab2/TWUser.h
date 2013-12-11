// Created by Alexander Degtyarev on 12/11/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#include "IPerson.h"

#ifndef __TWUser_H_
#define __TWUser_H_


class TWUser: public IPerson{
public:
	virtual std::vector<IPerson *> getConnections();
	virtual std::string getName();
	virtual void AddConnection(IPerson* connection);
	virtual void RemoveConnection(IPerson* connection);
	virtual void AddContent(std::string content);
	virtual void DisplayFeed();
	TWUser(std::string name);
private:
	std::string name;
	std::vector<IPerson*> following;
	std::vector<std::string> tweets;
};


#endif //__TWUser_H_
