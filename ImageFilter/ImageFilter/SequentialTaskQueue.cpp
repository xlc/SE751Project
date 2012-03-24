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

SequentialTaskQueue::SequentialTaskQueue() {
    new std::thread(worker_thread, this);
}

SequentialTaskQueue::~SequentialTaskQueue() {
    _action = StopThread;
    _cv.notify_one();
}

void SequentialTaskQueue::addTask(TaskRef task) {
    {
        std::lock_guard<std::mutex> lk(_mutex);
        _tasks.push_back(task);
        _action = NewTask;
    }
    _cv.notify_one();
}

void SequentialTaskQueue::removeTask(TaskRef task) {
    std::lock_guard<std::mutex> lk(_mutex);
    std::list<TaskRef>::iterator it;
    for (it = _tasks.begin(); it != _tasks.end(); it++) {
        if (*it == task) {
            _tasks.erase(it);
            break;
        }
    }
}

void SequentialTaskQueue::removeAllRemainTasks() {
    std::lock_guard<std::mutex> lk(_mutex);
    _tasks = std::list<TaskRef>();
}

std::list<TaskRef> SequentialTaskQueue::getTasks() {
    return _tasks;
}

unsigned int SequentialTaskQueue::getTaskCount() {
    std::lock_guard<std::mutex> lk(_mutex);
    return (unsigned int)_tasks.size();
}

void worker_thread(SequentialTaskQueue *queue) {
    TaskRef task = nullptr;
    for (;;) {  // run loop
        if (!task) { // if did not get a task last time
                            // than wait
            std::unique_lock<std::mutex> lk(queue->_mutex);
            queue->_cv.wait(lk);
        }
        
        switch (queue->_action) {
            case SequentialTaskQueue::StopThread:
                return;
                
            case SequentialTaskQueue::NewTask:
            {
                task = nullptr;
                {
                    std::lock_guard<std::mutex> lk(queue->_mutex);
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
        }
    }
}