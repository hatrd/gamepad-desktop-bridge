# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Cross-platform Xbox controller support
- Mouse control via gamepad sticks
- Keyboard input simulation
- Media playback control
- Voice input trigger (Win+H)
- Real-time gamepad state monitoring

### Changed
- Initial project structure
- SDL3 integration
- vcpkg configuration

### Security
- Input simulation with proper platform permissions

## [1.0.0] - 2025-01-XX

### Added
- Initial release
- Windows, Linux, macOS support
- SDL3 gamepad integration
- Basic mouse and keyboard control
- Media control functionality
- Voice input trigger
- CI/CD workflows
- Comprehensive build documentation
- vcpkg package management

### Features
- Left stick controls mouse movement
- Right stick controls scroll wheel
- A button for left mouse click
- B button for right mouse click
- X button for media play/pause
- Y button for voice input (Win+H)
- Start button to exit program

### Technical
- C++20 implementation
- CMake build system
- Cross-platform input simulation
- Real-time event processing
- Low-latency input handling

### Documentation
- Detailed build instructions for all platforms
- CI/CD setup for automated releases
- Comprehensive README with usage examples
- Troubleshooting guide

### Dependencies
- SDL3 for gamepad input
- Platform-specific libraries for input simulation
- vcpkg for dependency management