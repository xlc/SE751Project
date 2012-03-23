//
//  Task.cpp
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Task.h"

#include <cassert>
#include <Block.h>
#include <dispatch/dispatch.h>

void Task::start() {
    {
        std::lock_guard<std::mutex> lk(_mutex);
        assert(_completed == false);
        assert(_executing == false);
        _executing = true;
    }
    main();
    {
        std::lock_guard<std::mutex> lk(_mutex);
        _completed = true;
        _executing = false;
    }
    if (_completionHandler) {   // call this method on main thread
        dispatch_async(dispatch_get_main_queue(), ^{
            _completionHandler(this);
        });
    }
    _executingCV.notify_all();
}

void Task::setTaskCompletionHandler(TaskCompletionHandler handler) {
    std::lock_guard<std::mutex> lk(_mutex);
    if (_completionHandler) {
        Block_release(_completionHandler);
        _completionHandler = NULL;
    }
    if (handler)
        _completionHandler = Block_copy(handler);
}

void Task::join() {
    if (_completed)
        return;
    std::unique_lock<std::mutex> lk(_mutex);
    _executingCV.wait(lk);
}