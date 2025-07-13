#include "input_simulator.h"
#include <iostream>

#ifdef __linux__
#include <unistd.h>
#include <cstring>
#endif

InputSimulator::InputSimulator() 
#ifdef __linux__
    : display_(nullptr)
#endif
{
}

InputSimulator::~InputSimulator() {
    shutdown();
}

bool InputSimulator::initialize() {
#ifdef _WIN32
    return true;
#elif __linux__
    display_ = XOpenDisplay(nullptr);
    if (!display_) {
        std::cerr << "Cannot open X11 display" << std::endl;
        return false;
    }
    
    int event_base, error_base, major, minor;
    if (!XTestQueryExtension(display_, &event_base, &error_base, &major, &minor)) {
        std::cerr << "XTest extension not available" << std::endl;
        XCloseDisplay(display_);
        display_ = nullptr;
        return false;
    }
    
    return true;
#elif __APPLE__
    return true;
#else
    return false;
#endif
}

void InputSimulator::shutdown() {
#ifdef __linux__
    if (display_) {
        XCloseDisplay(display_);
        display_ = nullptr;
    }
#endif
}

void InputSimulator::moveMouse(int delta_x, int delta_y) {
#ifdef _WIN32
    POINT cursor;
    GetCursorPos(&cursor);
    SetCursorPos(cursor.x + delta_x, cursor.y + delta_y);
#elif __linux__
    if (display_) {
        XTestFakeRelativeMotionEvent(display_, delta_x, delta_y, CurrentTime);
        XFlush(display_);
    }
#elif __APPLE__
    CGPoint cursor = CGEventGetLocation(CGEventCreate(NULL));
    CGWarpMouseCursorPosition(CGPointMake(cursor.x + delta_x, cursor.y + delta_y));
#endif
}

void InputSimulator::setMousePosition(int x, int y) {
#ifdef _WIN32
    SetCursorPos(x, y);
#elif __linux__
    if (display_) {
        XTestFakeMotionEvent(display_, DefaultScreen(display_), x, y, CurrentTime);
        XFlush(display_);
    }
#elif __APPLE__
    CGWarpMouseCursorPosition(CGPointMake(x, y));
#endif
}

void InputSimulator::leftClick() {
#ifdef _WIN32
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#elif __linux__
    simulateMouseClick(Button1, true);
    simulateMouseClick(Button1, false);
#elif __APPLE__
    simulateMouseClick(kCGMouseButtonLeft, true);
    simulateMouseClick(kCGMouseButtonLeft, false);
#endif
}

void InputSimulator::leftMouseDown() {
#ifdef _WIN32
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
#elif __linux__
    simulateMouseClick(Button1, true);
#elif __APPLE__
    simulateMouseClick(kCGMouseButtonLeft, true);
#endif
}

void InputSimulator::leftMouseUp() {
#ifdef _WIN32
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#elif __linux__
    simulateMouseClick(Button1, false);
#elif __APPLE__
    simulateMouseClick(kCGMouseButtonLeft, false);
#endif
}

void InputSimulator::rightClick() {
#ifdef _WIN32
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
#elif __linux__
    simulateMouseClick(Button3, true);
    simulateMouseClick(Button3, false);
#elif __APPLE__
    simulateMouseClick(kCGMouseButtonRight, true);
    simulateMouseClick(kCGMouseButtonRight, false);
#endif
}

void InputSimulator::rightMouseDown() {
#ifdef _WIN32
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
#elif __linux__
    simulateMouseClick(Button3, true);
#elif __APPLE__
    simulateMouseClick(kCGMouseButtonRight, true);
#endif
}

void InputSimulator::rightMouseUp() {
#ifdef _WIN32
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
#elif __linux__
    simulateMouseClick(Button3, false);
#elif __APPLE__
    simulateMouseClick(kCGMouseButtonRight, false);
#endif
}

void InputSimulator::middleClick() {
#ifdef _WIN32
    mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
#elif __linux__
    simulateMouseClick(Button2, true);
    simulateMouseClick(Button2, false);
#elif __APPLE__
    simulateMouseClick(kCGMouseButtonCenter, true);
    simulateMouseClick(kCGMouseButtonCenter, false);
#endif
}

void InputSimulator::scroll(int delta) {
#ifdef _WIN32
    mouse_event(MOUSEEVENTF_WHEEL, 0, 0, delta * WHEEL_DELTA, 0);
#elif __linux__
    if (display_) {
        int button = (delta > 0) ? Button4 : Button5;
        XTestFakeButtonEvent(display_, button, True, CurrentTime);
        XTestFakeButtonEvent(display_, button, False, CurrentTime);
        XFlush(display_);
    }
#elif __APPLE__
    CGEventRef scroll_event = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 1, delta * 10);
    CGEventPost(kCGHIDEventTap, scroll_event);
    CFRelease(scroll_event);
