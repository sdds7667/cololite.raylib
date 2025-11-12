#import <Cocoa/Cocoa.h>
#include <screensize.hh>



Vector2 MacOS::get_screen_size() {
    Vector2 result;
    @autoreleasepool {
        NSScreen* screen = [NSScreen mainScreen];
        NSRect visibleFrame = [screen visibleFrame];
        result.x = visibleFrame.size.width;
        result.y = visibleFrame.size.height;
    }
    return result;
}
