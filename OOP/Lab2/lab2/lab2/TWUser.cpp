// Created by Alexander Degtyarev on 12/11/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include <iostream>
#include "TWUser.h"

std::vector<IPerson *> TWUser::getConnections() {
	return following;
}

std::string TWUser::getName() {
	return name;
}

void TWUser::AddConnection(IPerson *connection) {
    if(connection!=nullptr){
        following.push_back(connection);
    }
}

void TWUser::RemoveConnection(IPerson *connection) {
	following.erase(std::find(following.begin(),following.end(),connection));
}

void TWUser::AddContent(std::string content) {
	tweets.push_back(content);
}

void TWUser::DisplayFeed() {
	std::cout << "Tweets" << std::endl;
	for(auto i = tweets.begin(); i<tweets.end();++i){
			std::cout << *i << std::endl;
	}
	std::cout << "End" << std::endl;
}

TWUser::TWUser(std::string n){
	name = n;
}