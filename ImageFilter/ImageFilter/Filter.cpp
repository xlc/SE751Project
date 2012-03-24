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

class FilterTask : public Task {
    Filter *_filter;
    const Image *_source;
    Image *_target;
    size_t _x;
    size_t _y;
    
public:
    FilterTask(Filter *filter, const Image *source, Image *target, size_t x, size_t y)
    : Task(), _filter(filter), _source(source), _target(target), _x(x), _y(y)
    {}
    
    virtual void main() {
        _filter->applyFilter(_source, _target, _x, _y);
    }
    
};

#pragma mark -

Filter::Filter(Image *source, TaskQueue *queue, FilterCompletionHandler handler)
: _taskQueue(queue), _source(source), _result(NULL), _tasks(NULL), _w(0), _h(0) {
    _handler = Block_copy(handler);
}

Filter::~Filter() {
    if (_tasks) {
        for (int i = 0; i < _w * _h; i++) {
            delete _tasks[i];
        }
        delete [] _tasks;
    }
    Block_release(_handler);
}

void Filter::apply() {
    assert(_taskQueue);
    
    if (_result)
        return;
    
    _w = _source->getWidth();
    _h = _source->getHeight();
    
    _tasks = new Task *[_w * _h];
    _result = new Image(_w, _h);
    
    __block size_t taskCount = _w * _h;
    pthread_mutex_t *lock = NULL;
    if (_handler) {
        lock = new pthread_mutex_t;
        assert(pthread_mutex_init(lock, NULL) == 0);
    }
    
    BlockTask *blockTask = new BlockTask(^{
        int i = 0;
        for (int x = 0; x < _w; x++) {
            for (int y = 0; y < _h; y++) {
                FilterTask *task = new FilterTask(this, _source, _result, x, y);
                if (_handler) {
                    task->setTaskCompletionHandler(^(Task *task) {
                        assert(pthread_mutex_lock(lock) == 0);
                        taskCount--;
                        bool done = taskCount == 0;
                        assert(pthread_mutex_unlock(lock) == 0);
                        if (done) {
                            pthread_mutex_destroy(lock);
                            delete lock;
                            _handler(this);
                        }
                    });
                }
                _taskQueue->addTask(task);
                _tasks[i++] = task;
            }
        }

    });
    
    _taskQueue->addTask(blockTask, true);
}

Image * Filter::getResult() {
    if (_tasks) {
        for (int i = 0; i < _w * _h; i++) {
            _tasks[i]->join();  // wait until all tasks completed
        }
        for (int i = 0; i < _w * _h; i++) {
            delete _tasks[i];
        }
        delete [] _tasks;
        _tasks = NULL;
    }
    return _result;
}