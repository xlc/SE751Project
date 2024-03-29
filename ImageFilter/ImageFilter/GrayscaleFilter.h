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
    virtual void applyFilter(const ImageRef source, ImageRef target, size_t x, size_t y) {
        Pixel p = source->getPixel(x, y);
        int sum = p.r + p.g + p.b;
        p.r = p.g = p.b = sum / 3;
        target->setPixel(p, x, y);
    }
    
public:
    GrayscaleFilter(ImageRef source, TaskQueue *queue, int ppt, FilterCompletionHandler handler)
    :Filter(source, queue, ppt, handler) {}
    
};

#endif
