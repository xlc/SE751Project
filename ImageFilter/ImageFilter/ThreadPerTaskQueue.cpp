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

void worker_thread(TaskRef task);

void ThreadPerTaskQueue::addTask(TaskRef task) {
    LockGuard lk(_mutex);
    std::thread t (worker_thread, task);
    t.detach();
}

void worker_thread(TaskRef task) {
    if (task) {
        task->start();
    }
}
