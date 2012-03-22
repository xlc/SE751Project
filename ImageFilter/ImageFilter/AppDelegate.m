//
//  AppDelegate.m
//  ImageFilter
//
//  Created by Xiliang Chen on 12-3-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

- (void)handleImage:(NSString *)imagePath;

@end

@implementation AppDelegate

@synthesize window = _window;
@synthesize drawer = _drawer;
@synthesize imagePathField = _imagePathField;
@synthesize _imgView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [_drawer open];
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

- (IBAction)gcdGrayscaleApply:(id)sender {
    
}

- (IBAction)tpGrayscaleApply:(id)sender {
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
