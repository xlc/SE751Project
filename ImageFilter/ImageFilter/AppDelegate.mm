//
//  AppDelegate.m
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <sys/time.h>

#import "AppDelegate.h"

#import "time.h"

#import "GCDTaskQueue.h"
#import "ThreadPoolTaskQueue.h"
#import "SequentialTaskQueue.h"
#import "ThreadPerTaskQueue.h"

#import "GrayscaleFilter.h"
#import "ColorfulFilter.h"
#import "InvertFilter.h"
#import "SepiaFilter.h"
#import "EdgeFilter.h"
#import "BlurFilter.h"
#import "Image.h"
#import "CIEdgeFilter.h"

#import "BlockTask.h"

static TaskQueue *taskQueues[4];

@interface AppDelegate ()

@property (nonatomic) Filter *filter;

@end

@implementation AppDelegate {
    TaskQueue * _taskQueue;
    BOOL _processing;
    ImageRef _img;
    int _pixelsPerTask;
    NSInteger _threadpoolSize;
    dispatch_queue_priority_t _GCDPriority;
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

- (void)setThreadpoolSize:(NSInteger)size {
    _threadpoolSize = size;
}

- (void)setGCDPriority:(dispatch_queue_priority_t)priority {
    _GCDPriority = priority;
}

#pragma mark - NSApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [_drawer open];
    
    if (_threadpoolSize == 0) {
        _threadpoolSize = sysconf(_SC_NPROCESSORS_ONLN);
    }
    
    // create task queues
    taskQueues[0] = new GCDTaskQueue(_GCDPriority);
    taskQueues[1] = new ThreadPoolTaskQueue(_threadpoolSize);
    taskQueues[2] = new SequentialTaskQueue();
    taskQueues[3] = new ThreadPerTaskQueue();
    
    // set default task queue
    _taskQueue = taskQueues[0];
    
    _pixelsPerTask = 1;
}

#pragma mark - IBAction

- (IBAction)openImage:(id)sender {
    if (![self loadImage:_imagePathField.stringValue]) {
        [[NSAlert alertWithMessageText:@"Invalid image path"
                         defaultButton:@"OK"
                       alternateButton:nil
                           otherButton:nil
             informativeTextWithFormat:@"File '%@' is not a valid image", _imagePathField.stringValue]
         beginSheetModalForWindow:_window modalDelegate:nil
         didEndSelector:NULL contextInfo:NULL];
    }
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
    
    [self applyFilter:[sender title]];
}

- (IBAction)changeGranularity:(NSSlider *)sender {
    if (!_imgView.image)    // do nothing when no image loaded
        return;
    
    [self setGranularity:sender.intValue];
    
    _granularityLabel.stringValue = [NSString stringWithFormat:@"Granularity: %dpx", _pixelsPerTask];
}

- (IBAction)changeTaskQueue:(NSPopUpButton *)sender {
    _taskQueue = taskQueues[sender.indexOfSelectedItem];
}

- (IBAction)GPUFilter:(id)sender {
    if (!_imgView.image)    // do nothing when no image loaded
        return;
    init_time();
    CIEdgeFilter *filter = [[CIEdgeFilter alloc] init];
    CIImage *ciImg = [CIImage imageWithCGImage:[_imgView.image CGImageForProposedRect:NULL context:NULL hints:NULL]];
    [filter setValue:ciImg forKey:@"inputImage"];
    CIImage *result = [filter valueForKey:@"outputImage"];
    NSBitmapImageRep *rep = [[NSBitmapImageRep alloc] initWithCIImage:result];
    _imgView.image = [[NSImage alloc] initWithCGImage:rep.CGImage size:rep.size];
    stop_time();
    _timeLabel.stringValue = [NSString stringWithFormat:@"Time Taken: %.2lf seconds", get_time_diff()];
}

#pragma mark -

- (void)setTaskQueue:(NSString *)taskQueueName {
    if ([taskQueueName isEqualToString:@"GCDTaskQueue"]) {
        _taskQueue = taskQueues[0];
    } else if ([taskQueueName isEqualToString:@"ThreadPoolTaskQueue"]) {
        _taskQueue = taskQueues[1];
    } else if ([taskQueueName isEqualToString:@"SingleThreadTaskQueue"]) {
        _taskQueue = taskQueues[2];
    } else if ([taskQueueName isEqualToString:@"ThreadPerTaskQueue"]) {
        _taskQueue = taskQueues[3];
    }
}

