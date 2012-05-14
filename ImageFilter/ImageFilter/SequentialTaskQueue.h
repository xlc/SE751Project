//
//  SequentialTaskQueue.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_SequentialTaskQueue_h
#define ImageFilter_SequentialTaskQueue_h

#include <list>
#include <utility>
#include <thread>

#include "TaskQueue.h"

class SequentialTaskQueue : public TaskQueue {
    
public:
    SequentialTaskQueue() {}
    ~SequentialTaskQueue() {}
    
    void addTask(TaskRef task);
};

#endif
