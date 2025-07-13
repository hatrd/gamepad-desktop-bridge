# Xbox Controller API

跨平台Xbox手柄API，支持鼠标、键盘、媒体控制和语音输入。

## 功能特性

- ✅ **跨平台支持**: Windows, Linux, macOS
- ✅ **Xbox手柄支持**: 自动检测Xbox兼容手柄
- ✅ **鼠标控制**: 摇杆控制鼠标移动，按钮控制点击
- ✅ **键盘模拟**: 支持按键和文本输入
- ✅ **媒体控制**: 播放/暂停/上一首/下一首
- ✅ **语音输入**: Win+H语音输入触发
- ✅ **实时响应**: 低延迟输入处理

## 快速开始

### 按键映射

- **左摇杆**: 控制鼠标移动
- **右摇杆**: 控制滚轮 (Y轴)
- **A键**: 左键点击
- **B键**: 右键点击  
- **X键**: 播放/暂停媒体
- **Y键**: 触发语音输入 (Win+H)
- **Start键**: 退出程序

### 下载和运行

1. 从 [Releases](<repository-url>/releases) 下载对应平台的预编译包
2. 解压缩文件
3. 连接Xbox兼容手柄
4. 运行可执行文件

## 编译构建

详细的编译说明请参考 [BUILD.md](BUILD.md)

### Windows (vcpkg)
```bash
vcpkg install sdl3:x64-windows
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

### Linux (apt)
```bash
sudo apt install libsdl3-dev libx11-dev libxtst-dev
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### macOS (Homebrew)
```bash
brew install sdl3
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

## 项目结构

```
xbox-controller-api/
├── include/                 # 头文件
│   ├── gamepad_controller.h
│   ├── input_simulator.h
│   └── media_controller.h
├── src/                     # 源文件
│   ├── main.cpp
│   ├── gamepad_controller.cpp
│   ├── input_simulator.cpp
│   └── media_controller.cpp
├── scripts/                 # 构建脚本
├── .github/workflows/       # CI/CD配置
├── CMakeLists.txt          # CMake配置
├── vcpkg.json              # vcpkg依赖
└── BUILD.md                # 详细构建说明
```

## 依赖库

- **SDL3**: 跨平台手柄输入库
- **X11** (Linux): X Window系统支持
- **XTest** (Linux): X11扩展测试库
- **User32** (Windows): Windows用户界面API
- **CoreGraphics** (macOS): macOS图形框架

## 系统要求

### Windows
- Windows 10+ (推荐)
- Visual Studio 2019+ 或 MinGW-w64
- Xbox兼容手柄

### Linux
- Ubuntu 20.04+ / Fedora 33+ / Arch Linux
- X11桌面环境
- PulseAudio (音频控制)
- PlayerCtl (媒体控制)

### macOS
- macOS 10.15+
- Xcode 12+
- 输入监控权限

## 故障排除

### 手柄无法检测
- 确保手柄已正确连接并被系统识别
- 检查USB端口和驱动程序
- 尝试重新插拔手柄

### 权限问题
```bash
# Linux: 添加用户到input组
sudo usermod -a -G input $USER

# macOS: 系统偏好设置 -> 安全性与隐私 -> 隐私 -> 输入监控
```

### 媒体控制无响应
```bash
# Linux: 安装PlayerCtl
sudo apt install playerctl

# 确保有媒体播放器正在运行
```

## 开发贡献

欢迎提交Issues和Pull Requests！

### 开发环境设置
1. Fork本项目
2. 克隆到本地: `git clone <repository-url>`
3. 创建功能分支: `git checkout -b feature/your-feature`
4. 提交更改: `git commit -am 'Add your feature'`
5. 推送分支: `git push origin feature/your-feature`
6. 创建Pull Request

### 代码规范
- 使用C++20标准
- 遵循现有代码风格
- 添加适当的注释
- 确保跨平台兼容性

## 许可证

[MIT License](LICENSE)

## 更新日志

详见 [CHANGELOG.md](CHANGELOG.md)

## 致谢

- [SDL](https://www.libsdl.org/) - 跨平台媒体库
- [vcpkg](https://vcpkg.io/) - C++包管理器
- Xbox手柄社区的技术支持