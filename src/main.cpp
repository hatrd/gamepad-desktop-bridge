#include <iostream>
#include <thread>
#include <chrono>
#include "gamepad_controller.h"
#include "input_simulator.h"
#include "media_controller.h"

class GamepadAPI {
public:
    GamepadAPI() : running_(false) {}
    
    bool initialize() {
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
        
        std::cout << "Xbox手柄API启动成功!" << std::endl;
        std::cout << "功能说明:" << std::endl;
        std::cout << "- 左摇杆: 控制鼠标移动" << std::endl;
        std::cout << "- 右摇杆: 控制滚轮" << std::endl;
        std::cout << "- A键: 左键点击" << std::endl;
        std::cout << "- B键: 右键点击" << std::endl;
        std::cout << "- X键: 播放/暂停" << std::endl;
        std::cout << "- Y键: 触发语音输入 (Win+H)" << std::endl;
        std::cout << "- Start键: 退出程序" << std::endl;
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
    bool running_;
    
    void setupCallbacks() {
        gamepad_.setButtonCallback([this](int button, bool pressed) {
            if (pressed) {
                handleButtonPress(button);
            }
        });
    }
    
    void handleButtonPress(int button) {
        switch (button) {
            case SDL_GAMEPAD_BUTTON_A:
                input_sim_.leftClick();
                std::cout << "左键点击" << std::endl;
                break;
            case SDL_GAMEPAD_BUTTON_B:
                input_sim_.rightClick();
                std::cout << "右键点击" << std::endl;
                break;
            case SDL_GAMEPAD_BUTTON_X:
                media_ctrl_.playPause();
                std::cout << "播放/暂停" << std::endl;
                break;
            case SDL_GAMEPAD_BUTTON_Y:
                input_sim_.triggerVoiceInput();
                std::cout << "语音输入" << std::endl;
                break;
            case SDL_GAMEPAD_BUTTON_START:
                std::cout << "退出程序..." << std::endl;
                running_ = false;
                break;
        }
    }
    
    void processGamepadInput() {
        if (!gamepad_.isConnected()) return;
        
        auto state = gamepad_.getState();
        
        // 鼠标移动 (左摇杆)
        if (abs(state.left_stick_x) > 0.1f || abs(state.left_stick_y) > 0.1f) {
            int delta_x = static_cast<int>(state.left_stick_x * 10);
            int delta_y = static_cast<int>(state.left_stick_y * 10);
            input_sim_.moveMouse(delta_x, delta_y);
        }
        
        // 滚轮 (右摇杆Y轴)
        if (abs(state.right_stick_y) > 0.3f) {
            int scroll_delta = static_cast<int>(state.right_stick_y * 3);
            input_sim_.scroll(scroll_delta);
        }
    }
};

int main() {
    GamepadAPI api;
    
    if (!api.initialize()) {
        std::cerr << "初始化失败!" << std::endl;
        return -1;
    }
    
    try {
        api.run();
    } catch (const std::exception& e) {
        std::cerr << "运行时错误: " << e.what() << std::endl;
    }
    
    api.shutdown();
    return 0;
}