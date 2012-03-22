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

class Task;

typedef void(^CompletionHandler)(Task *task);

class Task {
private:
    bool _completed;
    bool _executing;
    CompletionHandler _completionHandler;
    std::condition_variable _executingCV;
    
protected:
    std::mutex _mutex;
    
    Task(): _completed(false) {}
    
public:
    // start this task, should only be called from TaskQueue
    void start();
    
    // perform task
    virtual void main() = 0;
    
    // getter
    bool isExecuting() { std::lock_guard<std::mutex> lk(_mutex); return _executing; }
    bool isCompleted() { std::lock_guard<std::mutex> lk(_mutex); return _completed; }
    
    // wait until this task completed
    void join();
    
    void setCompletionHandler(CompletionHandler handler);
};

#endif
