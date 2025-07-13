# Xbox Controller API - 构建指南

跨平台Xbox手柄API，支持鼠标、键盘、媒体控制和语音输入。

## 系统要求

- CMake 3.20+
- C++20兼容编译器
- SDL3开发库

## 平台特定要求

### Windows
- Visual Studio 2019+ 或 MinGW-w64
- Windows 10+ (支持Win+H语音输入)

### Linux
- GCC 9+ 或 Clang 10+
- X11开发库 (`libx11-dev`, `libxtst-dev`)
- PulseAudio开发库 (`libpulse-dev`)
- PlayerCtl (`playerctl`)

### macOS
- Xcode 12+ 或 Clang 12+
- macOS 10.15+
- Xcode Command Line Tools

---

## Windows构建 (推荐使用vcpkg)

### 1. 安装vcpkg

```bash
# 克隆vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# 引导vcpkg
.\bootstrap-vcpkg.bat

# 集成到Visual Studio (可选)
.\vcpkg integrate install
```

### 2. 安装依赖库

```bash
# 安装SDL3
.\vcpkg install sdl3:x64-windows

# 或安装静态库版本
.\vcpkg install sdl3:x64-windows-static
```

### 3. 构建项目

```bash
# 克隆项目
git clone <project-url>
cd xbox-controller-api

# 创建构建目录
mkdir build
cd build

# 配置CMake (使用vcpkg工具链)
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows

# 编译
cmake --build . --config Release
```

### 4. Visual Studio构建

```bash
# 生成Visual Studio解决方案
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake

# 打开生成的.sln文件进行编译
```

---

## Linux构建

### 1. 安装系统依赖

#### Ubuntu/Debian:
```bash
sudo apt update
sudo apt install build-essential cmake git
sudo apt install libx11-dev libxtst-dev libpulse-dev
sudo apt install playerctl

# SDL3 (从源码编译或使用包管理器)
# 方法1: 使用包管理器 (如果可用)
sudo apt install libsdl3-dev

# 方法2: 从源码编译SDL3
git clone https://github.com/libsdl-org/SDL.git -b SDL3
cd SDL
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

#### Fedora/RHEL:
```bash
sudo dnf install gcc-c++ cmake git
sudo dnf install libX11-devel libXtst-devel pulseaudio-libs-devel
sudo dnf install playerctl

# SDL3安装
sudo dnf install SDL3-devel  # 如果可用
# 或从源码编译 (参考上面Ubuntu部分)
```

#### Arch Linux:
```bash
sudo pacman -S base-devel cmake git
sudo pacman -S libx11 libxtst libpulse
sudo pacman -S playerctl

# SDL3
yay -S sdl3  # 使用AUR
# 或从源码编译
```

### 2. 构建项目

```bash
# 克隆项目
git clone <project-url>
cd xbox-controller-api

# 创建构建目录
mkdir build && cd build

# 配置和编译
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# 安装 (可选)
sudo make install
```

### 3. 运行时权限

```bash
# 确保用户在input组中 (访问手柄设备)
sudo usermod -a -G input $USER

# 重新登录使组变更生效
```

---

## macOS构建

### 1. 安装依赖

#### 使用Homebrew (推荐):
```bash
# 安装Homebrew (如果未安装)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 安装依赖
brew install cmake git
brew install sdl3

# 安装Xcode Command Line Tools
xcode-select --install
```

#### 使用vcpkg:
```bash
# 克隆vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# 引导vcpkg
./bootstrap-vcpkg.sh

# 安装SDL3
./vcpkg install sdl3:x64-osx
```

### 2. 构建项目

#### 使用Homebrew依赖:
```bash
# 克隆项目
git clone <project-url>
cd xbox-controller-api

# 创建构建目录
mkdir build && cd build

# 配置和编译
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

#### 使用vcpkg依赖:
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-osx
make -j$(sysctl -n hw.ncpu)
```

### 3. 应用程序包 (可选)

```bash
# 创建.app包
mkdir -p XboxControllerAPI.app/Contents/MacOS
cp xbox_controller_api XboxControllerAPI.app/Contents/MacOS/

# 创建Info.plist
cat > XboxControllerAPI.app/Contents/Info.plist << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>xbox_controller_api</string>
    <key>CFBundleIdentifier</key>
    <string>com.example.xbox-controller-api</string>
    <key>CFBundleName</key>
    <string>Xbox Controller API</string>
    <key>CFBundleVersion</key>
    <string>1.0.0</string>
</dict>
</plist>
EOF
```

---

## 交叉编译

### Windows上为Linux交叉编译
```bash
# 使用MinGW-w64交叉编译工具链
sudo apt install gcc-mingw-w64

# 配置CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE=cmake/linux-mingw.cmake
```

### Linux上为Windows交叉编译
```bash
# 使用MinGW-w64
sudo apt install mingw-w64

# 配置CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE=cmake/windows-mingw.cmake
```

---

## 故障排除

### SDL3找不到
```bash
# 设置SDL3路径
export SDL3_DIR=/usr/local/lib/cmake/SDL3
cmake .. -DSDL3_DIR=$SDL3_DIR
```

### X11库找不到 (Linux)
```bash
# 安装X11开发库
sudo apt install libx11-dev libxtst-dev

# 或指定路径
cmake .. -DX11_INCLUDE_DIR=/usr/include/X11
```

### 权限问题
```bash
# Linux: 手柄访问权限
sudo usermod -a -G input $USER

# macOS: 输入监控权限
# 系统偏好设置 -> 安全性与隐私 -> 隐私 -> 输入监控
```

### 媒体控制不工作
```bash
# Linux: 安装playerctl
sudo apt install playerctl

# macOS: 确保音乐应用正在运行
# Windows: 确保有媒体播放器在运行
```

---

## 构建选项

### CMake选项
```bash
# 静态链接
cmake .. -DBUILD_STATIC=ON

# 调试构建
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 安装路径
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local

# 禁用特定功能
cmake .. -DENABLE_MEDIA_CONTROL=OFF -DENABLE_VOICE_INPUT=OFF
```

### 编译器优化
```bash
# Release构建 (推荐)
cmake .. -DCMAKE_BUILD_TYPE=Release

# 性能优化
cmake .. -DCMAKE_CXX_FLAGS="-O3 -march=native"

# 调试信息
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

---

## CI/CD构建

参考 `.github/workflows/` 目录下的配置文件:

- `windows.yml` - Windows构建 (MSVC + vcpkg)
- `linux.yml` - Linux构建 (GCC + apt)
- `macos.yml` - macOS构建 (Clang + Homebrew)
- `release.yml` - 自动发布工作流

---

## 发布包

### 创建发布包
```bash
# 运行打包脚本
./scripts/package.sh

# 或手动打包
cmake --build . --target package
```

### 包内容
- `xbox-controller-api-{version}-{platform}.zip`
- 可执行文件
- 必需的动态库
- 文档和许可证
- 配置文件示例