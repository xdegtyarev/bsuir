// Created by Alexander Degtyarev on 12/11/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#include <vector>

#ifndef __IPerson_H_
#define __IPerson_H_


class IPerson{
public:
	virtual std::vector<IPerson*> getConnections()=0;
	virtual char* getName()=0;
	virtual void AddConnection(IPerson* connection)=0;
	virtual void RemoveConnection(IPerson* connection)=0;
	virtual void AddContent(char* content) = 0;
	virtual void DisplayFeed()=0;
};


#endif //__IPerson_H_
