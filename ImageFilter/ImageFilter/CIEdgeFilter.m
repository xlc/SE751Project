//
//  CIBlurFilter.m
//  ImageFilter
//
//  Created by Xiliang Chen on 12-4-15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "CIEdgeFilter.h"

@implementation CIEdgeFilter

static CIKernel *blueKernel = nil;

- (id)init
{
    if(blueKernel == nil)
    {
        NSBundle    *bundle = [NSBundle bundleForClass: [self class]];
        NSString    *str = [NSString stringWithContentsOfFile:[bundle pathForResource:@"edge" ofType:@"cikernel"] encoding:NSUTF8StringEncoding error:NULL];
                
        NSArray     *kernels = [CIKernel kernelsWithString: str];
        blueKernel = [kernels objectAtIndex:0];
    }
    
    return [super init];
}

- (CIImage *)outputImage
{
    CISampler *src = [CISampler samplerWithImage: inputImage];
    
    return [self apply: blueKernel, src, kCIApplyOptionDefinition, [src definition], nil];
}

@end
