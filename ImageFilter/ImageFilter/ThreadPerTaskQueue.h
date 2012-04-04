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

void worker_thread(ThreadPerTaskQueue *queue);

class ThreadPerTaskQueue : public TaskQueue {
private:
    std::list<std::thread*> _threads;
    std::list<TaskRef> _tasks;
    volatile enum {
        NoAction,
        StopThread,
        NewTask,
    } _action;
    
    friend void worker_thread(ThreadPerTaskQueue *queue);
    
public:
    ThreadPerTaskQueue();
    ~ThreadPerTaskQueue();
    
    void addTask(TaskRef task);
    void removeTask(TaskRef task);
    void removeAllRemainTasks();
    std::list<TaskRef> getTasks();
    unsigned int getTaskCount();
    
};


#endif
