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
        std::cerr << "无法打开X11显示" << std::endl;
        return false;
    }
    
    int event_base, error_base, major, minor;
    if (!XTestQueryExtension(display_, &event_base, &error_base, &major, &minor)) {
        std::cerr << "XTest扩展不可用" << std::endl;
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
        XTestRelativeMotionPtr(display_, delta_x, delta_y);
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
    simulateMouseClick(MOUSEEVENTF_LEFTDOWN, true);
    simulateMouseClick(MOUSEEVENTF_LEFTUP, false);
#elif __linux__
    simulateMouseClick(Button1, true);
    simulateMouseClick(Button1, false);
#elif __APPLE__
    simulateMouseClick(kCGMouseButtonLeft, true);
    simulateMouseClick(kCGMouseButtonLeft, false);
#endif
}

void InputSimulator::rightClick() {
#ifdef _WIN32
    simulateMouseClick(MOUSEEVENTF_RIGHTDOWN, true);
    simulateMouseClick(MOUSEEVENTF_RIGHTUP, false);
#elif __linux__
    simulateMouseClick(Button3, true);
    simulateMouseClick(Button3, false);
#elif __APPLE__
    simulateMouseClick(kCGMouseButtonRight, true);
    simulateMouseClick(kCGMouseButtonRight, false);
#endif
}

void InputSimulator::middleClick() {
#ifdef _WIN32
    simulateMouseClick(MOUSEEVENTF_MIDDLEDOWN, true);
    simulateMouseClick(MOUSEEVENTF_MIDDLEUP, false);
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
    std::cout << "语音输入触发 (Linux暂未实现)" << std::endl;
#elif __APPLE__
    // macOS voice input trigger
    std::cout << "语音输入触发 (macOS暂未实现)" << std::endl;
#endif
}

void InputSimulator::mediaPlayPause() {
#ifdef _WIN32
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0);
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    system("playerctl play-pause");
#elif __APPLE__
    system("osascript -e 'tell application \"Music\" to playpause'");
#endif
}

void InputSimulator::mediaNext() {
#ifdef _WIN32
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, 0, 0);
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    system("playerctl next");
#elif __APPLE__
    system("osascript -e 'tell application \"Music\" to next track'");
#endif
}

void InputSimulator::mediaPrevious() {
#ifdef _WIN32
    keybd_event(VK_MEDIA_PREV_TRACK, 0, 0, 0);
    keybd_event(VK_MEDIA_PREV_TRACK, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    system("playerctl previous");
#elif __APPLE__
    system("osascript -e 'tell application \"Music\" to previous track'");
#endif
}

void InputSimulator::volumeUp() {
#ifdef _WIN32
    keybd_event(VK_VOLUME_UP, 0, 0, 0);
    keybd_event(VK_VOLUME_UP, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    system("pactl set-sink-volume @DEFAULT_SINK@ +5%");
#elif __APPLE__
    system("osascript -e 'set volume output volume (output volume of (get volume settings) + 10)'");
#endif
}

void InputSimulator::volumeDown() {
#ifdef _WIN32
    keybd_event(VK_VOLUME_DOWN, 0, 0, 0);
    keybd_event(VK_VOLUME_DOWN, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    system("pactl set-sink-volume @DEFAULT_SINK@ -5%");
#elif __APPLE__
    system("osascript -e 'set volume output volume (output volume of (get volume settings) - 10)'");
#endif
}

void InputSimulator::volumeMute() {
#ifdef _WIN32
    keybd_event(VK_VOLUME_MUTE, 0, 0, 0);
    keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_KEYUP, 0);
#elif __linux__
    system("pactl set-sink-mute @DEFAULT_SINK@ toggle");
#elif __APPLE__
    system("osascript -e 'set volume with output muted'");
#endif
}

// Private helper methods
#ifdef _WIN32
void InputSimulator::simulateKeyPress(WORD key, bool key_down) {
    DWORD flags = key_down ? 0 : KEYEVENTF_KEYUP;
    keybd_event(key, 0, flags, 0);
}

void InputSimulator::simulateMouseClick(DWORD button, bool button_down) {
    DWORD flags = button_down ? button : (button << 1);
    mouse_event(flags, 0, 0, 0, 0);
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