#pragma once
#include <SDL3/SDL.h>
#include <functional>
#include <memory>

struct GamepadState {
    float left_stick_x = 0.0f;
    float left_stick_y = 0.0f;
    float right_stick_x = 0.0f;
    float right_stick_y = 0.0f;
    float left_trigger = 0.0f;
    float right_trigger = 0.0f;
    
    bool button_a = false;
    bool button_b = false;
    bool button_x = false;
    bool button_y = false;
    bool button_start = false;
    bool button_back = false;
    bool button_guide = false;
    bool left_shoulder = false;
    bool right_shoulder = false;
    bool left_stick_button = false;
    bool right_stick_button = false;
    
    bool dpad_up = false;
    bool dpad_down = false;
    bool dpad_left = false;
    bool dpad_right = false;
};

class GamepadController {
public:
    GamepadController();
    ~GamepadController();
    
    bool initialize();
    void shutdown();
    
    bool isConnected() const;
    GamepadState getState() const;
    void update();
    
    void setButtonCallback(std::function<void(int, bool)> callback);
    void setAxisCallback(std::function<void(int, float)> callback);
    
private:
    SDL_Gamepad* gamepad_;
    GamepadState current_state_;
    GamepadState previous_state_;
    std::function<void(int, bool)> button_callback_;
    std::function<void(int, float)> axis_callback_;
    
    void processEvents();
    void updateState();
};