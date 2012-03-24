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

#include "TaskQueue.h"

class SequentialTaskQueue;

void worker_thread(SequentialTaskQueue *queue);

class SequentialTaskQueue : public TaskQueue {
private:
    std::list<TaskRef> _tasks;
    volatile enum {
        StopThread,
        NewTask,
    } _action;
    
    friend void worker_thread(SequentialTaskQueue *queue);
    
public:
    SequentialTaskQueue();
    ~SequentialTaskQueue();
    
    void addTask(TaskRef task);
    void removeTask(TaskRef task);
    void removeAllRemainTasks();
    std::list<TaskRef> getTasks();
    unsigned int getTaskCount();
};

#endif
