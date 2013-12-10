// Created by Alexander Degtyarev on 12/11/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include "TW.h"
#include "TWUser.h"

IPerson *TW::CreateUser(char *name) {
	TWUser* user = new TWUser(name);
	userbase[name] = user;
	return user;
}

void TW::RemoveUser(IPerson *person) {
	userbase.erase(userbase.find(person->getName()));
}

IPerson *TW::TryGetUser(char *name) {
	return userbase[name];
}
