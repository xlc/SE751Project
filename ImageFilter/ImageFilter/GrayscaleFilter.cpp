//
//  GrayscaleFilter.cpp
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-23.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GrayscaleFilter.h"

#include "Image.h"

void GrayscaleFilter::applyFilter(const ImageRef source, ImageRef target, size_t x, size_t y) {
    Pixel p = source->getPixel(x, y);
    int sum = p.r + p.g + p.b;
    p.r = p.g = p.b = sum / 3;
    target->setPixel(p, x, y);
}