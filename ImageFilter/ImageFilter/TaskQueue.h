//
//  TaskQueue.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_TaskQueue_h
#define ImageFilter_TaskQueue_h

#include <vector>
#include <condition_variable>
#include <mutex>

#include "Task.h"

class TaskQueue {
    
protected:
    std::mutex _mutex;
    std::condition_variable _cv;
    
public:
    
    // add a task to queue
    void addTask(Task *task) { addTask(task, false); }
    // if freeWhenDone than delete task after complete it
    virtual void addTask(Task *task, bool freeWhenDone) = 0;
    // remove a task from queue
    virtual void removeTask(Task *task) = 0;
    // remove all not started taskes
    virtual void removeAllRemainTasks() = 0;
    
    // get a list of all unfinished tasks, includes task not started or executing
    virtual std::vector<Task *> getTasks() = 0;
    // get count of all unfinished tasks
    virtual unsigned int getTaskCount() = 0;
    
    // block wait until all tasks completed
    virtual void join() {
        std::vector<Task *> tasks = getTasks();
        std::vector<Task *>::iterator it;
        for (it = tasks.begin(); it != tasks.end(); it++) {
            (*it)->join();
        }
    };
};

#endif
