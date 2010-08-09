
/**
 * libemulation
 * Host
 * (C) 2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls host events
 */

#ifndef _HOST_H
#define _HOST_H

#include "OEComponent.h"

#define HOST_DEVICE "host:host"

#define HOST_HID_KEY_NUM				256
#define HOST_HID_POINTER_BUTTON_NUM		8
#define HOST_HID_MOUSE_BUTTON_NUM		8
#define HOST_HID_JOYSTICK_NUM			4
#define HOST_HID_JOYSTICK_AXIS_NUM		16
#define HOST_HID_JOYSTICK_BUTTON_NUM	16
#define HOST_HID_JOYSTICK_HAT_NUM		4
#define HOST_HID_JOYSTICK_RAXIS_NUM		4
#define HOST_HID_TABLET_BUTTON_NUM		8

// Notifications
enum
{
	HOST_CONFIGURATION_CHANGED,
	HOST_POWERSTATE_CHANGED,
	HOST_POWERED_ON,
	HOST_POWERED_OFF,
	
	HOST_AUDIO_RENDER_WILL_BEGIN,
	HOST_AUDIO_RENDER_DID_BEGIN,
	HOST_AUDIO_RENDER_WILL_END,
	HOST_AUDIO_RENDER_DID_END,
	
	HOST_HID_SYSTEM_CHANGED,
	HOST_HID_KEYBOARD_CHANGED,
	HOST_HID_UNICODEKEYBOARD_CHANGED,
	HOST_HID_POINTER_CHANGED,
	HOST_HID_MOUSE_CHANGED,
	HOST_HID_JOYSTICK1_CHANGED,
	HOST_HID_JOYSTICK2_CHANGED,
	HOST_HID_JOYSTICK3_CHANGED,
	HOST_HID_JOYSTICK4_CHANGED,
	HOST_HID_TABLET_CHANGED,
	
	HOST_CLIPBOARD_WILL_COPY,
	HOST_CLIPBOARD_WILL_PASTE,
};

// Events
enum
{
	HOST_SET_POWERSTATE,
	HOST_GET_POWERSTATE,
	
	HOST_REGISTER_VIDEO_CALLBACK,
	HOST_REGISTER_HID_CALLBACK,
	
	HOST_ADD_SCREEN,
	HOST_REMOVE_SCREEN,
	
	HOST_MOUNT_DISKIMAGE,
	
	HOST_ENUMERATE_DEBUGGERS,
	HOST_SEND_DEBUGGER_COMMAND,
};

// Power states
enum
{
	HOST_POWERSTATE_ON,
	HOST_POWERSTATE_PAUSE,
	HOST_POWERSTATE_STANDBY,
	HOST_POWERSTATE_SLEEP,
	HOST_POWERSTATE_HIBERNATE,
	HOST_POWERSTATE_OFF,
};

// Audio
typedef struct
{
	float sampleRate;
	int channelNum;
	int frameNum;
	
	float *input;
	float *output;
} HostAudioBuffer;

// Video
#define HOST_VIDEO_FRAMEBUFFERNUM 2

typedef struct
{
	int framebufferWidth;
	int framebufferHeight;
	float contentWidth;
	float contentHeight;
	float paddingTop;
	float paddingRight;
	float paddingBottom;
	float paddingLeft;
} HostVideoConfiguration;

typedef struct
{
	HostVideoConfiguration conf;
	int readIndex;
	int writeIndex;
	OEUInt32 *framebuffer[HOST_VIDEO_FRAMEBUFFERNUM];
} HostVideoScreen;

typedef vector<HostVideoScreen *> HostVideoScreens;

// HID
typedef struct
{
	int usageId;
	float value;
} HostHIDEvent;

