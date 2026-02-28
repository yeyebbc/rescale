# Rescale

一个简单的命令行应用程式，用来快速切换主监视器分辨率、DPI 缩放.  
绝大部分代码都是 vibe 的.

用法: `.\Rescale.exe [宽] [高] [DPI 缩放倍率（无需百分比符号）]`.  
示例: `.\Rescale.exe 1920 1080 125` 意思是设定为 1920x1080 125% 缩放.  
支持的 DPI 缩放倍率: `100, 125, 150, 175, 200, 225, 250, 300, 350, 400, 450, 500`.

## 使用场景

我只有一个电脑，但是有多个位于不同地方的监视器，我通过 Moonlight 来串流使用这台电脑，因此需要频繁切换分辨率和 DPI 缩放.

推荐添加到 PATH 环境变量中，以快速运行它.

## 编译

**前置需求**:

- Microsoft Visual C++ (MSVC) 编译器工具包，参考 https://code.visualstudio.com/docs/cpp/config-msvc
- Windows SDK（最低限度不知道，我用的 Windows 11 SDK 10.0.22621.0）

1. 克隆本仓库
2. 确认前置需求已准备
3. 运行 `build-cl.bat`，它会自动在常见安装位置里找 MSVC 的位置

# English

**Rescale** is a simple console program to quickly change resolution and DPI scaling of main display.  
Most codes are vibed.

Usage: `.\Rescale.exe [width] [height] [DPI scaling number]`  
Example: `.\Rescale.exe 1920 1080 125` means Change to 1920x1080 with 125% DPI scaling.  
Supported DPI scaling: `100, 125, 150, 175, 200, 225, 250, 300, 350, 400, 450, 500`

## Usage Scenario

I have only one computer but multiple monitors spreaded across rooms. Moonlight is used to stream the computer to different monitors. Therefore frequent changes to resolution and DPI is needed.

Adding it to PATH environment variable is recommended for quick access.

## Build

**Requirements**:

- Microsoft Visual C++ (MSVC) compiler toolset, check https://code.visualstudio.com/docs/cpp/config-msvc
- Windows SDK (Minimum version requirement unknow, I use Windows 11 SDK 10.0.22621.0)

1. Clone this repository.
2. Make sure requirements are met.
3. Run `build-cl.bat`, it should automatically find your MSVC in common installation paths then build it.