- (void)setGranularity:(NSInteger)granularity {
    if (!_imgView.image)    // do nothing when no image loaded
        return;
    
    CGSize size = _imgView.image.size;
    int area = size.width * size.height;
    switch(granularity) {   // 0 to 8
        case 0:
            _pixelsPerTask = 1;
            break;
        case 1:
            _pixelsPerTask = 4;
            break;
        case 2:
            _pixelsPerTask = 32;
            break;
        case 3:
            _pixelsPerTask = 128;
            break;
        case 4:
            _pixelsPerTask = sqrtf(area);
            break;
        case 5:
            _pixelsPerTask = area / 128;
            break;
        case 6:
            _pixelsPerTask = area / 32;
            break;
        case 7:
            _pixelsPerTask = area / 4;
            break;
        case 8:
            _pixelsPerTask = area;
            break;
    }
}

- (void)setTaskCount:(NSInteger)count {
    CGSize size = _imgView.image.size;
    int area = size.width * size.height;
    _pixelsPerTask = area / count;
}

- (BOOL)loadImage:(NSString *)imagePath {
    
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:imagePath];
    if (!image) {
        return NO;
    } else {
        _imgView.image = image;
        return YES;
    }
}

- (BOOL)applyFilter:(NSString *)filterName {
    CGImageRef cgImg = [_imgView.image CGImageForProposedRect:NULL context:NULL hints:NULL];
    _img = ImageRef(new Image(cgImg));
    FilterCompletionHandler handler = ^(Filter *filter) {
        ImageRef img = filter->getResult();
        CGContextRef context = img->getContext();
        CGImageRef cgImg = CGBitmapContextCreateImage(context);
        _imgView.image = [[NSImage alloc] initWithCGImage:cgImg size:NSMakeSize(img->getWidth(), img->getHeight())];
        CGImageRelease(cgImg);
        [_progressBar stopAnimation:nil];
        _processing = NO;
        stop_time();
        double dt = get_time_diff();
        _timeLabel.stringValue = [NSString stringWithFormat:@"Time Taken: %.2lf seconds", dt];
        
        if ([[NSUserDefaults standardUserDefaults] boolForKey:@"n"]) {
            // run in command line
            
            printf("%.4lf\n", dt);
            
            exit(0);    // end
        }
    };
    
    if ([filterName isEqualToString:@"Grayscale"]) {
        self.filter = new GrayscaleFilter(_img, _taskQueue, _pixelsPerTask, handler);
    } else if ([filterName isEqualToString:@"Colorful"]) {
        self.filter = new ColorfulFilter(_img, _taskQueue, _pixelsPerTask, handler);
    } else if ([filterName isEqualToString:@"Invert"]){
        self.filter = new InvertFilter(_img, _taskQueue, _pixelsPerTask, handler);
    } else if ([filterName isEqualToString:@"Sepia"]){
        self.filter = new SepiaFilter(_img, _taskQueue, _pixelsPerTask, handler);
    } else if ([filterName isEqualToString:@"Edge"]){
        self.filter = new EdgeFilter(_img, _taskQueue, _pixelsPerTask, handler);
    } else if ([filterName isEqualToString:@"Blur"]){
        self.filter = new BlurFilter(_img, _taskQueue, _pixelsPerTask, handler);
    } else {
        return NO;
    }
    

    self.filter->apply();
    return YES;
}

#pragma mark -

- (void)startNonComputationalTaskWithSleepTime:(NSTimeInterval)time taskCount:(NSInteger)count {
    TaskGroup *group = new TaskGroup();
    group->setCompletionHandler(^{
        delete group;
        
        stop_time();
        double dt = get_time_diff();
        printf("%.4lf\n", dt);
        exit(0);    // end
    });
    for (int i = 0; i < count; i++) {
        TaskRef task(new BlockTask(^{
            [NSThread sleepForTimeInterval:time]; 
        }));
        group->addTask(task);
    }
    init_time();
    _taskQueue->addTaskGroup(group);
}

@end

static struct timeval start_time;
static struct timeval end_time;

void init_time() {
    gettimeofday(&start_time, NULL);
}

void stop_time() {
    gettimeofday(&end_time, NULL);
}

double get_time_diff() {
    return (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
}