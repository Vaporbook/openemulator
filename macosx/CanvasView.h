
/**
 * OpenEmulator
 * Mac OS X Canvas View
 * (C) 2010-2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls a canvas view.
 */

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>

#import "Document.h"

#define DEVICE_KEYMAP_SIZE		256
#define DEVICE_MOUSE_BUTTONNUM	8

@interface CanvasView : NSOpenGLView
<NSWindowDelegate, NSTextInputClient>
{
	CVDisplayLinkRef displayLink;
	CGLContextObj cglContextObj;
	CGLPixelFormatObj cglPixelFormatObj;
	
	Document *document;
	void *canvas;
	
	NSUInteger keyMap[DEVICE_KEYMAP_SIZE];
	NSUInteger keyModifierFlags;
	NSUInteger keyboardFlags;
	BOOL capsLockNotSynchronized;
}

- (void)initOpenGL;
- (void)freeOpenGL;
- (void)startDisplayLink;
- (void)stopDisplayLink;
- (NSSize)defaultViewSize;
- (void)updateView;

- (void)setKeyboardFlags:(NSInteger)theKeyboardFlags;
- (void)synchronizeKeyboardFlags;

@end