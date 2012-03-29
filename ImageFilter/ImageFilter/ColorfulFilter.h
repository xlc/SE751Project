//
//  ColorfulFilter.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_ColorfulFilter_h
#define ImageFilter_ColorfulFilter_h

#include "Filter.h"
#include "Image.h"

class ColorfulFilter : public Filter {
protected:
    virtual void applyFilter(const ImageRef source, ImageRef target, size_t x, size_t y) {
        Pixel p = source->getPixel(x, y);
        for (int i = 0; i < 3; i++) {
            int delta = 0;
            for (int j = 0; j < 3; j++) {
                const unsigned char FULL = ~0;
                double diff = p[i] - p[j];
                if (diff > 0) {
                    int remain = FULL - p[i];
                    if (remain != 0) {
                        delta += MIN(remain / 2, diff);
                    }
                }
            }
            p[i] += delta;
        }
        target->setPixel(p, x, y);
    }
    
public:
    ColorfulFilter(ImageRef source, TaskQueue *queue, int ppt, FilterCompletionHandler handler)
    :Filter(source, queue, ppt, handler) {}
    
};

#endif