// HID System Event
enum
{
	HOST_HID_S_POWERDOWN = 0x81,
	HOST_HID_S_SLEEP,
	HOST_HID_S_WAKEUP,
	HOST_HID_S_CONTEXT,
	HOST_HID_S_MAINMENU,
	HOST_HID_S_APPMENU,
	HOST_HID_S_MENUHELP,
	HOST_HID_S_MENUEXIT,
	HOST_HID_S_MENUSELECT,
	HOST_HID_S_MENURIGHT,
	HOST_HID_S_MENULEFT,
	HOST_HID_S_MENUUP,
	HOST_HID_S_MENUDOWN,
	HOST_HID_S_COLDRESTART,
	HOST_HID_S_WARMRESTART,
	HOST_HID_S_DPADUP,
	HOST_HID_S_DPADDOWN,
	HOST_HID_S_DPADRIGHT,
	HOST_HID_S_DPADLEFT,
	
	HOST_HID_S_DOCK = 0xa0,
	HOST_HID_S_UNDOCK,
	HOST_HID_S_SETUP,
	HOST_HID_S_BREAK,
	HOST_HID_S_DEBUGGERBREAK,
	HOST_HID_S_APPLICATIONBREAK,
	HOST_HID_S_APPLICATIONDEBUGGERBREAK,
	HOST_HID_S_SPEAKERMUTE,
	HOST_HID_S_HIBERNATE,
	
	HOST_HID_S_DISPLAYINVERT = 0xb0,
	HOST_HID_S_DISPLAYINTERNAL,
	HOST_HID_S_DISPLAYEXTERNAL,
	HOST_HID_S_DISPLAYBOTH,
	HOST_HID_S_DISPLAYDUAL,
	HOST_HID_S_DISPLAYTOGGLEINTEXT,
	HOST_HID_S_DISPLAYSWAPPRIMARYSECONDARY,
	HOST_HID_S_DISPLAYLCDAUTOSCALE,
};

