//
//  BlockTask.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_BlockTask_h
#define ImageFilter_BlockTask_h

#include "Task.h"

#include <Block.h>

class BlockTask : public Task {
    void (^_block)(void);
    
public:
    
    BlockTask(void(^block)(void)) {
#if __has_feature(objc_arc)
        _block = block;
#else
        _block = Block_copy(block);  
#endif
        
    }
    
    ~BlockTask() {
        Block_release(_block);
    }
    
    virtual void main() {
        _block();
    }
};

#endif
