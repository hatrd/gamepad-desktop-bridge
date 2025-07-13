#pragma once

class MediaController {
public:
    MediaController();
    ~MediaController();
    
    bool initialize();
    void shutdown();
    
    // Media controls
    void playPause();
    void stop();
    void next();
    void previous();
    
    // Volume controls
    void volumeUp();
    void volumeDown();
    void volumeMute();
    
    // Get current state (if supported by platform)
    bool isPlaying() const;
    int getVolume() const;
    
private:
    bool is_initialized_;
    
#ifdef _WIN32
    void sendMediaKey(unsigned long key);
#elif __linux__
    void sendMediaCommand(const char* command);
#elif __APPLE__
    void sendAppleScriptCommand(const char* script);
#endif
};