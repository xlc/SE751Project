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

void SequentialTaskQueue::addTask(Task *task, bool freeWhenDone) {
    {
        std::lock_guard<std::mutex> lk(_mutex);
        _tasks.push_back(std::pair<Task *, bool>(task, freeWhenDone));
        _action = NewTask;
    }
    _cv.notify_one();
}

void SequentialTaskQueue::removeTask(Task *task) {
    std::lock_guard<std::mutex> lk(_mutex);
    std::list<std::pair<Task *, bool>>::iterator it;
    for (it = _tasks.begin(); it != _tasks.end(); it++) {
        if (it->first == task) {
            _tasks.erase(it);
            break;
        }
    }
}

void SequentialTaskQueue::removeAllRemainTasks() {
    std::lock_guard<std::mutex> lk(_mutex);
    _tasks = std::list<std::pair<Task *, bool>>();
}

std::vector<Task *> SequentialTaskQueue::getTasks() {
    std::lock_guard<std::mutex> lk(_mutex);
    std::vector<Task *> list(_tasks.size());
    std::list<std::pair<Task *, bool>>::iterator it;
    for (it == _tasks.begin(); it != _tasks.end(); it++) {
        list.push_back(it->first);
    }
    return list;
}

unsigned int SequentialTaskQueue::getTaskCount() {
    std::lock_guard<std::mutex> lk(_mutex);
    return (unsigned int)_tasks.size();
}

void worker_thread(SequentialTaskQueue *queue) {
    Task *task = NULL;
    for (;;) {  // run loop
        if (task == NULL) { // if did not get a task last time
                            // than wait
            std::unique_lock<std::mutex> lk(queue->_mutex);
            queue->_cv.wait(lk);
        }
        
        switch (queue->_action) {
            case SequentialTaskQueue::StopThread:
                return;
                
            case SequentialTaskQueue::NewTask:
            {
                task = NULL;
                bool shouldFree;
                {
                    std::lock_guard<std::mutex> lk(queue->_mutex);
                    std::list<std::pair<Task *, bool>>::iterator it;
                    if (queue->_tasks.size() != 0) {
                        it = queue->_tasks.begin();
                        task = it->first;
                        shouldFree = it->second;
                        queue->_tasks.erase(it);
                    }
                }   // release lock
                if (task) {
                    task->start();
                    if (shouldFree)
                        delete task;
                }
            }
                break;
        }
    }
}