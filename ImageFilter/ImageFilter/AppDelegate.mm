//
//  AppDelegate.m
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"

#include "GCDTaskQueue.h"
#include "ThreadPoolTaskQueue.h"
#import "SequentialTaskQueue.h"
#import "GrayscaleFilter.h"
#import "Image.h"

static TaskQueue *taskQueues[3];

@interface AppDelegate ()

@property (nonatomic) Filter *filter;

- (void)handleImage:(NSString *)imagePath;

@end

@implementation AppDelegate {
    TaskQueue * _taskQueue;
    BOOL _processing;
    ImageRef _img;
    NSDate *_startTime;
}

@synthesize window = _window;
@synthesize drawer = _drawer;
@synthesize imagePathField = _imagePathField;
@synthesize imgView = _imgView;
@synthesize progressBar = _progressBar;
@synthesize timeLabel = _timeLabel;
@synthesize filter = _filter;

#pragma mark -

- (void)dealloc {
    if (_filter)
        delete _filter;
}

#pragma mark -

- (void)setFilter:(Filter *)filter {
    if (filter == _filter)
        return;
    if (_filter)
        delete _filter;
    _filter = filter;
}

#pragma mark - NSApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [_drawer open];
    
    // create task queues
    taskQueues[0] = new GCDTaskQueue();
    taskQueues[1] = new SequentialTaskQueue();
//    taskQueues[1] = new ThreadPoolTaskQueue();
    taskQueues[2] = new SequentialTaskQueue();
    
    // set default task queue
    _taskQueue = taskQueues[0];
}

#pragma mark - IBAction

- (IBAction)openImage:(id)sender {
    [self handleImage:_imagePathField.stringValue];
}

- (IBAction)selectImage:(id)sender {
    
    NSOpenPanel* openPanel = [NSOpenPanel openPanel];
    [openPanel setCanChooseFiles:YES];
    [openPanel setCanChooseDirectories:NO];
    [openPanel setAllowsMultipleSelection:NO];
    [openPanel beginSheetModalForWindow:_window completionHandler:^(NSInteger result) {
        if (result == NSFileHandlingPanelOKButton) {
            _imagePathField.stringValue = [[openPanel URL] relativePath];
            [self openImage:nil];
        }
    }];
}

- (IBAction)toggleDrawer:(id)sender {
    [_drawer toggle:self];
}

- (IBAction)grayscaleApply:(id)sender {
    if (!_imgView.image)    // do nothing when no image loaded
        return;
    
    // TODO what to do when user click button while processing a image?
    if (_processing)
        return;
    
    [_progressBar startAnimation:nil];
    _processing = YES;
    
    CGImageRef cgImg = [_imgView.image CGImageForProposedRect:NULL context:NULL hints:NULL];
    _img = ImageRef(new Image(cgImg));
    self.filter = new GrayscaleFilter(_img, _taskQueue, ^(Filter *filter) {
        ImageRef img = filter->getResult();
        CGContextRef context = img->getContext();
        CGImageRef cgImg = CGBitmapContextCreateImage(context);
        _imgView.image = [[NSImage alloc] initWithCGImage:cgImg size:NSMakeSize(img->getWidth(), img->getHeight())];
        [_progressBar stopAnimation:nil];
        _processing = NO;
        NSDate *endTime = [NSDate date];
        NSTimeInterval dt = [endTime timeIntervalSinceDate:_startTime];
        _timeLabel.stringValue = [NSString stringWithFormat:@"Time Taken: %.2lf seconds", dt];
    });
    _startTime = [NSDate date];
    self.filter->apply();
}

- (IBAction)changeTaskQueue:(NSPopUpButton *)sender {
    _taskQueue = taskQueues[sender.indexOfSelectedItem];
}

#pragma mark -

- (void)handleImage:(NSString *)imagePath {
    NSString *path = _imagePathField.stringValue;
    
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
    if (!image) {
        [[NSAlert alertWithMessageText:@"Invalid image path" defaultButton:@"OK" alternateButton:nil otherButton:nil informativeTextWithFormat:@"File '%@' is not a valid image", _imagePathField.stringValue] beginSheetModalForWindow:_window modalDelegate:nil didEndSelector:NULL contextInfo:NULL];
    } else {
        _imgView.image = image;
    }
}

@end
