//
//  GCDTaskQueue.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_GCDTaskQueue_h
#define ImageFilter_GCDTaskQueue_h

#include "TaskQueue.h"

#include <dispatch/dispatch.h>
#include <exception>

class GCDTaskQueue : public TaskQueue {
private:
    dispatch_queue_t _queue;
    
public:
    
    GCDTaskQueue() {
        // simply use high priority queue to save work
        _queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    }
    
    void addTask(TaskRef task);
    
    // unimplemented methods
    virtual void removeTask(TaskRef task) { throw std::exception(); }
    virtual void removeAllRemainTasks() { throw std::exception(); }
    virtual std::list<TaskRef> getTasks() { throw std::exception(); }
    virtual unsigned int getTaskCount() { throw std::exception(); }
};

#endif
