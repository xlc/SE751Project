//
//  EdgeDetectorFilter.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_EdgeFilter_h
#define ImageFilter_EdgeFilter_h

#include "Filter.h"
#include "Image.h"

class EdgeFilter : public Filter {
protected:
    virtual void applyFilter(const ImageRef source, ImageRef target, size_t x, size_t y) {
        Pixel p = source->getPixel(x, y);
        size_t w = source->getWidth();
        size_t h = source->getHeight();
        int diff = 0;
        int tot = 0;
        for (int i = -1; i < 1; i++) {
            for (int j = -1; j < 1; j++) {
                int x1 = x + i;
                int y1 = y + j;
                if (x1 < 0 || x1 >= w)
                    continue;
                if (y1 < 0 || y1 >= h)
                    continue;
                Pixel p2 = source->getPixel(x1, y1);
                for (int k = 0; k < 3; k++) {
                    diff += abs(p[k] - p2[k]);
                }
                ++tot;
            }
        }
        
        p[0] = p[1] = p[2] = MAX(diff * 3, 255);
        
        target->setPixel(p, x, y);
    }
    
public:
    EdgeFilter(ImageRef source, TaskQueue *queue, int ppt, FilterCompletionHandler handler)
    :Filter(source, queue, ppt, handler) {}
    
};

#endif
