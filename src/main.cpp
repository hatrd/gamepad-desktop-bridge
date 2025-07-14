#include <iostream>
#include <thread>
#include <chrono>
#include "gamepad_controller.h"
#include "input_simulator.h"
#include "media_controller.h"
#include "config_manager.h"

class GamepadAPI {
public:
    GamepadAPI() : running_(false) {
        // Initialize all button states
        prev_button_a_ = false;
        prev_button_b_ = false;
        prev_button_x_ = false;
        prev_button_y_ = false;
        prev_button_start_ = false;
        prev_button_back_ = false;
        prev_button_guide_ = false;
        prev_left_shoulder_ = false;
        prev_right_shoulder_ = false;
        prev_left_stick_button_ = false;
        prev_right_stick_button_ = false;
        prev_dpad_up_ = false;
        prev_dpad_down_ = false;
        prev_dpad_left_ = false;
        prev_dpad_right_ = false;
        left_mouse_held_ = false;
        right_mouse_held_ = false;
        prev_left_trigger_pressed_ = false;
        prev_right_trigger_pressed_ = false;
        mouse_sensitivity_ = 1.0f;
        scroll_sensitivity_ = 1.0f;
        invert_scroll_y_ = false;
    }
    
    bool initialize() {
        // Load configuration
        config_.loadConfig("controller_config.txt");
        config_.saveConfig("controller_config.txt");  // Save defaults if not exists
        
        // Load sensitivity settings from config
        mouse_sensitivity_ = config_.getMouseSensitivity();
        scroll_sensitivity_ = config_.getScrollSensitivity();
        invert_scroll_y_ = config_.getInvertScroll();
        
        if (!gamepad_.initialize()) {
            std::cerr << "Failed to initialize gamepad controller" << std::endl;
            return false;
        }
        
        if (!input_sim_.initialize()) {
            std::cerr << "Failed to initialize input simulator" << std::endl;
            return false;
        }
        
        if (!media_ctrl_.initialize()) {
            std::cerr << "Failed to initialize media controller" << std::endl;
            return false;
        }
        
        setupCallbacks();
        return true;
    }
    
