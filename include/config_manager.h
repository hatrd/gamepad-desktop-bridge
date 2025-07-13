#pragma once
#include <string>
#include <map>

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();
    
    bool loadConfig(const std::string& filename);
    bool saveConfig(const std::string& filename);
    void loadDefaults();
    
    // Get configuration values
    float getMouseSensitivity() const;
    float getScrollSensitivity() const;
    bool getInvertScroll() const;
    
    // Button mapping
    std::string getButtonAction(const std::string& button) const;
    
    // Set configuration values
    void setMouseSensitivity(float value);
    void setScrollSensitivity(float value);
    void setInvertScroll(bool value);
    void setButtonAction(const std::string& button, const std::string& action);
    
private:
    float mouse_sensitivity_;
    float scroll_sensitivity_;
    bool invert_scroll_;
    std::map<std::string, std::string> button_mappings_;
    
    void parseConfigLine(const std::string& line);
    std::string trim(const std::string& str);
};