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

ThreadPerTaskQueue::~ThreadPerTaskQueue() {
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
        std::thread *thread = new std::thread(worker_thread, task);
        _threads.push_back(thread);
    }
}

void worker_thread(TaskRef task) {
    if (task) {
        task->start();
    }
    
}
