#include "pch.h"

#include <iostream>
#include <thread>
#include <cstdint>

#include <Psapi.h>
#include <timeapi.h>

#include "Hooking.hpp"
#include "logging.hpp"

#pragma comment(lib, "winmm.lib")

class FrameLimiter {
private:
    LARGE_INTEGER ticksPerSecond;
    LARGE_INTEGER nextFrameStartTime;
    double targetFrameTimeSeconds;
    long long maxSleepMs;

public:
    FrameLimiter() {
        QueryPerformanceFrequency(&ticksPerSecond);
        QueryPerformanceCounter(&nextFrameStartTime);
        targetFrameTimeSeconds = 1.0 / 60.0;
        maxSleepMs = ceil(targetFrameTimeSeconds * 1000);
    }

    void WaitForNextFrame() {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        long long ticksToWait = nextFrameStartTime.QuadPart - currentTime.QuadPart;
        long long microsecondsToWait = (ticksToWait * 1000000) / ticksPerSecond.QuadPart;

        DWORD millisecondsToWait = max(0, min(microsecondsToWait / 1000, maxSleepMs)); // clamping to a reasonable about

        timeBeginPeriod(1);
        Sleep(millisecondsToWait);
        timeEndPeriod(1);

        do {
            QueryPerformanceCounter(&currentTime);
            YieldProcessor();
        } while (currentTime.QuadPart < nextFrameStartTime.QuadPart);

        
        long long targetIntervalTicks = (long long)(targetFrameTimeSeconds * ticksPerSecond.QuadPart);
        if (currentTime.QuadPart > nextFrameStartTime.QuadPart + targetIntervalTicks) {
            nextFrameStartTime.QuadPart = currentTime.QuadPart;
        }
        nextFrameStartTime.QuadPart += targetIntervalTicks;
    }
};

void waitForNextRenderFrame(uint32_t numFramesToWait) {
    static FrameLimiter instanceRender;

    if (numFramesToWait > 3) {
        numFramesToWait = 3;
    }

    for (uint32_t i = 0; i < numFramesToWait; i++) {
        instanceRender.WaitForNextFrame();
    }
}

bool patchRenderThreadFpsLimiter(void* gameMemory, size_t gameMemoryLen) {
    const char* signature = "\xf2\x0f\x5e\xc7\x66\x0f\x5a\xc0\xf3\x0f\x2c\xc0\x3b\xc6\x7d\x1c\xff\x15\x6b\xff\x09\x00\xeb\xb1\x48\x8b\x07\x44\x8b\x05\x27\x64\x6b\x00\x33\xd2\x48\x8b\xcf\xff\x50\x40\x8b\xf8";
    const char* mask = "............................................";

    void (*waitRendfuncPtr)(uint32_t) = waitForNextRenderFrame;
    uint8_t patch[] = {
        0x89, 0xf1, // mov ecx, esi (for the no of frames to wait)
        0x48, 0xb8, // movabs rax, imm64
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // placeholder for address
        0xff, 0xd0  // call rax
    };
    memcpy(&patch[4], &waitRendfuncPtr, 8);

    return CodeHook(gameMemory, gameMemoryLen, signature, mask, std::vector<uint8_t>(patch, patch + sizeof(patch)));
}

void waitForNextUpdateFrame() {
    static FrameLimiter instanceUpdate;
    instanceUpdate.WaitForNextFrame();
}

bool patchUpdateThreadFpsLimiter(void* gameMemory, size_t gameMemoryLen) {
    const char* signature = "\xf2\x41\x0f\x5e\xc0\x66\x0f\x5a\xd0\x0f\x28\xc2\xf3\x0f\x58\x83\x08\x1e\x00\x00\x0f\x2f\xc7\x73\x08\xff\x15\xd8\x19\x27\x00\xeb\x8e";
    const char* mask = ".................................";

    void (*waitUpdtfuncPtr)() = waitForNextUpdateFrame;
    uint8_t patch[] = {
        0x48, 0xb8, // movabs rax, imm64
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // placeholder for address
        0xff, 0xd0 // call rax
    };
    memcpy(&patch[2], &waitUpdtfuncPtr, 8);

    return CodeHook(gameMemory, gameMemoryLen, signature, mask, std::vector<uint8_t>(patch, patch + sizeof(patch)));
}

DWORD WINAPI WorkerThreadWrapper(LPVOID lpParam) {
    HMODULE hExeModule = GetModuleHandle(NULL);
    if (hExeModule == NULL) {
        return 1;
    }

    MODULEINFO moduleInfo;
    if (!GetModuleInformation(GetCurrentProcess(), hExeModule, &moduleInfo, sizeof(moduleInfo))) {
        return 1;
    }

    void* gameMemory = moduleInfo.lpBaseOfDll;
    size_t gameMemoryLen = moduleInfo.SizeOfImage;

    if (!patchRenderThreadFpsLimiter(gameMemory, gameMemoryLen)) {
        log("Failed to patch render thread FPS limiter.");
    } else {
        log("Patched render thread FPS limiter.");
    }

    if (!patchUpdateThreadFpsLimiter(gameMemory, gameMemoryLen)) {
        log("Failed to patch update thread FPS limiter.");
    }
    else {
        log("Patched update thread FPS limiter.");
    }

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        initLogging("./drv3miscfixes.log");
        CreateThread(nullptr, 0, WorkerThreadWrapper, nullptr, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

