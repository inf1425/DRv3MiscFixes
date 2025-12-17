#pragma once

#include "pch.h"

#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <iomanip>
#include <mutex>

namespace fs = std::filesystem;

struct LogState {
    std::ofstream file;
    std::mutex mtx;
    std::chrono::time_point<std::chrono::steady_clock> startTime;

    LogState() : startTime(std::chrono::steady_clock::now()) {}
};

inline LogState& getLogState() {
    static LogState state;
    return state;
}

inline void initLogging(const char* filename) {
    if (!filename) return;

    auto& state = getLogState();
    
    std::lock_guard<std::mutex> lock(state.mtx);

    try {
        fs::path logPath(filename);

        if (fs::exists(logPath)) {
            fs::path backupPath = logPath.parent_path() / ("prev-" + logPath.filename().string());

            if (fs::exists(backupPath)) {
                fs::remove(backupPath);
            }
            fs::rename(logPath, backupPath);
        }

        state.file.open(filename);
    }
    catch (const std::exception& e) {
    }
}

template <typename T>
auto safeArg(const T& arg) {
    if constexpr (std::is_pointer_v<T>) {
        if (arg == nullptr) return "(null)";
    }
    return arg;
}

template <typename... Args>
void log(Args... args) {
    auto& state = getLogState();
    
    std::lock_guard<std::mutex> lock(state.mtx);

    if (!state.file.is_open()) {
        return;
    }
        
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - state.startTime);
    
    long long totalMs = duration.count();
    long long hours = totalMs / 3600000;
    totalMs %= 3600000;
    long long minutes = totalMs / 60000;
    totalMs %= 60000;
    long long seconds = totalMs / 1000;
    long long millis = totalMs % 1000;

    state.file << '['
        << std::setfill('0') << std::setw(2) << hours << ':'
        << std::setfill('0') << std::setw(2) << minutes << ':'
        << std::setfill('0') << std::setw(2) << seconds << '.'
        << std::setfill('0') << std::setw(3) << millis
        << "] ";

    (state.file << ... << safeArg(args)) << '\n';
    
    state.file.flush();
}