// HID Keyboard Events
enum {
	HOST_HID_K_A = 0x04,
	HOST_HID_K_B,
	HOST_HID_K_C,
	HOST_HID_K_D,
	HOST_HID_K_E,
	HOST_HID_K_F,
	HOST_HID_K_G,
	HOST_HID_K_H,
	HOST_HID_K_I,
	HOST_HID_K_J,
	HOST_HID_K_K,
	HOST_HID_K_L,
	HOST_HID_K_M,
	HOST_HID_K_N,
	HOST_HID_K_O,
	HOST_HID_K_P,
	HOST_HID_K_Q,
	HOST_HID_K_R,
	HOST_HID_K_S,
	HOST_HID_K_T,
	HOST_HID_K_U,
	HOST_HID_K_V,
	HOST_HID_K_W,
	HOST_HID_K_X,
	HOST_HID_K_Y,
	HOST_HID_K_Z,
	HOST_HID_K_1,
	HOST_HID_K_2,
	HOST_HID_K_3,
	HOST_HID_K_4,
	HOST_HID_K_5,
	HOST_HID_K_6,
	HOST_HID_K_7,
	HOST_HID_K_8,
	HOST_HID_K_9,
	HOST_HID_K_0,
	HOST_HID_K_ENTER,
	HOST_HID_K_ESCAPE,
	HOST_HID_K_BACKSPACE,
	HOST_HID_K_TAB,
	HOST_HID_K_SPACE,
	HOST_HID_K_MINUS,
	HOST_HID_K_EQUAL,
	HOST_HID_K_LEFTBRACKET,
	HOST_HID_K_RIGHTBRACKET,
	HOST_HID_K_BACKSLASH,
	HOST_HID_K_NON_US1,
	HOST_HID_K_SEMICOLON,
	HOST_HID_K_QUOTE,
	HOST_HID_K_GRAVEACCENT,
	HOST_HID_K_COMMA,
	HOST_HID_K_PERIOD,
	HOST_HID_K_SLASH,
	HOST_HID_K_CAPSLOCK,
	HOST_HID_K_F1,
	HOST_HID_K_F2,
	HOST_HID_K_F3,
	HOST_HID_K_F4,
	HOST_HID_K_F5,
	HOST_HID_K_F6,
	HOST_HID_K_F7,
	HOST_HID_K_F8,
	HOST_HID_K_F9,
	HOST_HID_K_F10,
	HOST_HID_K_F11,
	HOST_HID_K_F12,
	HOST_HID_K_PRINTSCREEN,
	HOST_HID_K_SCROLLLOCK,
	HOST_HID_K_PAUSE,
	HOST_HID_K_INSERT,
	HOST_HID_K_HOME,
	HOST_HID_K_PAGEUP,
	HOST_HID_K_DELETE,
	HOST_HID_K_END,
	HOST_HID_K_PAGEDOWN,
	HOST_HID_K_RIGHT,
	HOST_HID_K_LEFT,
	HOST_HID_K_DOWN,
	HOST_HID_K_UP,
	HOST_HID_KP_NUMLOCK,
	HOST_HID_KP_SLASH,
	HOST_HID_KP_STAR,
	HOST_HID_KP_MINUS,
	HOST_HID_KP_PLUS,
	HOST_HID_KP_ENTER,
	HOST_HID_KP_1,
	HOST_HID_KP_2,
	HOST_HID_KP_3,
	HOST_HID_KP_4,
	HOST_HID_KP_5,
	HOST_HID_KP_6,
	HOST_HID_KP_7,
	HOST_HID_KP_8,
	HOST_HID_KP_9,
	HOST_HID_KP_0,
	HOST_HID_KP_PERIOD,
	HOST_HID_K_NON_US2,
	HOST_HID_K_APPLICATION,
	HOST_HID_K_POWER,
	HOST_HID_KP_EQUAL,
	HOST_HID_K_F13,
	HOST_HID_K_F14,
	HOST_HID_K_F15,
	HOST_HID_K_F16,
	HOST_HID_K_F17,
	HOST_HID_K_F18,
	HOST_HID_K_F19,
	HOST_HID_K_F20,
	HOST_HID_K_F21,
	HOST_HID_K_F22,
	HOST_HID_K_F23,
	HOST_HID_K_F24,
	HOST_HID_K_EXECUTE,
	HOST_HID_K_HELP,
	HOST_HID_K_MENU,
	HOST_HID_K_SELECT,
	HOST_HID_K_STOP,
	HOST_HID_K_AGAIN,
	HOST_HID_K_UNDO,
	HOST_HID_K_CUT,
	HOST_HID_K_COPY,
	HOST_HID_K_PASTE,
	HOST_HID_K_FIND,
	HOST_HID_K_MUTE,
	HOST_HID_K_VOLUMEUP,
	HOST_HID_K_VOLUMEDOWN,
	HOST_HID_K_LOCKINGCAPSLOCK,
	HOST_HID_K_LOCKINGNUMLOCK,
	HOST_HID_K_LOCKINGSCROLLLOCK,
	HOST_HID_KP_COMMA,
	HOST_HID_KP_EQUAL2,
	HOST_HID_K_INTERNATIONAL1,
	HOST_HID_K_INTERNATIONAL2,
	HOST_HID_K_INTERNATIONAL3,
	HOST_HID_K_INTERNATIONAL4,
	HOST_HID_K_INTERNATIONAL5,
	HOST_HID_K_INTERNATIONAL6,
	HOST_HID_K_INTERNATIONAL7,
	HOST_HID_K_INTERNATIONAL8,
	HOST_HID_K_INTERNATIONAL9,
	HOST_HID_K_LANG1,
	HOST_HID_K_LANG2,
	HOST_HID_K_LANG3,
	HOST_HID_K_LANG4,
	HOST_HID_K_LANG5,
	HOST_HID_K_LANG6,
	HOST_HID_K_LANG7,
	HOST_HID_K_LANG8,
	HOST_HID_K_LANG9,
	HOST_HID_K_ALTERASE,
	HOST_HID_K_SYSREQ,
	HOST_HID_K_CANCEL,
	HOST_HID_K_CLEAR,
	HOST_HID_K_PRIOR,
	HOST_HID_K_RETURN,
	HOST_HID_K_SEPARATOR,
	HOST_HID_K_OUT,
	HOST_HID_K_OPER,
	HOST_HID_K_CLEARAGAIN,
	HOST_HID_K_CRSELPROPS,
	HOST_HID_K_EXSEL,
	
