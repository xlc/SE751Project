//
//  Image.cpp
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Image.h"


Image::Image(CGImageRef cgImg) {
    size_t w = CGImageGetWidth(cgImg);
    size_t h = CGImageGetHeight(cgImg);
    
    init(w, h, NULL);
    
    CGRect rect = {{0,0},{_width,_height}}; 
    CGContextDrawImage(_context, rect, cgImg); 
}

Image::~Image() {
    CGContextRelease(_context);
    free(_data);
}

void Image::init(size_t width, size_t height, void *bitmapData) {
    _width = width;
    _height = height;
    
    CGColorSpaceRef colorSpace;
    size_t             bitmapByteCount;
    size_t             bitmapBytesPerRow;
    
    bitmapBytesPerRow   = (_width * 4);
    bitmapByteCount     = (bitmapBytesPerRow * _height);
    
    // Use the generic RGB color space.
    colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    assert(colorSpace);
    
    if (!bitmapData)
        bitmapData = malloc( bitmapByteCount );
    assert(bitmapData);
    
    _context = CGBitmapContextCreate (bitmapData,
                                      _width,
                                      _height,
                                      8,      // bits per component
                                      bitmapBytesPerRow,
                                      colorSpace,
                                      kCGImageAlphaPremultipliedLast);
    assert(_context);
    
    CGColorSpaceRelease( colorSpace );
    
    _data = (unsigned char *)CGBitmapContextGetData (_context);
}


