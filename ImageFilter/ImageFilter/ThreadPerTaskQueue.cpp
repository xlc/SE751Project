//
//  ThreadPerTaskQueue.cpp
//  ImageFilter
//
//  Created by Alice on 4/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ThreadPerTaskQueue.h"

#include <thread>
#include <iostream>
#include <mutex>

ThreadPerTaskQueue::ThreadPerTaskQueue()
:_action(NoAction){}

ThreadPerTaskQueue::~ThreadPerTaskQueue() {
    _action = StopThread;
    _cv.notify_all();
    std::list<std::thread*>::iterator threadIt;
    std::thread *thread;
    for(threadIt = _threads.begin(); threadIt!= _threads.end(); threadIt++){
        thread = *threadIt;
        _threads.erase(threadIt);
        delete thread;
    }
}

void ThreadPerTaskQueue::addTask(TaskRef task) {
    {
        LockGuard lk(_mutex);
        _tasks.push_back(task);
        std::thread *thread = new std::thread(worker_thread, this);
        _threads.push_back(thread);
        _action = NewTask;
    }
}

void ThreadPerTaskQueue::removeTask(TaskRef task) {
    LockGuard lk(_mutex);
    std::list<TaskRef>::iterator it;
    for (it = _tasks.begin(); it != _tasks.end(); it++) {
        if (*it == task) {
            _tasks.erase(it);
            break;
        }
    }
}

void ThreadPerTaskQueue::removeAllRemainTasks() {
    LockGuard lk(_mutex);
    _tasks = std::list<TaskRef>();
}

std::list<TaskRef> ThreadPerTaskQueue::getTasks() {
    return _tasks;
}

unsigned int ThreadPerTaskQueue::getTaskCount() {
    LockGuard lk(_mutex);
    return (unsigned int)_tasks.size();
}

void worker_thread(ThreadPerTaskQueue *queue) {
    TaskRef task = nullptr;
    for (;;) {  // run loop
        
        switch (queue->_action) {
            case ThreadPerTaskQueue::StopThread:
                return;
                
            case ThreadPerTaskQueue::NewTask:
            {
                task = nullptr;
                {
                    ThreadPerTaskQueue::LockGuard lk(queue->_mutex);
                    std::list<TaskRef>::iterator it;
                    if (queue->_tasks.size() != 0) {
                        it = queue->_tasks.begin();
                        task = *it;
                        queue->_tasks.erase(it);
                    }
                }   // release lock
                if (task) {
                    task->start();
                    return; //thread has executed task => complete
                }
            }
                break;
                
            case ThreadPerTaskQueue::NoAction:
                return;
        }
    }
}
