#include "media_controller.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <cstdlib>
#elif __APPLE__
#include <cstdlib>
#endif

MediaController::MediaController() : is_initialized_(false) {
}

MediaController::~MediaController() {
    shutdown();
}

bool MediaController::initialize() {
    is_initialized_ = true;
    std::cout << "媒体控制器初始化成功" << std::endl;
    return true;
}

void MediaController::shutdown() {
    is_initialized_ = false;
}

void MediaController::playPause() {
    if (!is_initialized_) return;
    
#ifdef _WIN32
    sendMediaKey(VK_MEDIA_PLAY_PAUSE);
#elif __linux__
    sendMediaCommand("playerctl play-pause");
#elif __APPLE__
    sendAppleScriptCommand("tell application \"Music\" to playpause");
#endif
}

void MediaController::stop() {
    if (!is_initialized_) return;
    
#ifdef _WIN32
    sendMediaKey(VK_MEDIA_STOP);
#elif __linux__
    sendMediaCommand("playerctl stop");
#elif __APPLE__
    sendAppleScriptCommand("tell application \"Music\" to stop");
#endif
}

void MediaController::next() {
    if (!is_initialized_) return;
    
#ifdef _WIN32
    sendMediaKey(VK_MEDIA_NEXT_TRACK);
#elif __linux__
    sendMediaCommand("playerctl next");
#elif __APPLE__
    sendAppleScriptCommand("tell application \"Music\" to next track");
#endif
}

void MediaController::previous() {
    if (!is_initialized_) return;
    
#ifdef _WIN32
    sendMediaKey(VK_MEDIA_PREV_TRACK);
#elif __linux__
    sendMediaCommand("playerctl previous");
#elif __APPLE__
    sendAppleScriptCommand("tell application \"Music\" to previous track");
#endif
}

void MediaController::volumeUp() {
    if (!is_initialized_) return;
    
#ifdef _WIN32
    sendMediaKey(VK_VOLUME_UP);
#elif __linux__
    sendMediaCommand("pactl set-sink-volume @DEFAULT_SINK@ +5%");
#elif __APPLE__
    sendAppleScriptCommand("set volume output volume (output volume of (get volume settings) + 10)");
#endif
}

void MediaController::volumeDown() {
    if (!is_initialized_) return;
    
#ifdef _WIN32
    sendMediaKey(VK_VOLUME_DOWN);
#elif __linux__
    sendMediaCommand("pactl set-sink-volume @DEFAULT_SINK@ -5%");
#elif __APPLE__
    sendAppleScriptCommand("set volume output volume (output volume of (get volume settings) - 10)");
#endif
}

void MediaController::volumeMute() {
    if (!is_initialized_) return;
    
#ifdef _WIN32
    sendMediaKey(VK_VOLUME_MUTE);
#elif __linux__
    sendMediaCommand("pactl set-sink-mute @DEFAULT_SINK@ toggle");
#elif __APPLE__
    sendAppleScriptCommand("set volume with output muted");
#endif
}

bool MediaController::isPlaying() const {
    // 实现播放状态检测 (平台相关)
    return false;
}

int MediaController::getVolume() const {
    // 实现音量获取 (平台相关)
    return 50;
}

// Private helper methods
#ifdef _WIN32
void MediaController::sendMediaKey(DWORD key) {
    keybd_event(key, 0, 0, 0);
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}
#elif __linux__
void MediaController::sendMediaCommand(const char* command) {
    if (system(command) != 0) {
        std::cerr << "媒体命令执行失败: " << command << std::endl;
    }
}
#elif __APPLE__
void MediaController::sendAppleScriptCommand(const char* script) {
    char command[512];
    snprintf(command, sizeof(command), "osascript -e '%s'", script);
    if (system(command) != 0) {
        std::cerr << "AppleScript命令执行失败: " << script << std::endl;
    }
}
#endif