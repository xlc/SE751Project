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

class TaskQueue;
class Task;

typedef std::shared_ptr<Task> TaskRef;
typedef void(^TaskCompletionHandler)();

class Task {
private:
    bool _completed;
    bool _executing;
    TaskCompletionHandler _completionHandler;
    std::condition_variable _executingCV;
    
protected:
    std::mutex _mutex;
    
    Task()
    : _completed(false), _executing(false), _completionHandler(NULL), _executingCV() {}
    
    void setExecuting(bool b) { std::lock_guard<std::mutex> lk(_mutex); _executing = b; }
    
    friend TaskQueue;
    
public:
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

#endif
