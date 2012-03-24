//
//  Image.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_Image_h
#define ImageFilter_Image_h

#include <memory>

#include <ApplicationServices/ApplicationServices.h>

class Image;

typedef std::shared_ptr<Image> ImageRef;
typedef struct {
    unsigned char r, g, b, a;
} Pixel;

class Image {
private:
    void init(size_t width, size_t height, void *data);
    
protected:
    size_t _width;
    size_t _height;
    unsigned char *_data; // 3d array of width * height * 3(rgbvalue)
    CGContextRef _context;
    
public:
    
    // create image from CGImage
    Image(CGImageRef cgImg);
    // create black image with width and height
    Image(size_t width, size_t height) { init(width, height, NULL); }
    // create image with data
    Image(size_t width, size_t height, unsigned char *data) { init(width, height, data); }
    ~Image();
    
    // getter
    size_t getWidth() const { return _width; }
    size_t getHeight() const { return _height; }
    unsigned char *getData() { return _data; }
    unsigned char const *getData() const { return _data; }
    CGContextRef getContext() { return _context; }
    Pixel getPixel(size_t x, size_t y) const {
        size_t idx = (y * _width + x) * 4;
        Pixel p;
        p.r = _data[idx];
        p.g = _data[idx+1];
        p.b = _data[idx+2];
        p.a = _data[idx+3];
        return p;
    }
    
    // setter
    void setPixel(Pixel p, size_t x, size_t y) {
        size_t idx = (y * _width + x) * 4;
        _data[idx] = p.r;
        _data[idx+1] = p.g;
        _data[idx+2] = p.b;
        _data[idx+3] = p.a;
    }
};

#endif
