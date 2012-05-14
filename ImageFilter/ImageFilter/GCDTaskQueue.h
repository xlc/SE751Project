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
    
    GCDTaskQueue():GCDTaskQueue(DISPATCH_QUEUE_PRIORITY_HIGH) {}
    
    GCDTaskQueue(dispatch_queue_priority_t p) {
        _queue = dispatch_get_global_queue(p, 0);
    }
    
    void addTask(TaskRef task);
};

#endif
