//
//  AppDelegate.h
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (weak) IBOutlet NSDrawer *drawer;
@property (weak) IBOutlet NSTextField *imagePathField;
@property (weak) IBOutlet NSImageView *imgView;
@property (weak) IBOutlet NSProgressIndicator *progressBar;
@property (weak) IBOutlet NSTextField *timeLabel;
@property (weak) IBOutlet NSTextField *granularityLabel;

- (IBAction)openImage:(id)sender;
- (IBAction)selectImage:(id)sender;
- (IBAction)toggleDrawer:(id)sender;
- (IBAction)changeTaskQueue:(NSPopUpButton *)sender;
- (IBAction)filterApply:(NSButton *)sender;
- (IBAction)changeGranularity:(NSSlider *)sender;
- (IBAction)GPUFilter:(id)sender;

- (void)setTaskQueue:(NSString *)taskQueueName;
- (void)setGranularity:(NSInteger)granularity;
- (void)setTaskCount:(NSInteger)count;
- (void)setThreadpoolSize:(NSInteger)size;
- (void)setGCDPriority:(dispatch_queue_priority_t)priority;
- (BOOL)loadImage:(NSString *)imagePath;
- (BOOL)applyFilter:(NSString *)filterName;

@end
