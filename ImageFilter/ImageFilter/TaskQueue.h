//
//  TaskQueue.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_TaskQueue_h
#define ImageFilter_TaskQueue_h

#include <list>
#include <condition_variable>
#include <mutex>
#include <memory.h>

#include "Task.h"
#include "Atomic.h"

class TaskQueue {
    
protected:
    
//    typedef SpinLock LockType;
    typedef std::mutex LockType;
    typedef std::lock_guard<LockType> LockGuard;
    
    // shared mutex, acquire it before modify anything
    LockType _mutex;
    // shared condition variable, use it to wait/notify event
    std::condition_variable_any _cv;
    
public:
    virtual ~TaskQueue() {};
    
    // add a task to queue
    virtual void addTask(TaskRef task) = 0;
    // remove a task from queue
    virtual void removeTask(TaskRef task) = 0;
    // remove all not started taskes
    virtual void removeAllRemainTasks() = 0;
    
    // get a list of all task that not executed yet
    virtual std::list<TaskRef> getTasks() = 0;
    // get count of all unfinished tasks
    virtual unsigned int getTaskCount() = 0;
    
    // block wait until all tasks completed
    virtual void join() {
        std::list<TaskRef> tasks = getTasks();
        std::list<TaskRef>::iterator it;
        for (it = tasks.begin(); it != tasks.end(); it++) {
            (*it)->join();
        }
    };
};

#endif
