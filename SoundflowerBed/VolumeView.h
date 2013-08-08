//
//  VolumeView.h
//  Soundflowerbed
//
//  Created by koji on 2013/06/01.
//
//

#import <Cocoa/Cocoa.h>

@interface VolumeView : NSView{
    IBOutlet NSSlider *mSlider;
    IBOutlet NSTextField *mLabel;
}
- (void)setScalar:(float)scalar;
- (void)setDB:(float)db;
@end
