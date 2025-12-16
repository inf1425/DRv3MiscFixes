#pragma once

#include "pch.h"

#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;
static std::ofstream log_file;

static const auto startTime = std::chrono::steady_clock::now();

void initLogging(const char* filename) {
    fs::path logPath(filename);

    if (fs::exists(logPath)) {
        fs::path backupPath = logPath.parent_path() / ("prev-" + logPath.filename().string());

        if (fs::exists(backupPath)) {
            fs::remove(backupPath);
        }

        fs::rename(logPath, backupPath);
    }

    log_file.open(filename);
}

template <typename... Args>
void log(Args... args) {
    if (log_file.is_open()) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        long long totalMs = duration.count();

        long long hours = totalMs / 3600000;
        totalMs %= 3600000;

        long long minutes = totalMs / 60000;
        totalMs %= 60000;

        long long seconds = totalMs / 1000;
        long long millis = totalMs % 1000;

        log_file << '['
            << std::setfill('0') << std::setw(2) << hours << ':'
            << std::setfill('0') << std::setw(2) << minutes << ':'
            << std::setfill('0') << std::setw(2) << seconds << '.'
            << std::setfill('0') << std::setw(3) << millis
            << "] ";

        (log_file << ... << args) << '\n';
        log_file.flush();
    }
}