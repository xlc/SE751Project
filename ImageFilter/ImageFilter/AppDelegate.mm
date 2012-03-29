//
//  AppDelegate.m
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"

#import "GCDTaskQueue.h"
#import "ThreadPoolTaskQueue.h"
#import "SequentialTaskQueue.h"
#import "GrayscaleFilter.h"
#import "ColorfulFilter.h"
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
    int _pixelsPerTask;
}

@synthesize window = _window;
@synthesize drawer = _drawer;
@synthesize imagePathField = _imagePathField;
@synthesize imgView = _imgView;
@synthesize progressBar = _progressBar;
@synthesize timeLabel = _timeLabel;
@synthesize granularityLabel = _granularityLabel;
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
    
    _pixelsPerTask = 1;
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

- (IBAction)filterApply:(NSButton *)sender {
    if (!_imgView.image)    // do nothing when no image loaded
        return;
    
    // TODO what to do when user click button while processing a image?
    if (_processing)
        return;
    
    [_progressBar startAnimation:nil];
    _processing = YES;
    
    CGImageRef cgImg = [_imgView.image CGImageForProposedRect:NULL context:NULL hints:NULL];
    _img = ImageRef(new Image(cgImg));
    FilterCompletionHandler handler = ^(Filter *filter) {
        ImageRef img = filter->getResult();
        CGContextRef context = img->getContext();
        CGImageRef cgImg = CGBitmapContextCreateImage(context);
        _imgView.image = [[NSImage alloc] initWithCGImage:cgImg size:NSMakeSize(img->getWidth(), img->getHeight())];
        [_progressBar stopAnimation:nil];
        _processing = NO;
        NSDate *endTime = [NSDate date];
        NSTimeInterval dt = [endTime timeIntervalSinceDate:_startTime];
        _timeLabel.stringValue = [NSString stringWithFormat:@"Time Taken: %.2lf seconds", dt];
    };
    
    NSString *buttonName = [sender title];
    if ([buttonName isEqualToString:@"Grayscale"]) {
        self.filter = new GrayscaleFilter(_img, _taskQueue, _pixelsPerTask, handler);
    } else if ([buttonName isEqualToString:@"Colorful"]) {
        self.filter = new ColorfulFilter(_img, _taskQueue, _pixelsPerTask, handler);
    }
    
    _startTime = [NSDate date];
    self.filter->apply();
}

- (IBAction)changeGranularity:(NSSlider *)sender {
    if (!_imgView.image)    // do nothing when no image loaded
        return;
    
    CGSize size = _imgView.image.size;
    int area = size.width * size.height;
    double sliderValue = sender.doubleValue;
    if (sliderValue == 1.0) {
        _pixelsPerTask = area;
    } else if (sliderValue == 0.0) {
        _pixelsPerTask = 1;
    } else {
        int devide = (1 - sliderValue) * 1000;
        devide = MAX(2, devide);
        _pixelsPerTask = area/devide;
    }
    _granularityLabel.stringValue = [NSString stringWithFormat:@"Granularity: %dpx", _pixelsPerTask];
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
