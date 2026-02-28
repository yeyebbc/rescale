#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include "help.h"
#include "change-dpi.h"

// 自动链接 User32.lib
#pragma comment(lib, "User32.lib")

// 设定分辨率函数，接收 宽、高 参数
bool ChangeResolution(int width, int height)
{
    DEVMODE devMode = {0};
    devMode.dmSize = sizeof(DEVMODE);

    // 获取当前监视器设定
    if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
    {
        std::cerr << "[!] Failed to enumerate display settings.\n";
        return false;
    }

    devMode.dmPelsWidth = width;
    devMode.dmPelsHeight = height;
    devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

    // 设定分辨率
    LONG result = ChangeDisplaySettingsEx(NULL, &devMode, NULL, CDS_UPDATEREGISTRY, NULL);

    if (result == DISP_CHANGE_SUCCESSFUL)
    {
        std::cout << "[+] Resolution successfully changed to " << width << "x" << height << ".\n";
        return true;
    }
    else
    {
        std::cerr << "[!] Failed to change resolution. Error code: " << result << "\n";
        return false;
    }
}

int main(int argc, char *argv[])
{
    // 初始值
    int width = 0, height = 0, targetDpi = 0;
    bool hasDpi = false;

    try
    {
        width = std::stoi(argv[1]);
        height = std::stoi(argv[2]);
        if (argc == 4)
        {
            targetDpi = std::stoi(argv[3]);
            hasDpi = true;
        }
    }
    catch (...)
    {
        std::cerr << "[!] 参数不对. \n\n";
        PrintHelp();
        std::cout << "\n回车以退出...";
        std::cin.get();
        return 1;
    }

    // 1. 设定分辨率
    std::cout << "[*] 正在试图设定分辨率为: " << width << "x" << height << "...\n";
    ChangeResolution(width, height);

    // 2. 设定 DPI 缩放（如果有提供）
    if (hasDpi)
    {
        std::cout << "[*] 正在试图设定 DPI 缩放为: " << targetDpi << "%...\n";
        ChangeDPI(targetDpi);
    }

    return 0;
}