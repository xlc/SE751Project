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
    long NUM_THREADS;
    std::thread **_threads;
    std::list<TaskRef> _tasks;
    volatile enum {
        NoAction,
        StopThread,
        NewTask,
    } _action;
    
    friend void worker_thread(ThreadPoolTaskQueue *queue);
    
public:
    ThreadPoolTaskQueue(): ThreadPoolTaskQueue(sysconf(_SC_NPROCESSORS_ONLN)) {}
    ThreadPoolTaskQueue(long threads);
    ~ThreadPoolTaskQueue();
    
    void addTask(TaskRef task);
    void removeTask(TaskRef task);
    void removeAllRemainTasks();
    std::list<TaskRef> getTasks();
    unsigned int getTaskCount();
    
};

#endif



