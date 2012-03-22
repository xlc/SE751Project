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
@property (weak) IBOutlet NSImageView *_imgView;

- (IBAction)openImage:(id)sender;
- (IBAction)selectImage:(id)sender;
- (IBAction)toggleDrawer:(id)sender;
- (IBAction)gcdGrayscaleApply:(id)sender;
- (IBAction)tpGrayscaleApply:(id)sender;


@end
