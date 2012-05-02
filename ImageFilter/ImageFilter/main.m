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
 *                  tasks = 2 ^ ts
 * t/taskqueue:     name of taskqueue should used, default GCDTaskQueue
 */
int main(int argc, char *argv[])
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    
//    NSLog(@"%@", [defaults volatileDomainForName:NSArgumentDomain]);
    
    BOOL no_gui = [defaults boolForKey:@"n"];
    if (no_gui) {
        
        AppDelegate *app = [[AppDelegate alloc] init];
        [app applicationDidFinishLaunching:nil];   // initialize app
        NSImageView *imgView = [[NSImageView alloc] init];
        app.imgView = imgView;
        
        NSString *taskQueue = [defaults stringForKey:@"t"];
        [app setTaskQueue:taskQueue];    // set task queue
        
        NSString *input = [defaults stringForKey:@"i"];     // set input image
        if ([app loadImage:input]) {
            
            int granularity = (int)[defaults integerForKey:@"g"];
            [app setGranularity:granularity];    // set granularity
            if ([defaults objectForKey:@"ts"] != nil) {
                int count = [defaults integerForKey:@"ts"];
                count = 1 << count; // pow(2, count);
                [app setTaskCount:count];
            }
            NSString *filterName = [defaults stringForKey:@"f"];
            
            NSTimeInterval d = [defaults integerForKey:@"d"];
            if (d != 0) {
                NSDate *date = [NSDate dateWithTimeIntervalSince1970:d];
                [NSThread sleepUntilDate:date];
            }
                
            
            if ([app applyFilter:filterName]) { // apply filter
                
//                printf("Image: %s\n", [input UTF8String]);
//                printf("TaskQueue: %s\n", [taskQueue UTF8String]);
//                printf("Granularity: %d\n", granularity);
//                printf("Filter: %s\n", [filterName UTF8String]);
                
                [[NSRunLoop currentRunLoop] run];   // run and wait until finished
                
            } else {
                fprintf(stderr, "invalid filter name: '%s'\n", [filterName UTF8String]);
            }
            
        } else {
            fprintf(stderr, "invalid input path: '%s'\n", [input UTF8String]);
        }
        
        return 0;
    } else {
        return NSApplicationMain(argc, (const char **)argv);
    }
}