#endif
}

void InputSimulator::pressKey(int key_code) {
#ifdef _WIN32
    simulateKeyPress(key_code, true);
#elif __linux__
    simulateKeyPress(key_code, true);
#elif __APPLE__
    simulateKeyPress(key_code, true);
#endif
}

void InputSimulator::releaseKey(int key_code) {
#ifdef _WIN32
    simulateKeyPress(key_code, false);
#elif __linux__
    simulateKeyPress(key_code, false);
#elif __APPLE__
    simulateKeyPress(key_code, false);
#endif
}

void InputSimulator::typeKey(int key_code) {
    pressKey(key_code);
    releaseKey(key_code);
}

void InputSimulator::typeText(const char* text) {
#ifdef _WIN32
    // Convert to wide string and send as unicode
    int len = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    wchar_t* wide_text = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, text, -1, wide_text, len);
    
    for (int i = 0; i < len - 1; ++i) {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = 0;
        input.ki.wScan = wide_text[i];
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1, &input, sizeof(INPUT));
        
        input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
    
    delete[] wide_text;
#elif __linux__
    // For simplicity, just print to console on Linux
    std::cout << "Text input: " << text << std::endl;
#elif __APPLE__
    // For simplicity, just print to console on macOS
    std::cout << "Text input: " << text << std::endl;
#endif
}

void InputSimulator::triggerVoiceInput() {
#ifdef _WIN32
    // Win + H
    keybd_event(VK_LWIN, 0, 0, 0);
    keybd_event('H', 0, 0, 0);
    keybd_event('H', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    // Linux implementation would depend on the desktop environment
    std::cout << "Voice input triggered (Linux not implemented yet)" << std::endl;
#elif __APPLE__
    // macOS voice input trigger
    std::cout << "Voice input triggered (macOS not implemented yet)" << std::endl;
#endif
}

void InputSimulator::altTab() {
#ifdef _WIN32
    // Alt + Tab
    keybd_event(VK_MENU, 0, 0, 0);  // Alt down
    keybd_event(VK_TAB, 0, 0, 0);   // Tab down
    keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);  // Tab up
    keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0); // Alt up
#elif __linux__
    if (display_) {
        KeyCode alt = XKeysymToKeycode(display_, XK_Alt_L);
        KeyCode tab = XKeysymToKeycode(display_, XK_Tab);
        
        XTestFakeKeyEvent(display_, alt, True, CurrentTime);
        XTestFakeKeyEvent(display_, tab, True, CurrentTime);
        XTestFakeKeyEvent(display_, tab, False, CurrentTime);
        XTestFakeKeyEvent(display_, alt, False, CurrentTime);
        XFlush(display_);
    }
#elif __APPLE__
    // macOS Cmd+Tab
    CGEventRef cmd_down = CGEventCreateKeyboardEvent(NULL, kVK_Command, true);
    CGEventRef tab_down = CGEventCreateKeyboardEvent(NULL, kVK_Tab, true);
    CGEventRef tab_up = CGEventCreateKeyboardEvent(NULL, kVK_Tab, false);
    CGEventRef cmd_up = CGEventCreateKeyboardEvent(NULL, kVK_Command, false);
    
    CGEventPost(kCGHIDEventTap, cmd_down);
    CGEventPost(kCGHIDEventTap, tab_down);
    CGEventPost(kCGHIDEventTap, tab_up);
    CGEventPost(kCGHIDEventTap, cmd_up);
    
    CFRelease(cmd_down);
    CFRelease(tab_down);
    CFRelease(tab_up);
    CFRelease(cmd_up);
#endif
}

void InputSimulator::winTab() {
#ifdef _WIN32
    // Win + Tab (Task View)
    keybd_event(VK_LWIN, 0, 0, 0);  // Win down
    keybd_event(VK_TAB, 0, 0, 0);   // Tab down
    keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);  // Tab up
    keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0); // Win up
#elif __linux__
    // Linux: Super+Tab or similar depending on desktop environment
    if (display_) {
        KeyCode super = XKeysymToKeycode(display_, XK_Super_L);
        KeyCode tab = XKeysymToKeycode(display_, XK_Tab);
        
        XTestFakeKeyEvent(display_, super, True, CurrentTime);
        XTestFakeKeyEvent(display_, tab, True, CurrentTime);
        XTestFakeKeyEvent(display_, tab, False, CurrentTime);
        XTestFakeKeyEvent(display_, super, False, CurrentTime);
        XFlush(display_);
    }
