// Created by Alexander Degtyarev on 12/10/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#import "ISocialService.h"

#ifndef __FBUser_H_
#define __FBUser_H_


class FBUser: public IPerson {
public:
	FBUser(std::string name,int id);
	virtual std::vector<IPerson *> getConnections();
	virtual std::string getName();
	virtual void AddConnection(IPerson *connection);
	virtual void RemoveConnection(IPerson *connection);
	virtual void AddContent(std::string content);
	virtual void DisplayFeed();
private:
	std::string name;
	int id;
	std::vector<IPerson*> friends;
	std::vector<std::string> feed;
};


#endif //__FBUser_H_
