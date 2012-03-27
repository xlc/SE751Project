//
//  Atomic.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ImageFilter_Atomic_h
#define ImageFilter_Atomic_h

#include <libkern/OSAtomic.h>
#include <mutex>

// TODO lock free queue using OSFifoQueueHead

typedef struct AtomicInteger {
    volatile int32_t value;
    
    // methods
    AtomicInteger() { value = 0; }
    AtomicInteger(int32_t i) { value = i; }
    
    int32_t increase() {
        return OSAtomicIncrement32(&value);
    }
    
    int32_t decrease() {
        return OSAtomicDecrement32(&value);
    }
    
    int32_t add(int32_t amount) {
        return OSAtomicAdd32(amount, &value);
    }
    
    bool isEqual(int32_t other) {
        return OSAtomicCompareAndSwap32(other, other, &value);
    }
    
    // TODO make AtomicInteger can be used as an atomic bool using TestAndSet
    
} AtomicInteger;

class SpinLock {
    volatile OSSpinLock l;
    
public:
    SpinLock(): l(OS_SPINLOCK_INIT) {}
    
    void lock() { OSSpinLockLock(&l); }
    bool try_lock() { return OSSpinLockTry(&l); }
    void unlock() { return OSSpinLockUnlock(&l); }
};

#endif
