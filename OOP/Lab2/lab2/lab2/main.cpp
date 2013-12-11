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
	bool exiting = false;
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
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::string in;
		switch(response){
			case 1:
		        std::cout << "Type new username:";
		        std::getline(std::cin >> std::ws,in);
		        currentUser = selectedService->CreateUser(in);
		        std::cout << "Name is:" << currentUser->getName() << std::endl;
				break;
			case 2:
				if(currentUser!= nullptr){
					auto connections = currentUser->getConnections();
					int j = 0;
                    if(connections.size() > 0){
					std::cout << "Friends:" << std::endl;
					for(auto i = connections.begin();i<connections.end();++i){
						std::cout << j;
						std::cout << " ";
						std::cout << std::string((*i)->getName());
						std::cout << std::endl;
						j++;
					}
                    }else{
					std::cout << "You have no Friends" << std::endl;
                    }
				}else{
					std::cout << "You have no account, create one" << std::endl;
				}
				break;
			case 3:
				if(currentUser!= nullptr){
					std::cout << "Please enter friend name:";
					std::getline(std::cin >> std::ws,in);
					currentUser->AddConnection(selectedService->TryGetUser((char*)in.c_str()));
				}else{
					std::cout << "You have no account, create one" << std::endl;
				}
				break;
			case 4:
				if(currentUser!= nullptr){
					auto connections = currentUser->getConnections();
					int j = 0;
					std::cout << "Friends:" << std::endl;
					for(auto i = connections.begin();i<connections.end();++i){
						std::cout << j;
						std::cout << " ";
						std::cout << std::string((*i)->getName());
						std::cout << std::endl;
						j++;
					}
					std::cin >> j;
					currentUser->RemoveConnection(currentUser->getConnections()[j]);
				}else{
					std::cout << "You have no account, create one" << std::endl;
				}
				break;
			case 5:
				if(currentUser!= nullptr){
					std::cout << "Write post";
					std::getline(std::cin >> std::ws,in);
			        currentUser->AddContent((char*)in.c_str());
		        }else{
					std::cout << "You have no account, create one" << std::endl;
				}
				break;
			case 6:
				if(currentUser!= nullptr){
					currentUser->DisplayFeed();
				}else{
					std::cout << "You have no account, create one" << std::endl;
				}
				break;
			case 7:
				return;
			default:
				std::cout << "Invalid Input! Any key to continue";
				break;
		}
	}
}

int main(int argc, const char * argv[])
{
	bool exiting = false;
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
				currentUser = nullptr;
		        SocialNetworkMenu();
				break;
			case 2:
				selectedService = &tw;
		        currentUser = nullptr;
		        SocialNetworkMenu();
				break;
			case 3:
				exiting = true;
				break;
			default:
				std::cout << "Invalid Input! Any key to continue" << response;
				break;
		}
	}
	return 0;
}