#elif __APPLE__
    // macOS Mission Control (Ctrl+Up)
    CGEventRef ctrl_down = CGEventCreateKeyboardEvent(NULL, kVK_Control, true);
    CGEventRef up_down = CGEventCreateKeyboardEvent(NULL, kVK_UpArrow, true);
    CGEventRef up_up = CGEventCreateKeyboardEvent(NULL, kVK_UpArrow, false);
    CGEventRef ctrl_up = CGEventCreateKeyboardEvent(NULL, kVK_Control, false);
    
    CGEventPost(kCGHIDEventTap, ctrl_down);
    CGEventPost(kCGHIDEventTap, up_down);
    CGEventPost(kCGHIDEventTap, up_up);
    CGEventPost(kCGHIDEventTap, ctrl_up);
    
    CFRelease(ctrl_down);
    CFRelease(up_down);
    CFRelease(up_up);
    CFRelease(ctrl_up);
#endif
}

void InputSimulator::escape() {
#ifdef _WIN32
    keybd_event(VK_ESCAPE, 0, 0, 0);
    keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    if (display_) {
        KeyCode esc = XKeysymToKeycode(display_, XK_Escape);
        XTestFakeKeyEvent(display_, esc, True, CurrentTime);
        XTestFakeKeyEvent(display_, esc, False, CurrentTime);
        XFlush(display_);
    }
#elif __APPLE__
    CGEventRef esc_down = CGEventCreateKeyboardEvent(NULL, kVK_Escape, true);
    CGEventRef esc_up = CGEventCreateKeyboardEvent(NULL, kVK_Escape, false);
    CGEventPost(kCGHIDEventTap, esc_down);
    CGEventPost(kCGHIDEventTap, esc_up);
    CFRelease(esc_down);
    CFRelease(esc_up);
#endif
}

void InputSimulator::enter() {
#ifdef _WIN32
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    if (display_) {
        KeyCode enter = XKeysymToKeycode(display_, XK_Return);
        XTestFakeKeyEvent(display_, enter, True, CurrentTime);
        XTestFakeKeyEvent(display_, enter, False, CurrentTime);
        XFlush(display_);
    }
#elif __APPLE__
    CGEventRef enter_down = CGEventCreateKeyboardEvent(NULL, kVK_Return, true);
    CGEventRef enter_up = CGEventCreateKeyboardEvent(NULL, kVK_Return, false);
    CGEventPost(kCGHIDEventTap, enter_down);
    CGEventPost(kCGHIDEventTap, enter_up);
    CFRelease(enter_down);
    CFRelease(enter_up);
#endif
}

void InputSimulator::winKey() {
#ifdef _WIN32
    keybd_event(VK_LWIN, 0, 0, 0);
    keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    if (display_) {
        KeyCode super = XKeysymToKeycode(display_, XK_Super_L);
        XTestFakeKeyEvent(display_, super, True, CurrentTime);
        XTestFakeKeyEvent(display_, super, False, CurrentTime);
        XFlush(display_);
    }
#elif __APPLE__
    // macOS Cmd key
    CGEventRef cmd_down = CGEventCreateKeyboardEvent(NULL, kVK_Command, true);
    CGEventRef cmd_up = CGEventCreateKeyboardEvent(NULL, kVK_Command, false);
    CGEventPost(kCGHIDEventTap, cmd_down);
    CGEventPost(kCGHIDEventTap, cmd_up);
    CFRelease(cmd_down);
    CFRelease(cmd_up);
#endif
}

void InputSimulator::screenshot() {
#ifdef _WIN32
    // Win + Shift + S
    keybd_event(VK_LWIN, 0, 0, 0);
    keybd_event(VK_LSHIFT, 0, 0, 0);
    keybd_event('S', 0, 0, 0);
    keybd_event('S', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    // Linux: depends on desktop environment, common is Print Screen
    if (display_) {
        KeyCode print = XKeysymToKeycode(display_, XK_Print);
        XTestFakeKeyEvent(display_, print, True, CurrentTime);
        XTestFakeKeyEvent(display_, print, False, CurrentTime);
        XFlush(display_);
    }
#elif __APPLE__
    // macOS Cmd+Shift+4 (area screenshot)
    CGEventRef cmd_down = CGEventCreateKeyboardEvent(NULL, kVK_Command, true);
    CGEventRef shift_down = CGEventCreateKeyboardEvent(NULL, kVK_Shift, true);
    CGEventRef four_down = CGEventCreateKeyboardEvent(NULL, kVK_ANSI_4, true);
    CGEventRef four_up = CGEventCreateKeyboardEvent(NULL, kVK_ANSI_4, false);
    CGEventRef shift_up = CGEventCreateKeyboardEvent(NULL, kVK_Shift, false);
    CGEventRef cmd_up = CGEventCreateKeyboardEvent(NULL, kVK_Command, false);
    
    CGEventPost(kCGHIDEventTap, cmd_down);
    CGEventPost(kCGHIDEventTap, shift_down);
    CGEventPost(kCGHIDEventTap, four_down);
    CGEventPost(kCGHIDEventTap, four_up);
    CGEventPost(kCGHIDEventTap, shift_up);
    CGEventPost(kCGHIDEventTap, cmd_up);
    
    CFRelease(cmd_down);
    CFRelease(shift_down);
    CFRelease(four_down);
    CFRelease(four_up);
    CFRelease(shift_up);
    CFRelease(cmd_up);
#endif
}

void InputSimulator::mediaPlayPause() {
#ifdef _WIN32
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0);
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    (void)system("playerctl play-pause");
#elif __APPLE__
    (void)system("osascript -e 'tell application \"Music\" to playpause'");
#endif
}

