//
//  GrayscaleFilter.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-23.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_GrayscaleFilter_h
#define ImageFilter_GrayscaleFilter_h

#include "Filter.h"

class GrayscaleFilter : public Filter {
protected:
    virtual void applyFilter(const ImageRef ource, ImageRef arget, size_t x, size_t y);
    
public:
    GrayscaleFilter(ImageRef source, TaskQueue *queue, FilterCompletionHandler handler)
    :Filter(source, queue, handler) {}
    
};

#endif