    void run() {
        running_ = true;
        
        std::cout << "Xbox Controller API started successfully!" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "- Left stick: Mouse movement" << std::endl;
        std::cout << "- Right stick: Scroll wheel (Y-axis, " << (invert_scroll_y_ ? "inverted" : "normal") << ")" << std::endl;
        std::cout << "- A button: " << config_.getButtonAction("button_a") << std::endl;
        std::cout << "- B button: " << config_.getButtonAction("button_b") << std::endl;
        std::cout << "- X button: " << config_.getButtonAction("button_x") << std::endl;
        std::cout << "- Y button: " << config_.getButtonAction("button_y") << std::endl;
        std::cout << "- Left Shoulder: " << config_.getButtonAction("left_shoulder") << std::endl;
        std::cout << "- Right Shoulder: " << config_.getButtonAction("right_shoulder") << std::endl;
        std::cout << "- Back button: " << config_.getButtonAction("button_back") << std::endl;
        std::cout << "- Guide button: " << config_.getButtonAction("button_guide") << std::endl;
        std::cout << "- Left stick click: " << config_.getButtonAction("left_stick_button") << std::endl;
        std::cout << "- Right stick click: " << config_.getButtonAction("right_stick_button") << std::endl;
        std::cout << "- Left Trigger: " << config_.getButtonAction("left_trigger") << std::endl;
        std::cout << "- Right Trigger: " << config_.getButtonAction("right_trigger") << std::endl;
        std::cout << "- Start button: " << config_.getButtonAction("button_start") << std::endl;
        std::cout << "- D-pad Up: " << config_.getButtonAction("dpad_up") << std::endl;
        std::cout << "- D-pad Down: " << config_.getButtonAction("dpad_down") << std::endl;
        std::cout << "- D-pad Left: " << config_.getButtonAction("dpad_left") << std::endl;
        std::cout << "- D-pad Right: " << config_.getButtonAction("dpad_right") << std::endl;
        std::cout << "-------------------------------" << std::endl;
        
        while (running_) {
            gamepad_.update();
            processGamepadInput();
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    }
    
    void shutdown() {
        running_ = false;
        gamepad_.shutdown();
        input_sim_.shutdown();
        media_ctrl_.shutdown();
    }
    
private:
    GamepadController gamepad_;
    InputSimulator input_sim_;
    MediaController media_ctrl_;
    ConfigManager config_;
    bool running_;
    
    // Sensitivity settings
    float mouse_sensitivity_;
    float scroll_sensitivity_;
    bool invert_scroll_y_;
    
    // Previous button states for edge detection
    bool prev_button_a_;
    bool prev_button_b_;
    bool prev_button_x_;
    bool prev_button_y_;
    bool prev_button_start_;
    bool prev_button_back_;
    bool prev_button_guide_;
    bool prev_left_shoulder_;
    bool prev_right_shoulder_;
    bool prev_left_stick_button_;
    bool prev_right_stick_button_;
    bool prev_dpad_up_;
    bool prev_dpad_down_;
    bool prev_dpad_left_;
    bool prev_dpad_right_;
    
    // Button hold states for mouse buttons
    bool left_mouse_held_;
    bool right_mouse_held_;
    
    // Trigger states for edge detection
    bool prev_left_trigger_pressed_;
    bool prev_right_trigger_pressed_;
    
    void setupCallbacks() {
        // Remove callback-based approach, use state polling instead
    }
    
    void handleButtonAction(const std::string& action) {
        if (action.empty()) return;
        
        if (action == "left_click") {
            if (!left_mouse_held_) {
                input_sim_.leftMouseDown();
                left_mouse_held_ = true;
                std::cout << "Left mouse down" << std::endl;
            }
        } else if (action == "right_click") {
            if (!right_mouse_held_) {
                input_sim_.rightMouseDown();
                right_mouse_held_ = true;
                std::cout << "Right mouse down" << std::endl;
            }
        } else if (action == "middle_click") {
            input_sim_.middleClick();
            std::cout << "Middle click" << std::endl;
        } else if (action == "media_play_pause") {
            media_ctrl_.playPause();
            std::cout << "Play/Pause" << std::endl;
        } else if (action == "media_next") {
            media_ctrl_.next();
            std::cout << "Next track" << std::endl;
        } else if (action == "media_previous") {
            media_ctrl_.previous();
            std::cout << "Previous track" << std::endl;
        } else if (action == "voice_input") {
            input_sim_.triggerVoiceInput();
            std::cout << "Voice input" << std::endl;
        } else if (action == "alt_tab") {
            input_sim_.altTab();
            std::cout << "Alt+Tab" << std::endl;
        } else if (action == "win_tab") {
            input_sim_.winTab();
            std::cout << "Win+Tab" << std::endl;
        } else if (action == "escape") {
            input_sim_.escape();
            std::cout << "Escape" << std::endl;
        } else if (action == "enter") {
            input_sim_.enter();
            std::cout << "Enter" << std::endl;
        } else if (action == "windows_key") {
            input_sim_.winKey();
            std::cout << "Windows key" << std::endl;
        } else if (action == "screenshot") {
            input_sim_.screenshot();
            std::cout << "Screenshot" << std::endl;
        } else if (action == "volume_up") {
            input_sim_.volumeUp();
            std::cout << "Volume up" << std::endl;
        } else if (action == "volume_down") {
            input_sim_.volumeDown();
            std::cout << "Volume down" << std::endl;
        } else if (action == "volume_mute") {
            input_sim_.volumeMute();
            std::cout << "Volume mute" << std::endl;
        } else if (action == "browser_back") {
            input_sim_.browserBack();
            std::cout << "Browser back" << std::endl;
        } else if (action == "browser_forward") {
            input_sim_.browserForward();
            std::cout << "Browser forward" << std::endl;
        } else if (action == "increase_mouse_sensitivity") {
            mouse_sensitivity_ = std::min(5.0f, mouse_sensitivity_ + 0.2f);
            config_.setMouseSensitivity(mouse_sensitivity_);
            config_.saveConfig("controller_config.txt");
            std::cout << "Mouse sensitivity: " << mouse_sensitivity_ << std::endl;
        } else if (action == "decrease_mouse_sensitivity") {
            mouse_sensitivity_ = std::max(0.2f, mouse_sensitivity_ - 0.2f);
            config_.setMouseSensitivity(mouse_sensitivity_);
            config_.saveConfig("controller_config.txt");
            std::cout << "Mouse sensitivity: " << mouse_sensitivity_ << std::endl;
        } else if (action == "increase_scroll_sensitivity") {
            scroll_sensitivity_ = std::min(5.0f, scroll_sensitivity_ + 0.2f);
            config_.setScrollSensitivity(scroll_sensitivity_);
            config_.saveConfig("controller_config.txt");
            std::cout << "Scroll sensitivity: " << scroll_sensitivity_ << std::endl;
        } else if (action == "decrease_scroll_sensitivity") {
            scroll_sensitivity_ = std::max(0.2f, scroll_sensitivity_ - 0.2f);
            config_.setScrollSensitivity(scroll_sensitivity_);
            config_.saveConfig("controller_config.txt");
            std::cout << "Scroll sensitivity: " << scroll_sensitivity_ << std::endl;
        } else if (action == "exit") {
            std::cout << "Exiting program..." << std::endl;
            running_ = false;
        }
    }
    
    void handleButtonRelease(const std::string& action) {
        if (action.empty()) return;
        
        if (action == "left_click" && left_mouse_held_) {
            input_sim_.leftMouseUp();
            left_mouse_held_ = false;
            std::cout << "Left mouse up" << std::endl;
        } else if (action == "right_click" && right_mouse_held_) {
            input_sim_.rightMouseUp();
            right_mouse_held_ = false;
            std::cout << "Right mouse up" << std::endl;
        }
    }
    
    void processGamepadInput() {
        if (!gamepad_.isConnected()) return;
        
        auto state = gamepad_.getState();
        
        // Handle button A
        if (state.button_a && !prev_button_a_) {
            handleButtonAction(config_.getButtonAction("button_a"));
        } else if (!state.button_a && prev_button_a_) {
            handleButtonRelease(config_.getButtonAction("button_a"));
        }
        
        // Handle button B
        if (state.button_b && !prev_button_b_) {
            handleButtonAction(config_.getButtonAction("button_b"));
        } else if (!state.button_b && prev_button_b_) {
            handleButtonRelease(config_.getButtonAction("button_b"));
        }
        
        // Handle other button presses (only trigger on press)
        if (state.button_x && !prev_button_x_) {
            handleButtonAction(config_.getButtonAction("button_x"));
        }
        
        if (state.button_y && !prev_button_y_) {
            handleButtonAction(config_.getButtonAction("button_y"));
        }
        
        if (state.left_shoulder && !prev_left_shoulder_) {
            handleButtonAction(config_.getButtonAction("left_shoulder"));
        }
        
        if (state.right_shoulder && !prev_right_shoulder_) {
            handleButtonAction(config_.getButtonAction("right_shoulder"));
        }
        
        if (state.button_back && !prev_button_back_) {
            handleButtonAction(config_.getButtonAction("button_back"));
        }
        
        if (state.button_guide && !prev_button_guide_) {
            handleButtonAction(config_.getButtonAction("button_guide"));
        }
        
        if (state.left_stick_button && !prev_left_stick_button_) {
            handleButtonAction(config_.getButtonAction("left_stick_button"));
        }
        
        if (state.right_stick_button && !prev_right_stick_button_) {
            handleButtonAction(config_.getButtonAction("right_stick_button"));
        }
        
        if (state.button_start && !prev_button_start_) {
            handleButtonAction(config_.getButtonAction("button_start"));
        }
        
        // Handle triggers
        bool left_trigger_pressed = state.left_trigger > 0.5f;
        bool right_trigger_pressed = state.right_trigger > 0.5f;
        
        if (left_trigger_pressed && !prev_left_trigger_pressed_) {
            handleButtonAction(config_.getButtonAction("left_trigger"));
        }
        
        if (right_trigger_pressed && !prev_right_trigger_pressed_) {
            handleButtonAction(config_.getButtonAction("right_trigger"));
        }
        
        prev_left_trigger_pressed_ = left_trigger_pressed;
        prev_right_trigger_pressed_ = right_trigger_pressed;
        
        // Handle D-pad
        if (state.dpad_up && !prev_dpad_up_) {
            handleButtonAction(config_.getButtonAction("dpad_up"));
        }
        if (state.dpad_down && !prev_dpad_down_) {
            handleButtonAction(config_.getButtonAction("dpad_down"));
        }
        if (state.dpad_right && !prev_dpad_right_) {
            handleButtonAction(config_.getButtonAction("dpad_right"));
        }
        if (state.dpad_left && !prev_dpad_left_) {
            handleButtonAction(config_.getButtonAction("dpad_left"));
        }
        
        // Update all previous button states
        prev_button_a_ = state.button_a;
        prev_button_b_ = state.button_b;
        prev_button_x_ = state.button_x;
        prev_button_y_ = state.button_y;
        prev_button_start_ = state.button_start;
        prev_button_back_ = state.button_back;
        prev_button_guide_ = state.button_guide;
        prev_left_shoulder_ = state.left_shoulder;
        prev_right_shoulder_ = state.right_shoulder;
        prev_left_stick_button_ = state.left_stick_button;
        prev_right_stick_button_ = state.right_stick_button;
        prev_dpad_up_ = state.dpad_up;
        prev_dpad_down_ = state.dpad_down;
        prev_dpad_left_ = state.dpad_left;
        prev_dpad_right_ = state.dpad_right;
        
        // Mouse movement (left stick) with sensitivity
        if (abs(state.left_stick_x) > 0.1f || abs(state.left_stick_y) > 0.1f) {
            int delta_x = static_cast<int>(state.left_stick_x * 15 * mouse_sensitivity_);
            int delta_y = static_cast<int>(state.left_stick_y * 15 * mouse_sensitivity_);
            input_sim_.moveMouse(delta_x, delta_y);
        }
        
        // Scroll wheel (right stick Y-axis) with sensitivity and inversion
        if (abs(state.right_stick_y) > 0.3f) {
            float y_value = invert_scroll_y_ ? -state.right_stick_y : state.right_stick_y;
            int scroll_delta = static_cast<int>(y_value * 5 * scroll_sensitivity_);
            input_sim_.scroll(scroll_delta);
        }
    }
};

int main() {
    try {
        std::cout << "Starting Xbox Controller API..." << std::endl;
        std::cout << "Checking system compatibility..." << std::endl;
        
        GamepadAPI api;
        
        std::cout << "Initializing components..." << std::endl;
        if (!api.initialize()) {
            std::cerr << "Initialization failed!" << std::endl;
            std::cout << "Press any key to exit..." << std::endl;
            std::cin.get();
            return -1;
        }
        
        std::cout << "Starting main loop..." << std::endl;
        api.run();
        
        std::cout << "Shutting down..." << std::endl;
        api.shutdown();
        
    } catch (const std::exception& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        std::cout << "Press any key to exit..." << std::endl;
        std::cin.get();
        return -1;
    } catch (...) {
        std::cerr << "Unknown error occurred!" << std::endl;
        std::cout << "Press any key to exit..." << std::endl;
        std::cin.get();
        return -1;
    }
    
    return 0;
}
