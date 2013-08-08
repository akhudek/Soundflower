//
//  VolumeView.m
//  Soundflowerbed
//
//  Created by koji on 2013/06/01.
//
//

#import "VolumeView.h"

@implementation VolumeView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

- (NSSlider *)slider
{
    return mSlider;
}

- (NSTextField *)label
{
    return mLabel;
}

- (void)setEnabled:(BOOL)enabled
{
    [mSlider setEnabled:enabled];
    [mLabel setEnabled:enabled];
    if (!enabled){
        [mSlider setFloatValue:0.0f];
        [mLabel setStringValue:@""];
    }
}

- (void)setScalar:(float)scalar
{
    [mSlider setFloatValue:scalar];
}
- (void)setDB:(float)db
{
    NSString *str = [NSString stringWithFormat:@"%3.2f[dB]",db];
    [mLabel setStringValue:str];
    //[str release];
}

@end
