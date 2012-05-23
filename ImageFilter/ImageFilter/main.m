//
//  main.m
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "AppDelegate.h"

/* arguments:
 * n/no_gui:        YES - command line only
 *                  NO - have GUI, default value
 * i/input:         path to image to process
 * o/output:        output image path TODO not implemented yet
 * f/filter:        filter name to specify which filter should used
 * g/granularity:   level of granularity / how many task should be created
 *                  number from 1 - 9, 1 means a task per pixel, 9 means only one task
 * ts/tasks:        control how many tasks, override granularity
 *                  for filter, tasks = 2 ^ ts
 *                  for non-computational task, tasks = ts
 * t/taskqueue:     name of taskqueue should used, default GCDTaskQueue
 * tp/threadpool size: size of thread pool queue
 * p/GCD priority:  priority of GCD task queue, 1 - background, 2 - default, 3 - high
 *
 * nc/non-computational task: perform non-computational task and measure the perforamnce
 * st/sleep time:   sleep time for non-computational task
 */
int main(int argc, char *argv[])
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    
    //    NSLog(@"%@", [defaults volatileDomainForName:NSArgumentDomain]);
    
    BOOL no_gui = [defaults boolForKey:@"n"];
    if (no_gui) {
        
        AppDelegate *app = [[AppDelegate alloc] init];
        
        long threadpoolSize = [defaults integerForKey:@"tp"];
        [app setThreadpoolSize:threadpoolSize];
        NSInteger priority = [defaults integerForKey:@"p"];
        switch (priority) {
            case 0:
            case 2:
                [app setGCDPriority:DISPATCH_QUEUE_PRIORITY_DEFAULT];
                break;
            case 1:
                [app setGCDPriority:DISPATCH_QUEUE_PRIORITY_BACKGROUND];
                break;
            case 3:
                [app setGCDPriority:DISPATCH_QUEUE_PRIORITY_HIGH];
                break;
                
            default:
                break;
        }
        
        [app applicationDidFinishLaunching:nil];   // initialize app
        NSImageView *imgView = [[NSImageView alloc] init];
        app.imgView = imgView;
        
        NSString *taskQueue = [defaults stringForKey:@"t"];
        [app setTaskQueue:taskQueue];    // set task queue
        
        NSInteger granularity = [defaults integerForKey:@"g"];
        [app setGranularity:granularity];    // set granularity
        NSInteger taskcount = 0;
        if ([defaults objectForKey:@"ts"] != nil) {
            taskcount = [defaults integerForKey:@"ts"];
            taskcount = 1 << taskcount; // pow(2, count);
            [app setTaskCount:taskcount];
        }
        
        if ([defaults boolForKey:@"nc"]) {
            double sleeptime = [defaults doubleForKey:@"st"];
            [app startNonComputationalTaskWithSleepTime:sleeptime taskCount:[defaults integerForKey:@"ts"]];
            [[NSRunLoop currentRunLoop] run];   // run and wait until finished
        } else {
            NSString *input = [defaults stringForKey:@"i"];     // set input image
            if ([app loadImage:input]) {
                
                NSString *filterName = [defaults stringForKey:@"f"];
                
                NSTimeInterval d = [defaults integerForKey:@"d"];
                if (d != 0) {
                    NSDate *date = [NSDate dateWithTimeIntervalSince1970:d];
                    [NSThread sleepUntilDate:date];
                }
                
                if ([app applyFilter:filterName]) { // apply filter8
                    
                    [[NSRunLoop currentRunLoop] run];   // run and wait until finished
                    
                } else {
                    fprintf(stderr, "invalid filter name: '%s'\n", [filterName UTF8String]);
                }
                
            } else {
                fprintf(stderr, "invalid input path: '%s'\n", [input UTF8String]);
            }
        }
        
        return 0;
    } else {
        return NSApplicationMain(argc, (const char **)argv);
    }
}