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

void worker_thread(TaskRef task);

class ThreadPerTaskQueue : public TaskQueue {
private:
    std::list<std::thread*> _threads;
    
    friend void worker_thread(ThreadPerTaskQueue *queue);
    
public:
    ThreadPerTaskQueue(){};
    ~ThreadPerTaskQueue();
    
    void addTask(TaskRef task);
    
    // unimplemented methods
    virtual void removeTask(TaskRef task) { throw std::exception(); }
    virtual void removeAllRemainTasks() { throw std::exception(); }
    virtual std::list<TaskRef> getTasks() { throw std::exception(); }
    virtual unsigned int getTaskCount() { throw std::exception(); }
};


#endif
