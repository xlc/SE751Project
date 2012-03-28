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
    size_t _x;
    size_t _y;
    
public:
    FilterTask(Filter *filter, const ImageRef source, ImageRef target, size_t x, size_t y)
    : Task(), _filter(filter), _source(source), _target(target), _x(x), _y(y)
    {}
    
    virtual void main() {
        _filter->applyFilter(_source, _target, _x, _y);
    }
    
};

#pragma mark -

Filter::Filter(ImageRef source, TaskQueue *queue, FilterCompletionHandler handler)
: _taskQueue(queue), _source(source), _result(nullptr), _w(0), _h(0) {
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
        
        for (int x = 0; x < _w; x++) {
            for (int y = 0; y < _h; y++) {
                TaskRef task(new FilterTask(this, _source, _result, x, y));           
                group->addTask(task);
            }
        }
        
        _taskQueue->addTaskGroup(group);
    }));
    
    _taskQueue->addTask(blockTask);
}

ImageRef Filter::getResult() {
    return _result;
}