//
//  ISerializer.h
//  lab2
//
//  Created by Alexander Degtyarev on 12/6/13.
//  Copyright (c) 2013 Alexander Degtyarev. All rights reserved.
//

#ifndef lab2_ISerializer_h
#define lab2_ISerializer_h
class ISerializer{
public:
	virtual void Serialize()=0;
	virtual void Deserialize()=0;
};
#endif
