#include "gamepad_controller.h"
#include <iostream>

GamepadController::GamepadController() 
    : gamepad_(nullptr)
    , current_state_{}
    , previous_state_{}
{
}

GamepadController::~GamepadController() {
    shutdown();
}

bool GamepadController::initialize() {
    if (SDL_Init(SDL_INIT_GAMEPAD) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Wait for gamepad connection
    std::cout << "Waiting for gamepad connection..." << std::endl;
    
    int timeout = 5000; // 5 second timeout
    while (timeout > 0) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_GAMEPAD_ADDED) {
                gamepad_ = SDL_OpenGamepad(event.gdevice.which);
                if (gamepad_) {
                    std::cout << "Gamepad connected: " << SDL_GetGamepadName(gamepad_) << std::endl;
                    return true;
                }
            }
        }
        
        // 检查已连接的手柄
        int num_joysticks = 0;
        SDL_JoystickID* joysticks = SDL_GetJoysticks(&num_joysticks);
        for (int i = 0; i < num_joysticks; ++i) {
            if (SDL_IsGamepad(joysticks[i])) {
                gamepad_ = SDL_OpenGamepad(joysticks[i]);
                if (gamepad_) {
                    std::cout << "Gamepad connected: " << SDL_GetGamepadName(gamepad_) << std::endl;
                    SDL_free(joysticks);
                    return true;
                }
            }
        }
        SDL_free(joysticks);
        
        SDL_Delay(100);
        timeout -= 100;
    }
    
    std::cout << "No gamepad detected within 5 seconds. Please ensure gamepad is connected and recognized by system." << std::endl;
    return false;
}

void GamepadController::shutdown() {
    if (gamepad_) {
        SDL_CloseGamepad(gamepad_);
        gamepad_ = nullptr;
    }
    SDL_Quit();
}

bool GamepadController::isConnected() const {
    return gamepad_ != nullptr && SDL_GamepadConnected(gamepad_);
}

GamepadState GamepadController::getState() const {
    return current_state_;
}

void GamepadController::update() {
    processEvents();
    updateState();
}

void GamepadController::setButtonCallback(std::function<void(int, bool)> callback) {
    button_callback_ = callback;
}

void GamepadController::setAxisCallback(std::function<void(int, float)> callback) {
    axis_callback_ = callback;
}

void GamepadController::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_GAMEPAD_ADDED:
                if (!gamepad_) {
                    gamepad_ = SDL_OpenGamepad(event.gdevice.which);
                    if (gamepad_) {
                        std::cout << "手柄连接: " << SDL_GetGamepadName(gamepad_) << std::endl;
                    }
                }
                break;
                
            case SDL_EVENT_GAMEPAD_REMOVED:
                if (gamepad_ && event.gdevice.which == SDL_GetGamepadID(gamepad_)) {
                    SDL_CloseGamepad(gamepad_);
                    gamepad_ = nullptr;
                    std::cout << "Gamepad disconnected" << std::endl;
                }
                break;
                
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            case SDL_EVENT_GAMEPAD_BUTTON_UP:
                if (button_callback_ && gamepad_ && 
                    event.gbutton.which == SDL_GetGamepadID(gamepad_)) {
                    button_callback_(event.gbutton.button, 
                                   event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN);
                }
                break;
                
            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                if (axis_callback_ && gamepad_ && 
                    event.gaxis.which == SDL_GetGamepadID(gamepad_)) {
                    float value = event.gaxis.value / 32767.0f;
                    axis_callback_(event.gaxis.axis, value);
                }
                break;
        }
    }
}

void GamepadController::updateState() {
    if (!isConnected()) return;
    
    previous_state_ = current_state_;
    
    // 读取摇杆
    current_state_.left_stick_x = SDL_GetGamepadAxis(gamepad_, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f;
    current_state_.left_stick_y = SDL_GetGamepadAxis(gamepad_, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f;
    current_state_.right_stick_x = SDL_GetGamepadAxis(gamepad_, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f;
    current_state_.right_stick_y = SDL_GetGamepadAxis(gamepad_, SDL_GAMEPAD_AXIS_RIGHTY) / 32767.0f;
    
    // 读取扳机
    current_state_.left_trigger = SDL_GetGamepadAxis(gamepad_, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) / 32767.0f;
    current_state_.right_trigger = SDL_GetGamepadAxis(gamepad_, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) / 32767.0f;
    
    // 读取按钮
    current_state_.button_a = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_SOUTH);
    current_state_.button_b = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_EAST);
    current_state_.button_x = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_WEST);
    current_state_.button_y = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_NORTH);
    current_state_.button_start = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_START);
    current_state_.button_back = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_BACK);
    current_state_.button_guide = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_GUIDE);
    current_state_.left_shoulder = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
    current_state_.right_shoulder = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
    current_state_.left_stick_button = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_LEFT_STICK);
    current_state_.right_stick_button = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_RIGHT_STICK);
    
    // 读取方向键
    current_state_.dpad_up = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_DPAD_UP);
    current_state_.dpad_down = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
    current_state_.dpad_left = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
    current_state_.dpad_right = SDL_GetGamepadButton(gamepad_, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
}
