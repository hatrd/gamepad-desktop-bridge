#pragma once

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#elif __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#endif

class InputSimulator {
public:
    InputSimulator();
    ~InputSimulator();
    
    bool initialize();
    void shutdown();
    
    // Mouse control
    void moveMouse(int delta_x, int delta_y);
    void setMousePosition(int x, int y);
    void leftClick();
    void rightClick();
    void middleClick();
    void scroll(int delta);
    
    // Mouse button press/release
    void leftMouseDown();
    void leftMouseUp();
    void rightMouseDown();
    void rightMouseUp();
    
    // Keyboard control
    void pressKey(int key_code);
    void releaseKey(int key_code);
    void typeKey(int key_code);
    void typeText(const char* text);
    
    // System shortcuts
    void triggerVoiceInput();
    void altTab();  // Alt+Tab task switcher
    void winTab();  // Win+Tab task view
    void escape();  // Escape key
    void enter();   // Enter key
    void winKey();  // Windows key
    void screenshot(); // Win+Shift+S
    void mediaPlayPause();
    void mediaNext();
    void mediaPrevious();
    void volumeUp();
    void volumeDown();
    void volumeMute();
    
private:
#ifdef _WIN32
    void simulateKeyPress(WORD key, bool key_down);
    void simulateMouseClick(DWORD button, bool button_down);
#elif __linux__
    Display* display_;
    void simulateKeyPress(KeyCode key, bool key_down);
    void simulateMouseClick(int button, bool button_down);
#elif __APPLE__
    void simulateKeyPress(CGKeyCode key, bool key_down);
    void simulateMouseClick(CGMouseButton button, bool button_down);
#endif
};