void InputSimulator::mediaNext() {
#ifdef _WIN32
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, 0, 0);
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    (void)system("playerctl next");
#elif __APPLE__
    (void)system("osascript -e 'tell application \"Music\" to next track'");
#endif
}

void InputSimulator::mediaPrevious() {
#ifdef _WIN32
    keybd_event(VK_MEDIA_PREV_TRACK, 0, 0, 0);
    keybd_event(VK_MEDIA_PREV_TRACK, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    (void)system("playerctl previous");
#elif __APPLE__
    (void)system("osascript -e 'tell application \"Music\" to previous track'");
#endif
}

void InputSimulator::volumeUp() {
#ifdef _WIN32
    keybd_event(VK_VOLUME_UP, 0, 0, 0);
    keybd_event(VK_VOLUME_UP, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    (void)system("pactl set-sink-volume @DEFAULT_SINK@ +5%");
#elif __APPLE__
    (void)system("osascript -e 'set volume output volume (output volume of (get volume settings) + 10)'");
#endif
}

void InputSimulator::volumeDown() {
#ifdef _WIN32
    keybd_event(VK_VOLUME_DOWN, 0, 0, 0);
    keybd_event(VK_VOLUME_DOWN, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    (void)system("pactl set-sink-volume @DEFAULT_SINK@ -5%");
#elif __APPLE__
    (void)system("osascript -e 'set volume output volume (output volume of (get volume settings) - 10)'");
#endif
}

void InputSimulator::volumeMute() {
#ifdef _WIN32
    keybd_event(VK_VOLUME_MUTE, 0, 0, 0);
    keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    (void)system("pactl set-sink-mute @DEFAULT_SINK@ toggle");
#elif __APPLE__
    (void)system("osascript -e 'set volume with output muted'");
#endif
}

// Private helper methods
#ifdef _WIN32
void InputSimulator::simulateKeyPress(WORD key, bool key_down) {
    DWORD flags = key_down ? 0 : KEYEVENTF_KEYUP;
    keybd_event(key, 0, flags, 0);
}

void InputSimulator::simulateMouseClick(DWORD button, bool button_down) {
    // Windows mouse_event uses separate flags for down and up
    // MOUSEEVENTF_LEFTDOWN = 0x0002, MOUSEEVENTF_LEFTUP = 0x0004
    // MOUSEEVENTF_RIGHTDOWN = 0x0008, MOUSEEVENTF_RIGHTUP = 0x0010
    // MOUSEEVENTF_MIDDLEDOWN = 0x0020, MOUSEEVENTF_MIDDLEUP = 0x0040
    mouse_event(button, 0, 0, 0, 0);
}
#elif __linux__
void InputSimulator::simulateKeyPress(KeyCode key, bool key_down) {
    if (display_) {
        XTestFakeKeyEvent(display_, key, key_down, CurrentTime);
        XFlush(display_);
    }
}

void InputSimulator::simulateMouseClick(int button, bool button_down) {
    if (display_) {
        XTestFakeButtonEvent(display_, button, button_down, CurrentTime);
        XFlush(display_);
    }
}
#elif __APPLE__
void InputSimulator::simulateKeyPress(CGKeyCode key, bool key_down) {
    CGEventRef key_event = CGEventCreateKeyboardEvent(NULL, key, key_down);
    CGEventPost(kCGHIDEventTap, key_event);
    CFRelease(key_event);
}

void InputSimulator::simulateMouseClick(CGMouseButton button, bool button_down) {
    CGPoint cursor = CGEventGetLocation(CGEventCreate(NULL));
    CGEventType event_type;
    
    if (button == kCGMouseButtonLeft) {
        event_type = button_down ? kCGEventLeftMouseDown : kCGEventLeftMouseUp;
    } else if (button == kCGMouseButtonRight) {
        event_type = button_down ? kCGEventRightMouseDown : kCGEventRightMouseUp;
    } else {
        event_type = button_down ? kCGEventOtherMouseDown : kCGEventOtherMouseUp;
    }
    
    CGEventRef click_event = CGEventCreateMouseEvent(NULL, event_type, cursor, button);
    CGEventPost(kCGHIDEventTap, click_event);
    CFRelease(click_event);
}
#endif