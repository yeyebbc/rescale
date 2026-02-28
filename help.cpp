#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

void PrintHelp()
{
    std::cout << "用法：.\Rescale.exe [宽] [高] [DPI 缩放倍率（无需百分比符号）]\n";
    std::cout << "示例：.\Rescale.exe 1920 1080 125\n\n";
    std::cout << "支持的 DPI 缩放倍率：100, 125, 150, 175, 200, 225, 250, 300, 350, 400, 450, 500\n";
}