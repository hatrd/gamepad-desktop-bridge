#include "config_manager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

ConfigManager::ConfigManager() {
    loadDefaults();
}

ConfigManager::~ConfigManager() {
}

void ConfigManager::loadDefaults() {
    // Default sensitivity settings
    mouse_sensitivity_ = 1.0f;
    scroll_sensitivity_ = 1.0f;
    invert_scroll_ = true;  // Default to inverted (natural scrolling)
    
    // Default button mappings
    button_mappings_["button_a"] = "left_click";
    button_mappings_["button_b"] = "right_click";
    button_mappings_["button_x"] = "media_play_pause";
    button_mappings_["button_y"] = "voice_input";
    button_mappings_["button_start"] = "exit";
    button_mappings_["button_back"] = "escape";
    button_mappings_["button_guide"] = "windows_key";
    button_mappings_["left_shoulder"] = "middle_click";
    button_mappings_["right_shoulder"] = "enter";
    button_mappings_["left_stick_button"] = "win_tab";
    button_mappings_["right_stick_button"] = "screenshot";
    button_mappings_["dpad_up"] = "increase_mouse_sensitivity";
    button_mappings_["dpad_down"] = "decrease_mouse_sensitivity";
    button_mappings_["dpad_left"] = "decrease_scroll_sensitivity";
    button_mappings_["dpad_right"] = "increase_scroll_sensitivity";
    button_mappings_["left_trigger"] = "media_previous";
    button_mappings_["right_trigger"] = "media_next";
    
    // 添加前进/后退和音量控制作为可选映射
    // 用户可以在配置文件中手动设置这些映射到任意按键
}

bool ConfigManager::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Config file not found, using defaults: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        parseConfigLine(line);
    }
    
    file.close();
    std::cout << "Config loaded from: " << filename << std::endl;
    return true;
}

bool ConfigManager::saveConfig(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to save config: " << filename << std::endl;
        return false;
    }
    
    file << "# Xbox Controller API Configuration\n";
    file << "# Generated config file - feel free to edit\n\n";
    
    file << "# Sensitivity Settings\n";
    file << "mouse_sensitivity = " << mouse_sensitivity_ << "\n";
    file << "scroll_sensitivity = " << scroll_sensitivity_ << "\n";
    file << "invert_scroll = " << (invert_scroll_ ? "true" : "false") << "\n\n";
    
    file << "# Button Mappings\n";
    file << "# Available actions:\n";
    file << "#   left_click, right_click, middle_click\n";
    file << "#   media_play_pause, media_next, media_previous\n";
    file << "#   voice_input, alt_tab, win_tab, escape, enter\n";
    file << "#   windows_key, screenshot, volume_up, volume_down, volume_mute\n";
    file << "#   browser_back, browser_forward\n";
    file << "#   increase/decrease_mouse/scroll_sensitivity, exit\n\n";
    
    for (const auto& mapping : button_mappings_) {
        file << mapping.first << " = " << mapping.second << "\n";
    }
    
    file.close();
    std::cout << "Config saved to: " << filename << std::endl;
    return true;
}

void ConfigManager::parseConfigLine(const std::string& line) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') return;
    
    size_t pos = line.find('=');
    if (pos == std::string::npos) return;
    
    std::string key = trim(line.substr(0, pos));
    std::string value = trim(line.substr(pos + 1));
    
    // Parse sensitivity settings
    if (key == "mouse_sensitivity") {
        mouse_sensitivity_ = std::stof(value);
    } else if (key == "scroll_sensitivity") {
        scroll_sensitivity_ = std::stof(value);
    } else if (key == "invert_scroll") {
        invert_scroll_ = (value == "true" || value == "1");
    } else {
        // Assume it's a button mapping
        button_mappings_[key] = value;
    }
}

std::string ConfigManager::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    return str.substr(first, last - first + 1);
}

float ConfigManager::getMouseSensitivity() const {
    return mouse_sensitivity_;
}

float ConfigManager::getScrollSensitivity() const {
    return scroll_sensitivity_;
}

bool ConfigManager::getInvertScroll() const {
    return invert_scroll_;
}

std::string ConfigManager::getButtonAction(const std::string& button) const {
    auto it = button_mappings_.find(button);
    if (it != button_mappings_.end()) {
        return it->second;
    }
    return "";
}

void ConfigManager::setMouseSensitivity(float value) {
    mouse_sensitivity_ = std::max(0.2f, std::min(5.0f, value));
}

void ConfigManager::setScrollSensitivity(float value) {
    scroll_sensitivity_ = std::max(0.2f, std::min(5.0f, value));
}

void ConfigManager::setInvertScroll(bool value) {
    invert_scroll_ = value;
}

void ConfigManager::setButtonAction(const std::string& button, const std::string& action) {
    button_mappings_[button] = action;
}
