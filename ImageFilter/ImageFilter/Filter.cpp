//
//  Filter.cpp
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-23.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Filter.h"

#include <pthread.h>
#include <iostream>

#include "TaskQueue.h"
#include "Task.h"
#include "Image.h"
#include "BlockTask.h"
#include "Atomic.h"

class FilterTask : public Task {
    Filter *_filter;
    const ImageRef _source;
    ImageRef _target;
    size_t _start;
    size_t _length;
    
public:
    FilterTask(Filter *filter, const ImageRef source, ImageRef target, size_t start, size_t length)
    : Task(), _filter(filter), _source(source), _target(target), _start(start), _length(length)
    {}
    
    virtual void main() {
        size_t w = _source->getWidth();
        for (int i = 0; i < _length; i++) {
            _filter->applyFilter(_source, _target, (_start+i) % w, (_start+i) / w);
        }
    }
    
};

#pragma mark -

Filter::Filter(ImageRef source, TaskQueue *queue, int ppt, FilterCompletionHandler handler)
: _taskQueue(queue), _source(source), _result(nullptr), _w(0), _h(0), _pixelPerTask(ppt) {
    _handler = Block_copy(handler);
}

Filter::~Filter() {
    Block_release(_handler);
}

void Filter::apply() {
    assert(_taskQueue);
    if (_result)
        return;
    
    _w = _source->getWidth();
    _h = _source->getHeight();
    
    _result = ImageRef(new Image(_w, _h));
    
    TaskRef blockTask(new BlockTask(^{
        TaskGroup *group = new TaskGroup();
        if (_handler) {
            group->setCompletionHandler(^{
                _handler(this);
                delete group;
            });
        }
        
        size_t length = _w * _h;
        size_t l = length - length % _pixelPerTask;
        for (size_t i = 0; i < l; i+=_pixelPerTask) {
            TaskRef task(new FilterTask(this, _source, _result, i, _pixelPerTask));
            group->addTask(task);
        }
        
        // explicitly handle last task
        size_t diff = length - l;
        if (diff != 0) {
            TaskRef task(new FilterTask(this, _source, _result, l, diff));
            group->addTask(task);
        }
        
        _taskQueue->addTaskGroup(group);
    }));
    
    _taskQueue->addTask(blockTask);
}

ImageRef Filter::getResult() {
    return _result;
}