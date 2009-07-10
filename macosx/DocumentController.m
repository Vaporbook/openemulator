
/**
 * OpenEmulator
 * Mac OS X Document Controller
 * (C) 2009 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Manages documents.
 */

#import <Carbon/Carbon.h>

#import "Document.h"
#import "DocumentController.h"
#import "NewDocumentWindowController.h"

static int portAudioCallback(const void *inputBuffer, void *outputBuffer,
							 unsigned long framesPerBuffer,
							 const PaStreamCallbackTimeInfo* timeInfo,
							 PaStreamCallbackFlags statusFlags,
							 void *userData)
{
	return paContinue;
	
	float *in = (float *)inputBuffer;
	float *out = (float *)outputBuffer;
	unsigned int i;
	
	for(i = 0; i < framesPerBuffer; i++)
	{
		*out++ = (rand() & 0xffff) / 65535.0f / 16;
		*out++ = (rand() & 0xffff) / 65535.0f / 16;
	}
	
	return paContinue;
}

@implementation DocumentController

- (id)init
{
	if (self = [super init])
	{
		fDefaults = [NSUserDefaults standardUserDefaults];
		fileTypes = [NSArray arrayWithObjects:
					 @"emulation",
					 @"wav", @"aiff", @"aif",
					 @"dsk", @"do", @"d13", @"po", @"img", @"cpm", @"nib", @"v2d",
					 @"vdsk",
					 @"2mg", @"2img", @"hdv", @"sdk",
					 @"shk", @"img",
					 @"fdi",
					 nil];
		[fileTypes retain];
		
		disableMenuBarCount = 0;
		
		if (Pa_Initialize() != paNoError)
			return self;
		
		if (Pa_OpenDefaultStream(&portAudioStream, 2, 2, paFloat32,
								 48000, 512, portAudioCallback, self) != paNoError)
			return self;
		
		if (Pa_StartStream(portAudioStream) != paNoError)
			return self;
	}
	
	return self;
}

- (void)dealloc
{
	[fileTypes release];
	[super dealloc];
}

- (BOOL)application:(NSApplication *)theApplication
		   openFile:(NSString *)filename
{
	NSError *error;
	printf("openFile\n");
	if ([[filename pathExtension] caseInsensitiveCompare:@"emulation"] == NSOrderedSame)
		return NO;

	if (![self currentDocument])
		[self openUntitledDocumentAndDisplay:YES error:&error];
	
	// It is a disk image
	
	// If there is a default template, create a new emulation
	
	// Now mount it
	
	return YES;
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
	printf("applicationWillFinishLaunching\n");
	if ([fDefaults boolForKey:@"InspectorPanelIsVisible"])
		[fInspectorPanelController toggleInspectorPanel:self];
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
	printf("applicationDidFinishLaunching\n");
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
	NSWindow *window;
	
	Pa_Terminate();
	
	window = [fInspectorPanelController window];
	[fDefaults setInteger:[window isVisible] forKey:@"InspectorPanelIsVisible"];
}

- (BOOL)validateUserInterfaceItem:(id)item
{
	if ([item action] == @selector(newDocument:))
		return !isNewDocumentWindowOpen;
	
	return YES;
}

- (id)openUntitledDocumentAndDisplay:(BOOL)displayDocument error:(NSError **)outError
{
	BOOL isChooseTemplate = YES;
	if (isChooseTemplate)
	{
		*outError = [NSError errorWithDomain:NSCocoaErrorDomain
										code:NSUserCancelledError userInfo:nil];
		
		if (isNewDocumentWindowOpen)
			return nil;
		
		isNewDocumentWindowOpen = YES;
		
		NewDocumentWindowController *newDocumentWindowController;
		newDocumentWindowController = [[NewDocumentWindowController alloc] init:self];
		
		[newDocumentWindowController showWindow:self];
		
		return nil;
	}
	else
	{
		NSURL *url = [NSURL fileURLWithPath:@"/tmp/test.emulation"];
		return [self openUntitledDocumentFromTemplateURL:url error:outError];
	}
}

- (id)openUntitledDocumentFromTemplateURL:(NSURL *)templateURL error:(NSError **)outError
{
	NSDocument *document = [self makeUntitledDocumentFromTemplateURL:templateURL
															   error:outError];
	if (document)
	{
		[self addDocument:document];
		[document makeWindowControllers];
		[document showWindows];
	}
	
	return document;
}

- (id)makeUntitledDocumentFromTemplateURL:(NSURL *)templateURL error:(NSError **)outError
{
	*outError = [[NSError alloc] init];
	
	return [[Document alloc] initWithTemplateURL:templateURL error:outError];
}

- (void)noteNewDocumentWindowClosed
{
	isNewDocumentWindowOpen = NO;
}

- (void)openDocument:(id)sender
{
	NSOpenPanel *panel = [NSOpenPanel openPanel];
	
	if ([panel runModalForTypes:fileTypes] == NSOKButton)
	{
		NSURL *url = [[panel URLs] lastObject];
		NSError *error;
		
		if (![self application:NSApp openFile:[url path]])
			[self openDocumentWithContentsOfURL:url display:YES error:&error];
	}
}

- (void)disableMenuBar
{
	disableMenuBarCount++;
	
	if (disableMenuBarCount == 1)
		SetSystemUIMode(kUIModeAllHidden, kUIOptionAutoShowMenuBar);
}

- (void)enableMenuBar
{
	disableMenuBarCount--;
	
	if (disableMenuBarCount == 0)
		SetSystemUIMode(kUIModeNormal, 0);
}

@end