//
//  main.cpp
//  ipw2
//
//  Created by Alexander Degtyarev on 12/2/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#include "XFS.h"
#include "NTFS.h"
#include "NTFSFactory.h"
#include "UFSFactory.h"

void MainMenu();
void InitFS();
void Tree();
void ItemCreationMenu();
bool active;
IVFS* currentFS;
int currentFSIndex;
IVFSFactory* factory[] = {new NTFSFactory(),new UFSFactory(),new XFSFactory()};
IDirectory* currentDirectory;

int main(int argc, const char * argv[])
{
    MainMenu();
    return 0;
}

void MainMenu(){
    active = true;
    while (active) {
        system("cls");
        std::cout<<"MENU";
        std::cout<<"\n1.InitFS"<<"\n2.Create Item"<<"\n3.Tree"<<"\n4.Exit"<<std::endl;
        char response;
        std::cin >> response;
        switch(response){
            case '1':
            InitFS();
            break;
            case '2':
                if(currentFS==nullptr){
                    std::cout<<"specify current fs";
                    std::cin>>response;
                    InitFS();
                }
                ItemCreationMenu();
            break;
            case '3':
                if(currentFS==nullptr){
                    std::cout<<"specify current fs";
                    std::cin>>response;
                    InitFS();
                }
				currentDirectory->ListContentToConsole();
            break;
            case '4':
            active = false;
            break;
        }
    }
}

void InitFS(){

    bool invalidInput = true;
    while(invalidInput){
        system("cls");
        std::cout<<"Creating FS";
        std::cout<<"\n1.NTFS"<<"\n2.UFS"<<"\n3.XFS"<<"\n4.Exit"<<std::endl;
        char response;
        std::cin >> response;
	    switch(response){
	        case '1':
		        currentFS = factory[0]->CreateVFS();
	            currentFSIndex=0;
				invalidInput = false;
		    break;
		    case '2':
			    currentFS = factory[1]->CreateVFS();
	            currentFSIndex=1;
	            invalidInput = false;
	        break;
		    case '3':
	            currentFS = factory[2]->CreateVFS();
	            currentFSIndex=2;
	            invalidInput = false;
	        break;
		    case '4':
			    return;
	        break;
			default:
			    std::cout << "Wrong input. Press any key to continue";
		        std::cin >> response;
	        break;
	    }
    }
	currentDirectory = currentFS->GetRoot();
	std::cout << "Driver Name:" << currentFS->GetDriverName() << std::endl;
	std::cout << "Encoding:" << currentFS->GetFileNameEncodingInfo() << std::endl;
}
void ItemCreationMenu(){
	bool invalidInput = true;
	while(invalidInput){
		system("cls");
		std::cout<<"CreationMenu";
		std::cout<<"\n1.File"<<"\n2.Folder"<<"\n3.Exit"<<std::endl;
		char response;
		std::cin >> response;
		switch(response){
			case '1':
				currentDirectory->AddChild(factory[currentFSIndex]->CreateDir());
		        invalidInput = false;
		        break;
			case '2':
				currentDirectory->AddChild(factory[currentFSIndex]->CreateFile());
		        invalidInput = false;
		        break;
			case '3':
				return;
		        break;
			default:
				std::cout << "Wrong input. Press any key to continue";
		        std::cin >> response;
		        break;
		}
	}
	std::cin;
}