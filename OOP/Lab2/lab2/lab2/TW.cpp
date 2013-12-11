// Created by Alexander Degtyarev on 12/11/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include <string>
#include "TW.h"
#include "TWUser.h"

IPerson *TW::CreateUser(std::string name) {
	TWUser* user = new TWUser(name);
	userbase[name] = user;
	return user;
}

void TW::RemoveUser(IPerson *person) {
	userbase.erase(userbase.find(person->getName()));
}

IPerson *TW::TryGetUser(std::string name) {
	return userbase[name];
}
