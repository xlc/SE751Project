//
//  SepiaFilter.h
//  ImageFilter
//
//  Created by Alice on 2/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_SepiaFilter_h
#define ImageFilter_SepiaFilter_h

#include "Filter.h"
#include "Image.h"

class SepiaFilter : public Filter {
protected:
    virtual void applyFilter(const ImageRef source, ImageRef target, size_t x, size_t y) {
        Pixel p = source->getPixel(x, y);
        int r,g,b;
        r = (int)(p.r * 0.393 + p.g * 0.769 +p.b * 0.189);
        g = (int)(p.r * 0.349 + p.g * 0.686 + p.b * 0.168);
        b = (int)(p.r * 0.272 + p.g * 0.534 + p.b * 0.131);
        
        if (r > 255){
            r = 255;
        }
        if (g > 255){
            g = 255;
        }
        if (b > 255){
            b = 255;
        }
        
        p.r = r;
        p.b = b;
        p.g = g;
        
        target->setPixel(p, x, y);
    }
    
public:
    SepiaFilter(ImageRef source, TaskQueue *queue, int ppt, FilterCompletionHandler handler)
    :Filter(source, queue, ppt, handler) {}
    
};

#endif
