
/**
 * OpenEmulator
 * Mac OS X Canvas Window Controller
 * (C) 2009-2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls a canvas window.
 */

#import "CanvasWindowController.h"

@implementation CanvasWindowController

- (id)initWithTitle:(NSString *)theTitle
			 canvas:(void *)theCanvas
{
	if (self = [self initWithWindowNibName:@"Canvas"])
	{
		title = [theTitle copy];
		
		canvas = theCanvas;
	}
	
	return self;
}

- (void)dealloc
{
	[title release];
	
	[super dealloc];
}

- (void *)canvas
{
	return canvas;
}

- (void)windowDidLoad
{
	NSToolbar *toolbar;
	toolbar = [[NSToolbar alloc] initWithIdentifier:@"Canvas Toolbar"];
	[toolbar setDelegate:self];
	[toolbar setAllowsUserCustomization:YES];
	[toolbar setAutosavesConfiguration:YES];
	[[self window] setToolbar:toolbar];
	[toolbar release];
	
	NSRect windowFrame = [[self window] frame];
	[[self window] setContentSize:[fCanvasView canvasSize]];
	[[self window] setFrameTopLeftPoint:NSMakePoint(NSMinX(windowFrame),
													NSMaxY(windowFrame))];
}

- (NSString *)windowTitleForDocumentDisplayName:(NSString *)displayName
{
	return [NSString stringWithFormat:@"%@ - %@", displayName, title];
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar
	 itemForItemIdentifier:(NSString *)ident
 willBeInsertedIntoToolbar:(BOOL)flag
{
	NSToolbarItem *item;
	item = [[NSToolbarItem alloc] initWithItemIdentifier:ident];
	if (!item)
		return nil;
	
	if ([ident isEqualToString:@"Power Down"])
	{
		[item setLabel:NSLocalizedString(@"Power Down",
										 @"Canvas Toolbar Label.")];
		[item setPaletteLabel:NSLocalizedString(@"Power Down",
												@"Canvas Toolbar Palette Label.")];
		[item setToolTip:NSLocalizedString(@"Initiate power-down.",
										   @"Canvas Toolbar Tool Tip.")];
		[item setImage:[NSImage imageNamed:@"IconPowerDown.png"]];
		[item setAction:@selector(systemPowerDown:)];
	}
	else if ([ident isEqualToString:@"Sleep"])
	{
		[item setLabel:NSLocalizedString(@"Sleep",
										 @"Canvas Toolbar Label.")];
		[item setPaletteLabel:NSLocalizedString(@"Sleep",
												@"Canvas Toolbar Palette Label.")];
		[item setToolTip:NSLocalizedString(@"Initiate low power mode.",
										   @"Canvas Toolbar Tool Tip.")];
		[item setImage:[NSImage imageNamed:@"IconSleep.png"]];
		[item setAction:@selector(systemSleep:)];
	}
	else if ([ident isEqualToString:@"Wake Up"])
	{
		[item setLabel:NSLocalizedString(@"Wake Up",
										 @"Canvas Toolbar Label.")];
		[item setPaletteLabel:NSLocalizedString(@"Wake Up",
												@"Canvas Toolbar Palette Label.")];
		[item setToolTip:NSLocalizedString(@"Initiate full power state.",
										   @"Canvas Toolbar Tool Tip.")];
		[item setImage:[NSImage imageNamed:@"IconWakeUp.png"]];
		[item setAction:@selector(systemWakeUp:)];
	}
	else if ([ident isEqualToString:@"Cold Restart"])
	{
		[item setLabel:NSLocalizedString(@"Cold Restart",
										 @"Canvas Toolbar Label.")];
		[item setPaletteLabel:NSLocalizedString(@"Cold Restart",
												@"Canvas Toolbar Palette Label.")];
		[item setToolTip:NSLocalizedString(@"Restart the system at the most "
										   "primitive level.",
										   @"Canvas Toolbar Tool Tip.")];
		[item setImage:[NSImage imageNamed:@"IconColdRestart.png"]];
		[item setAction:@selector(systemColdRestart:)];
	}
	else if ([ident isEqualToString:@"Warm Restart"])
	{
		[item setLabel:NSLocalizedString(@"Warm Restart",
										 @"Canvas Toolbar Label.")];
		[item setPaletteLabel:NSLocalizedString(@"Warm Restart",
												@"Canvas Toolbar Palette Label.")];
		[item setToolTip:NSLocalizedString(@"Restart the operating system.",
										   @"Canvas Toolbar Tool Tip.")];
		[item setImage:[NSImage imageNamed:@"IconWarmRestart.png"]];
		[item setAction:@selector(systemWarmRestart:)];
	}
	else if ([ident isEqualToString:@"Debugger Break"])
	{
		[item setLabel:NSLocalizedString(@"Debugger Break",
										 @"Canvas Toolbar Label.")];
		[item setPaletteLabel:NSLocalizedString(@"Debugger Break",
												@"Canvas Toolbar Palette Label.")];
		[item setToolTip:NSLocalizedString(@"Break into the operating system debugger.",
										   @"Canvas Toolbar Tool Tip.")];
		[item setImage:[NSImage imageNamed:@"IconDebuggerBreak.png"]];
		[item setAction:@selector(systemDebuggerBreak:)];
	}
	else if ([ident isEqualToString:@"AudioControls"])
	{
		[item setLabel:NSLocalizedString(@"Audio Controls",
										 @"Canvas Toolbar Label.")];
		[item setPaletteLabel:NSLocalizedString(@"Audio Controls",
												@"Canvas Toolbar Palette Label.")];
		[item setToolTip:NSLocalizedString(@"Show or hide audio controls.",
										   @"Canvas Toolbar Tool Tip.")];
		[item setImage:[NSImage imageNamed:@"IconAudio.png"]];
		[item setAction:@selector(toggleAudioControls:)];
	}
	else if ([ident isEqualToString:@"Devices"])
	{
		[item setLabel:NSLocalizedString(@"Devices",
										 @"Canvas Toolbar Label.")];
		[item setPaletteLabel:NSLocalizedString(@"Devices",
												@"Canvas Toolbar Palette Label.")];
		[item setToolTip:NSLocalizedString(@"Show devices.",
										   @"Canvas Toolbar Tool Tip.")];
		[item setImage:[NSImage imageNamed:@"IconDevices.png"]];
		[item setAction:@selector(showEmulation:)];
	}
	[item autorelease];
	
	return item;
}

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar *)toolbar
{
	return [NSArray arrayWithObjects:
			@"Power Down",
			@"Sleep",
			@"Wake Up",
			NSToolbarFlexibleSpaceItemIdentifier,
			@"AudioControls",
			@"Devices",
			nil];
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar
{
	return [NSArray arrayWithObjects:
			@"Power Down",
			@"Sleep",
			@"Wake Up",
			@"Cold Restart",
			@"Warm Restart",
			@"Debugger Break",
			@"AudioControls",
			@"Devices",
			NSToolbarSeparatorItemIdentifier,
			NSToolbarSpaceItemIdentifier,
			NSToolbarFlexibleSpaceItemIdentifier,
			nil];
}

@end