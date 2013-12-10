//
//  main.cpp
//  lab2
//
//  Created by Alexander Degtyarev on 12/6/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#include <iostream>
#include <string>
#include "FB.h"
#include "TW.h"

FB fb;
TW tw;
ISocialService* selectedService;
IPerson* currentUser;

void SocialNetworkMenu(){
	bool exiting;
	int response = 0;
	while(!exiting){
		std::cout << "SocialServiceMenu" << std::endl;
		std::cout << "ChooseAction" << std::endl;
		std::cout << "1. CreateUser" << std::endl;
		std::cout << "2. List Connections" << std::endl;
		std::cout << "3. Add Connection" << std::endl;
		std::cout << "4. Remove Connection" << std::endl;
		std::cout << "5. Post" << std::endl;
		std::cout << "6. Show Feed" << std::endl;
		std::cout << "7. Back" << std::endl;
		std::cin >> response;
		switch(response){
			case 1:
				char uname[100];
				std::cout << "Type new username:" << std::endl;
				std::cin >> uname;
		        std::cout << "Name is:" << uname << std::endl;
				currentUser = selectedService->CreateUser(uname);
				break;
			case 2:
				if(currentUser!= nullptr){
					auto connections = currentUser->getConnections();
					int j;
					std::cout << "Friends:" << std::endl;
					for(auto i = connections.begin();i<connections.end();++i){
						std::cout << ++j << " " << (*i)->getName() << std::endl;
					}
					std::cout << "Any key to continue" << std::endl;
					std::cin;
				}else{
					std::cout << "You have no account, create one" << std::endl;
					std::cin;
				}
				break;
			case 3:
				if(currentUser!= nullptr){
					char query[100];
					std::cout << "Please enter friend name:" << std::endl;
					std::cin >> query;
					currentUser->AddConnection(selectedService->TryGetUser(query));
					std::cout << "Any key to continue" << std::endl;
					std::cin;
				}else{
					std::cout << "You have no account, create one" << std::endl;
					std::cin;
				}
				break;
			case 4:
				if(currentUser!= nullptr){
					auto connections = currentUser->getConnections();
					int j;
					std::cout << "Friends:" << std::endl;
					for(auto i = connections.begin();i<connections.end();++i){
						std::cout << ++j << " " << (*i)->getName() << std::endl;
					}
					std::cin >> j;
					currentUser->RemoveConnection(currentUser->getConnections()[j]);
					std::cout << "Any key to continue" << std::endl;
					std::cin;
				}else{
					std::cout << "You have no account, create one" << std::endl;
					std::cin;
				}
				break;
			case 5:
				if(currentUser!= nullptr){
					char post[1000];
					std::cout << "Write post" << std::endl;
					std::cin >> post;
			        currentUser->AddContent(post);
					std::cout << "Any key to continue" << std::endl;
					std::cin;
		        }else{
					std::cout << "You have no account, create one" << std::endl;
					std::cin;
				}
				break;
			case 6:
				if(currentUser!= nullptr){
					currentUser->DisplayFeed();
					std::cout << "Any key to continue" << std::endl;
					std::cin;
				}else{
					std::cout << "You have no account, create one" << std::endl;
					std::cin;
				}
				break;
			case 7:
				return;
			default:
				std::cout << "Invalid Input! Any key to continue";
		        std::cin;
				break;
		}
	}
}

int main(int argc, const char * argv[])
{
	bool exiting;
	int response = 0;
	while(!exiting){
		std::cout << "Lab2" << std::endl;
		std::cout << "SocialService Demo" << std::endl;
		std::cout << "ChooseService" << std::endl;
		std::cout << "1. Facebook" << std::endl;
		std::cout << "2. Twitter" << std::endl;
		std::cout << "3. Exit" << std::endl;
		std::cin >> response;
		switch(response){
			case 1:
				selectedService = &fb;
		        SocialNetworkMenu();
				break;
			case 2:
				selectedService = &tw;
		        SocialNetworkMenu();
				break;
			case 3:
				exiting = true;
				break;
			default:
				std::cout << "Invalid Input! Any key to continue" << response;
				std::cin;
				break;
		}
	}
	return 0;
}








