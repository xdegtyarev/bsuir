// Created by Alexander Degtyarev on 11/28/13.
// Copyright (c) 2013 Alexander Degtyarev. All rights reserved.




#include "IDirectory.h"

#ifndef __IMountablePoint_H_
#define __IMountablePoint_H_


class IMountablePoint: public IDirectory {
public:
	virtual bool IsMounted()=0;
};


#endif //__IMountablePoint_H_
