//
//  BlurFilter.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_BlurFilter_h
#define ImageFilter_BlurFilter_h

#include "Filter.h"
#include "Image.h"

class BlurFilter : public Filter {
protected:
    virtual void applyFilter(const ImageRef source, ImageRef target, size_t x, size_t y) {
        Pixel p;
        size_t w = source->getWidth();
        size_t h = source->getHeight();
        int tot = 0;
        int r = 0;
        int g = 0;
        int b = 0;
        const int RANGE = 10;
        for (int i = -RANGE; i < RANGE; i++) {
            for (int j = -RANGE; j < RANGE; j++) {
                long x1 = x + i;
                long y1 = y + j;
                if (x1 < 0 || x1 >= w)
                    continue;
                if (y1 < 0 || y1 >= h)
                    continue;
                Pixel p2 = source->getPixel(x1, y1);
                r += p2.r;
                g += p2.g;
                b += p2.b;
                ++tot;
            }
        }
        
        p.r = r / tot;
        p.g = g / tot;
        p.b = b / tot;
        p.a = 255;
        
        target->setPixel(p, x, y);
    }
    
public:
    BlurFilter(ImageRef source, TaskQueue *queue, int ppt, FilterCompletionHandler handler)
    :Filter(source, queue, ppt, handler) {}
    
};

#endif
