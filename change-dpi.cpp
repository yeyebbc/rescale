#include "change-dpi.h"
#include <iostream>
#include <vector>

// Windows 支持的 DPI 缩放倍率
static const int DPI_VALS[] = {100, 125, 150, 175, 200, 225, 250, 300, 350, 400, 450, 500};
static const int DPI_VALS_COUNT = sizeof(DPI_VALS) / sizeof(DPI_VALS[0]);

bool ChangeDPI(int targetDpiPercent)
{
    // 1. 看看给的 DPI 缩放倍率是否在列表里
    int targetIndex = -1;
    for (int i = 0; i < DPI_VALS_COUNT; ++i)
    {
        if (DPI_VALS[i] == targetDpiPercent)
        {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1)
    {
        std::cerr << "[!] DPI 缩放倍率不对: " << targetDpiPercent << "%\n";
        return false;
    }

    // 2. 获取活跃中的监视器路径
    UINT32 numPathArrayElements = 0;
    UINT32 numModeInfoArrayElements = 0;
    if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &numPathArrayElements, &numModeInfoArrayElements) != ERROR_SUCCESS)
    {
        std::cerr << "[!] 无法获取显示配置缓冲区大小. \n";
        return false;
    }
    std::vector<DISPLAYCONFIG_PATH_INFO> pathInfo(numPathArrayElements);
    std::vector<DISPLAYCONFIG_MODE_INFO> modeInfo(numModeInfoArrayElements);
    if (QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &numPathArrayElements, pathInfo.data(), &numModeInfoArrayElements, modeInfo.data(), nullptr) != ERROR_SUCCESS)
    {
        std::cerr << "[!] 无法获取显示配置信息. \n";
        return false;
    }
    bool success = false;

    // 3. 在路径中循环对比以应用 DPI 缩放倍率
    for (UINT32 i = 0; i < numPathArrayElements; ++i)
    {
        DISPLAYCONFIG_SOURCE_DPI_SCALE_GET getPacket = {};
        getPacket.header.type = (DISPLAYCONFIG_DEVICE_INFO_TYPE)-3; // Undocumented GET
        getPacket.header.size = sizeof(getPacket);
        getPacket.header.adapterId = pathInfo[i].sourceInfo.adapterId;
        getPacket.header.id = pathInfo[i].sourceInfo.id;

        if (DisplayConfigGetDeviceInfo(&getPacket.header) != ERROR_SUCCESS)
        {
            continue;
        }

        // 计算相对缩放倍率
        // minScaleRel is negative (例如，-3 表示 100% 是比系统推荐值小 3 个挡位的)
        // 用这种办法来和系统的固定缩放倍率对齐
        int32_t targetScaleRel = targetIndex + getPacket.minScaleRel;

        if (targetScaleRel < getPacket.minScaleRel || targetScaleRel > getPacket.maxScaleRel)
        {
            std::cerr << "[!] Target DPI " << targetDpiPercent << "% is outside the supported range for monitor " << i << ".\n";
            continue;
        }

        DISPLAYCONFIG_SOURCE_DPI_SCALE_SET setPacket = {};
        setPacket.header.type = (DISPLAYCONFIG_DEVICE_INFO_TYPE)-4; // Undocumented SET
        setPacket.header.size = sizeof(setPacket);
        setPacket.header.adapterId = pathInfo[i].sourceInfo.adapterId;
        setPacket.header.id = pathInfo[i].sourceInfo.id;
        setPacket.scaleRel = targetScaleRel;

        if (DisplayConfigSetDeviceInfo(&setPacket.header) == ERROR_SUCCESS)
        {
            std::cout << "[+] DPI successfully changed to " << targetDpiPercent << "% for active monitor path " << i << ".\n";
            success = true;
        }
        else
        {
            std::cerr << "[!] Failed to set DPI for monitor path " << i << ".\n";
        }
    }

    return success;
}