	HOST_HID_KP_00 = 0xb0,
	HOST_HID_KP_000,
	HOST_HID_KP_THOUSANDSSEPARATOR,
	HOST_HID_KP_DECIMALSEPARATOR,
	HOST_HID_KP_CURRENCYUNIT,
	HOST_HID_KP_CURRENCYSUBUNIT,
	HOST_HID_KP_LEFTPARENTHESIS,
	HOST_HID_KP_RIGHTPARENTHESIS,
	HOST_HID_KP_LEFTCURLYBRACKET,
	HOST_HID_KP_RIGHTCURLYBRACKET,
	HOST_HID_KP_TAB,
	HOST_HID_KP_BACKSPACE,
	HOST_HID_KP_A,
	HOST_HID_KP_B,
	HOST_HID_KP_C,
	HOST_HID_KP_D,
	HOST_HID_KP_E,
	HOST_HID_KP_F,
	HOST_HID_KP_XOR,
	HOST_HID_KP_CARET,
	HOST_HID_KP_PERCENT,
	HOST_HID_KP_LESS,
	HOST_HID_KP_MORE,
	HOST_HID_KP_AND,
	HOST_HID_KP_LOGICAND,
	HOST_HID_KP_OR,
	HOST_HID_KP_LOGICOR,
	HOST_HID_KP_COLON,
	HOST_HID_KP_NUMERAL,
	HOST_HID_KP_SPACE,
	HOST_HID_KP_AT,
	HOST_HID_KP_EXCLAMATION,
	HOST_HID_KP_MEMSTORE,
	HOST_HID_KP_MEMRECALL,
	HOST_HID_KP_MEMCLEAR,
	HOST_HID_KP_MEMADD,
	HOST_HID_KP_MEMSUBTRACT,
	HOST_HID_KP_MEMMULTIPLY,
	HOST_HID_KP_MEMDIVIDE,
	HOST_HID_KP_PLUSMINUS,
	HOST_HID_KP_CLEAR,
	HOST_HID_KP_CLEARENTRY,
	HOST_HID_KP_BINARY,
	HOST_HID_KP_OCTAL,
	HOST_HID_KP_DECIMAL,
	HOST_HID_KP_HEXADECIMAL,
	
	HOST_HID_K_LEFTCONTROL = 0xe0,
	HOST_HID_K_LEFTSHIFT,
	HOST_HID_K_LEFTALT,
	HOST_HID_K_LEFTGUI,
	HOST_HID_K_RIGHTCONTROL,
	HOST_HID_K_RIGHTSHIFT,
	HOST_HID_K_RIGHTALT,
	HOST_HID_K_RIGHTGUI,
};

// HID LED Events
#define HOST_HID_L_NUMLOCK		(1 << 0)
#define HOST_HID_L_CAPSLOCK		(1 << 1)
#define HOST_HID_L_SCROLLLOCK	(1 << 2)
#define HOST_HID_L_COMPOSE		(1 << 3)
#define HOST_HID_L_KANA			(1 << 4)
#define HOST_HID_L_POWER		(1 << 5)
#define HOST_HID_L_SHIFT		(1 << 6)

// HID Unicode key modifier
#define HOST_HID_U_CONTROL		(1 << 0)
#define HOST_HID_U_SHIFT		(1 << 1)
#define HOST_HID_U_ALT			(1 << 2)
#define HOST_HID_U_GUI			(1 << 3)
#define HOST_HID_U_CAPSLOCK		(1 << 4)
#define HOST_HID_U_ANYKEY		(1 << 5)
#define HOST_HID_U_KEYPAD		(1 << 6)

