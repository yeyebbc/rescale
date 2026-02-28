#include <windows.h>
#include <cstdint>
#pragma once // Prevents the header from being included multiple times

// --- UNDOCUMENTED WINDOWS API STRUCTURES FOR DPI SCALING ---
// These mirror the definitions used by the Windows Settings UI and
// are required by the implementation in `change-dpi.cpp`.
struct DISPLAYCONFIG_SOURCE_DPI_SCALE_GET
{
    DISPLAYCONFIG_DEVICE_INFO_HEADER header;
    int32_t minScaleRel;
    int32_t curScaleRel;
    int32_t maxScaleRel;
};

struct DISPLAYCONFIG_SOURCE_DPI_SCALE_SET
{
    DISPLAYCONFIG_DEVICE_INFO_HEADER header;
    int32_t scaleRel;
};

bool ChangeDPI(int targetDpiPercent);