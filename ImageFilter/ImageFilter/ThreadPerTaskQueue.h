//
//  ThreadPerTaskQueue.h
//  ImageFilter
//
//  Created by Alice on 4/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_ThreadPerTaskQueue_h
#define ImageFilter_ThreadPerTaskQueue_h

#include <list>
#include <utility>
#include <thread>

#include "TaskQueue.h"
class ThreadPerTaskQueue;

class ThreadPerTaskQueue : public TaskQueue {
    
public:
    ThreadPerTaskQueue(){};
    
    void addTask(TaskRef task);
};


#endif
