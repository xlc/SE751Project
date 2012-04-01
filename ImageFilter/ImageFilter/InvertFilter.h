//
//  InvertFilter.h
//  ImageFilter
//
//  Created by Alice on 2/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_InvertFilter_h
#define ImageFilter_InvertFilter_h

#include "Filter.h"
#include "Image.h"

class InvertFilter : public Filter {
protected:
    virtual void applyFilter(const ImageRef source, ImageRef target, size_t x, size_t y) {
        Pixel p = source->getPixel(x, y);
        for (int i = 0; i < 3; i++) {
            p[i] = 255 - p[i];
            }
        target->setPixel(p, x, y);
    }
    
public:
    InvertFilter(ImageRef source, TaskQueue *queue, int ppt, FilterCompletionHandler handler)
    :Filter(source, queue, ppt, handler) {}
    
};


#endif
