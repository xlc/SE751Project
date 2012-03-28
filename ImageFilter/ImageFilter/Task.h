//
//  Task.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_Task_h
#define ImageFilter_Task_h

#include <mutex>
#include <condition_variable>
#include <memory>
#include <vector>
#include <dispatch/dispatch.h>

#include "Atomic.h"

class TaskQueue;
class Task;
class TaskGroup;

typedef std::shared_ptr<Task> TaskRef;
typedef void(^TaskCompletionHandler)();

class Task {
private:
    bool _completed;
    bool _executing;
    TaskCompletionHandler _completionHandler;
    std::condition_variable _cv;
    TaskGroup * _group;
    
protected:
    std::mutex _mutex;
    
    Task()
    : _completed(false), _executing(false), _completionHandler(nullptr), _cv(), _group(nullptr) {}
    
    void setExecuting(bool b) { std::lock_guard<std::mutex> lk(_mutex); _executing = b; }
    TaskGroup * getGroup() { return _group; }
    void setGroup(TaskGroup * group) { _group = group; }
    
    friend TaskQueue;
    friend TaskGroup;
    
public:
    virtual ~Task();
    
    // start this task, should only be called from TaskQueue
    void start();
    
    // perform task
    virtual void main() = 0;
    
    // getter
    bool isExecuting() {
        std::lock_guard<std::mutex> lk(_mutex);
        return _executing;
    }
    bool isCompleted() {
        std::lock_guard<std::mutex> lk(_mutex);
        return _completed;
    }
    
    // wait until this task completed
    void join();
    
    void setTaskCompletionHandler(TaskCompletionHandler handler);
};

class TaskGroup {
private:
    AtomicInteger _count;
    std::mutex _mutex;
    std::condition_variable _cv;
    TaskCompletionHandler _completionHandler;
    std::vector<TaskRef> _tasks;

protected:
    
    void taskCompleted(Task *task) {
        if (_count.decrease() == 0) {
            _cv.notify_all();
            if (_completionHandler) {
                dispatch_async(dispatch_get_main_queue(), _completionHandler);
            }
        }
    }
    
    const std::vector<TaskRef> & getTasks() const { return _tasks; }
    
    friend Task;
    friend TaskQueue;
    
public:
    TaskGroup(): _count(0), _mutex(), _cv(), _completionHandler(nullptr) {}
    
    void addTask(TaskRef &task) {
        _count.increase();
        task->setGroup(this);
        _tasks.push_back(task);
    }
    
    void setCompletionHandler(TaskCompletionHandler handler);
    
    void join() {
        if (_count.isEqual(0))
            return;
        std::unique_lock<std::mutex> lk(_mutex);
        _cv.wait(lk);
    }
};

#endif
