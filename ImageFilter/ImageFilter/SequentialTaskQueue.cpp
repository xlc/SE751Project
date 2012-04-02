//
//  SequentialTaskQueue.cpp
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "SequentialTaskQueue.h"

#include <thread>
#include <iostream>
#include <mutex>

//static AtomicInteger count;

SequentialTaskQueue::SequentialTaskQueue()
: _action(NoAction)
{
    _thread = new std::thread(worker_thread, this);
}

SequentialTaskQueue::~SequentialTaskQueue() {
    _action = StopThread;
    _cv.notify_one();
    delete _thread;
}

void SequentialTaskQueue::addTask(TaskRef task) {
//    printf("add %d\n", count.increase());
    {
        LockGuard lk(_mutex);
        _tasks.push_back(task);
        _action = NewTask;
        _cv.notify_one();
    }
}

void SequentialTaskQueue::removeTask(TaskRef task) {
    LockGuard lk(_mutex);
    std::list<TaskRef>::iterator it;
    for (it = _tasks.begin(); it != _tasks.end(); it++) {
        if (*it == task) {
            _tasks.erase(it);
            break;
        }
    }
}

void SequentialTaskQueue::removeAllRemainTasks() {
    LockGuard lk(_mutex);
    _tasks = std::list<TaskRef>();
}

std::list<TaskRef> SequentialTaskQueue::getTasks() {
    return _tasks;
}

unsigned int SequentialTaskQueue::getTaskCount() {
    LockGuard lk(_mutex);
    return (unsigned int)_tasks.size();
}

void worker_thread(SequentialTaskQueue *queue) {
    TaskRef task = nullptr;
    for (;;) {  // run loop
        if (!task) { // if did not get a task last time
                     // than wait
            std::unique_lock<SequentialTaskQueue::LockType> lk(queue->_mutex);
//            printf("wait %d\n", count.increase());
            queue->_cv.wait(lk);
//            printf("awake %d\n", count.increase());
        }
        
        switch (queue->_action) {
            case SequentialTaskQueue::StopThread:
                return;
                
            case SequentialTaskQueue::NewTask:
            {
                task = nullptr;
                {
                    SequentialTaskQueue::LockGuard lk(queue->_mutex);
                    std::list<TaskRef>::iterator it;
                    if (queue->_tasks.size() != 0) {
                        it = queue->_tasks.begin();
                        task = *it;
                        queue->_tasks.erase(it);
                    }
                }   // release lock
                if (task) {
                    task->start();
                }
            }
                break;
            case SequentialTaskQueue::NoAction:
                break;
        }
    }
}