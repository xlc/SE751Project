//
//  ThreadPoolTaskQueue.cpp
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "ThreadPoolTaskQueue.h"

#include <thread>
#include <iostream>
#include <mutex>

ThreadPoolTaskQueue::ThreadPoolTaskQueue(long threads)
:NUM_THREADS(threads)
{
    _threads = new std::thread * [NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++){
        _threads[i] = new std::thread(worker_thread, this);
    }
}

ThreadPoolTaskQueue::~ThreadPoolTaskQueue() {
    _action = StopThread;
    _cv.notify_all();
    for(int i = 0; i < NUM_THREADS; i++){
        delete _threads[i];
    }
    delete[] _threads;
}

void ThreadPoolTaskQueue::addTask(TaskRef task) {
    {
        LockGuard lk(_mutex);
        _tasks.push_back(task);
        _action = NewTask;
    }
    _cv.notify_all();
}

void ThreadPoolTaskQueue::removeTask(TaskRef task) {
    LockGuard lk(_mutex);
    std::list<TaskRef>::iterator it;
    for (it = _tasks.begin(); it != _tasks.end(); it++) {
        if (*it == task) {
            _tasks.erase(it);
            break;
        }
    }
}

void ThreadPoolTaskQueue::removeAllRemainTasks() {
    LockGuard lk(_mutex);
    _tasks = std::list<TaskRef>();
}

std::list<TaskRef> ThreadPoolTaskQueue::getTasks() {
    return _tasks;
}

unsigned int ThreadPoolTaskQueue::getTaskCount() {
    LockGuard lk(_mutex);
    return (unsigned int)_tasks.size();
}

void worker_thread(ThreadPoolTaskQueue *queue) {
    TaskRef task = nullptr;
    for (;;) {  // run loop
        if (!task) { // if did not get a task last time
            std::unique_lock<ThreadPoolTaskQueue::LockType> lk(queue->_mutex);
            queue->_cv.wait(lk);
        }
        
        switch (queue->_action) {
            case ThreadPoolTaskQueue::StopThread:
                return;
                
            case ThreadPoolTaskQueue::NewTask:
            {
                task = nullptr;
                {
                    ThreadPoolTaskQueue::LockGuard lk(queue->_mutex);
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