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
        
        [app setTaskQueue:[defaults stringForKey:@"t"]];    // set task queue
        
        NSString *input = [defaults stringForKey:@"i"];     // set input image
        if ([app loadImage:input]) {
            
            [app setGranularity:(int)[defaults integerForKey:@"g"]];    // set granularity
            NSString *filterName = [defaults stringForKey:@"f"];
            if ([app applyFilter:filterName]) { // apply filter
                
                [[NSRunLoop currentRunLoop] run];   // run and wait until finished
                
            } else {
                fprintf(stderr, "invalid filter name: '%s'", [filterName UTF8String]);
            }
            
        } else {
            fprintf(stderr, "invalid input path: '%s'", [input UTF8String]);
        }
        
        return 0;
    } else {
        return NSApplicationMain(argc, (const char **)argv);
    }
}
