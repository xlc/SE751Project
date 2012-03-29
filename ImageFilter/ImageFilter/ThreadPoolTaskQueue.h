//
//  ThreadPoolTaskQueue.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_ThreadPoolTaskQueue_h
#define ImageFilter_ThreadPoolTaskQueue_h

#include <list>
#include <utility>
#include <thread>

#include "TaskQueue.h"
class ThreadPoolTaskQueue;

void worker_thread(ThreadPoolTaskQueue *queue);

class ThreadPoolTaskQueue : public TaskQueue {
private:
    int NUM_THREADS = sysconf(_SC_NPROCESSORS_ONLN);
    std::thread *_threads[32];
    std::list<TaskRef> _tasks;
    volatile enum {
        StopThread,
        NewTask,
    } _action;
    
    friend void worker_thread(ThreadPoolTaskQueue *queue);
    
public:
    ThreadPoolTaskQueue();
    ~ThreadPoolTaskQueue();
    
    void addTask(TaskRef task);
    void removeTask(TaskRef task);
    void removeAllRemainTasks();
    std::list<TaskRef> getTasks();
    unsigned int getTaskCount();
    
};

#endif



