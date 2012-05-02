//
//  SequentialTaskQueue.cpp
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "SequentialTaskQueue.h"

void SequentialTaskQueue::addTask(TaskRef task) {
    task->start();
}