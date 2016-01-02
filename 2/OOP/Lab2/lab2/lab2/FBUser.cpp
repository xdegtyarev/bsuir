// Created by Alexander Degtyarev on 12/10/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.



#include <iostream>
#include "FBUser.h"

std::vector<IPerson *> FBUser::getConnections() {
	return friends;
}

std::string FBUser::getName() {
	return name;
}

void FBUser::AddConnection(IPerson *connection) {
    if(connection!=nullptr){
	friends.push_back(connection);
    }
}

void FBUser::RemoveConnection(IPerson *connection) {
	friends.erase(std::find(friends.begin(),friends.end(),connection));
}

void FBUser::AddContent(std::string content) {
	feed.push_back(content);
}

void FBUser::DisplayFeed() {
	std::cout << "Feed" << std::endl;
	for(auto i = feed.begin();i<feed.end();++i){
		std::cout << *i << std::endl;
	}
	std::cout << "End" << std::endl;
	std::cin;
}

FBUser::FBUser(std::string n,int i){
	id = i;
	name = n;
}