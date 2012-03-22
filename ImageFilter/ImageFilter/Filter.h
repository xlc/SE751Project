//
//  Filter.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_Filter_h
#define ImageFilter_Filter_h

#include "Image.h"

class Filter {
protected:
    virtual void applyFilter(const Image *source, Image *target, size_t x, size_t y) = 0;
    
public:
    Image *apply(const Image *source);
    
};

#endif