// HID Pointer Events
enum
{
	HOST_HID_P_X,
	HOST_HID_P_Y,
	HOST_HID_P_BUTTON1,
	HOST_HID_P_BUTTON2,
	HOST_HID_P_BUTTON3,
	HOST_HID_P_BUTTON4,
	HOST_HID_P_BUTTON5,
	HOST_HID_P_BUTTON6,
	HOST_HID_P_BUTTON7,
	HOST_HID_P_BUTTON8,
	HOST_HID_P_WX,
	HOST_HID_P_WY,
};

// HID Mouse Events
enum
{
	HOST_HID_M_RX,
	HOST_HID_M_RY,
	HOST_HID_M_BUTTON1,
	HOST_HID_M_BUTTON2,
	HOST_HID_M_BUTTON3,
	HOST_HID_M_BUTTON4,
	HOST_HID_M_BUTTON5,
	HOST_HID_M_BUTTON6,
	HOST_HID_M_BUTTON7,
	HOST_HID_M_BUTTON8,
	HOST_HID_M_WX,
	HOST_HID_M_WY,
};

// HID Joystick Events
//
// Axis range is -1.0 .. 1.0
enum
{
	HOST_HID_J_AXIS1,
	HOST_HID_J_AXIS2,
	HOST_HID_J_AXIS3,
	HOST_HID_J_AXIS4,
	HOST_HID_J_AXIS5,
	HOST_HID_J_AXIS6,
	HOST_HID_J_AXIS7,
	HOST_HID_J_AXIS8,
	HOST_HID_J_AXIS9,
	HOST_HID_J_AXIS10,
	HOST_HID_J_AXIS11,
	HOST_HID_J_AXIS12,
	HOST_HID_J_AXIS13,
	HOST_HID_J_AXIS14,
	HOST_HID_J_AXIS15,
	HOST_HID_J_AXIS16,
	HOST_HID_J_BUTTON1,
	HOST_HID_J_BUTTON2,
	HOST_HID_J_BUTTON3,
	HOST_HID_J_BUTTON4,
	HOST_HID_J_BUTTON5,
	HOST_HID_J_BUTTON6,
	HOST_HID_J_BUTTON7,
	HOST_HID_J_BUTTON8,
	HOST_HID_J_BUTTON9,
	HOST_HID_J_BUTTON10,
	HOST_HID_J_BUTTON11,
	HOST_HID_J_BUTTON12,
	HOST_HID_J_BUTTON13,
	HOST_HID_J_BUTTON14,
	HOST_HID_J_BUTTON15,
	HOST_HID_J_BUTTON16,
	HOST_HID_J_HAT1,
	HOST_HID_J_HAT2,
	HOST_HID_J_HAT3,
	HOST_HID_J_HAT4,
	HOST_HID_J_RAXIS1,
	HOST_HID_J_RAXIS2,
	HOST_HID_J_RAXIS3,
	HOST_HID_J_RAXIS4,
};

// HID Tablet Events
enum
{
	HOST_HID_T_X,
	HOST_HID_T_Y,
	HOST_HID_T_BUTTON1,
	HOST_HID_T_BUTTON2,
	HOST_HID_T_BUTTON3,
	HOST_HID_T_BUTTON4,
	HOST_HID_T_BUTTON5,
	HOST_HID_T_BUTTON6,
	HOST_HID_T_BUTTON7,
	HOST_HID_T_BUTTON8,
	HOST_HID_T_PROXIMITY,
};

class Host : public OEComponent
{
public:
	bool setProperty(const string &name, const string &value);
	bool getProperty(const string &name, string &value);
	
	bool postEvent(OEComponent *component, int event, void *data);
	
private:
	string notes;
	
	int powerState;
	
	string videoWindow;
	bool videoUpdated;
	HostVideoScreens videoScreens;
	
	bool hidMouseCapture;
	int hidKeyboardLEDs;
	
	void setPowerState(int powerState);
	
	bool addScreen(HostVideoScreen *screen);
	bool removeScreen(HostVideoScreen *screen);
};

#endif
