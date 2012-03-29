//
//  Filter.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_Filter_h
#define ImageFilter_Filter_h

#include <ApplicationServices/ApplicationServices.h>

#include "Image.h"

class Filter;
class TaskQueue;
class FilterTask;
class Task;

typedef void(^FilterCompletionHandler)(Filter *filter);

class Filter {
protected:
    TaskQueue *_taskQueue;
    ImageRef _source;
    ImageRef _result;
    size_t _w;
    size_t _h;
    FilterCompletionHandler _handler;
    int _pixelPerTask;
    
    // called for each pixel
    virtual void applyFilter(const ImageRef source, ImageRef target, size_t x, size_t y) = 0;
    
    friend FilterTask;
    
public:
    Filter(ImageRef source, TaskQueue *queue, int ppt, FilterCompletionHandler handler);
    ~Filter();
    
    // apply filter to source image
    void apply();
    
    // getter
    TaskQueue *getTaskQueue() const { return _taskQueue; }
    // will return result image, non-blocking
    ImageRef getResult();
    
    // setter
    void setTaskQueue(TaskQueue *queue) { _taskQueue = queue; }
    
};

#endif