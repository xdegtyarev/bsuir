// Created by Alexander Degtyarev on 12/10/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include <iostream>
#include "FB.h"
#include "FBUser.h"

IPerson *FB::CreateUser(std::string name) {
	currentGUID++;
	FBUser* user = new FBUser(name,currentGUID);
	users.push_back(user);
	return user;
}

void FB::RemoveUser(IPerson *person) {
	users.erase(std::find(users.begin(),users.end(),person));
}

IPerson *FB::TryGetUser(std::string name) {
	IPerson* person = nullptr;
	for(auto i = users.begin();i<users.end();++i){
		if((*i)->getName() == name){
			person = (*i);
		}
	}
	return person;
}
