//
//  XMLSerializer.h
//  lab2
//
//  Created by Alexander Degtyarev on 12/6/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __lab2__XMLSerializer__
#define __lab2__XMLSerializer__

#include <iostream>
#import "ISerializer.h"

class XMLSerializer: ISerializer{
	virtual void Serialize(int typeId,);
	virtual void Deserialize();
};
#endif /* defined(__lab2__XMLSerializer__) */
