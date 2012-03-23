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

class Filter;
class TaskQueue;
class Image;
class FilterTask;
class Task;

typedef void(^FilterCompletionHandler)(Filter *filter);

class Filter {
protected:
    TaskQueue *_taskQueue;
    Image *_source;
    Image *_result;
    Task **_tasks;
    size_t _w;
    size_t _h;
    FilterCompletionHandler _handler;
    
    // apply filter to source image
    virtual void apply();
    
    // called for each pixel
    virtual void applyFilter(const Image *source, Image *target, size_t x, size_t y) = 0;
    
    friend FilterTask;
    
public:
    Filter(Image *source, TaskQueue *queue, FilterCompletionHandler handler);
    ~Filter();
    
    // getter
    
    TaskQueue *getTaskQueue() const { return _taskQueue; }
    // to make memory management eaiser, caller is response to free the result image
    // will be blocked until all tasks done
    Image *getResult();
    
    // setter
    void setTaskQueue(TaskQueue *queue) { _taskQueue = queue; }
    
};

#endif