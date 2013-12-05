//
//  BinarySerializer.h
//  lab2
//
//  Created by Alexander Degtyarev on 12/6/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef __lab2__BinarySerializer__
#define __lab2__BinarySerializer__

#include <iostream>
#import "ISerializer.h"

class BinarySerializer: ISerializer{
	virtual void Serialize();
	virtual void Deserialize();
};
#endif /* defined(__lab2__BinarySerializer__) */
