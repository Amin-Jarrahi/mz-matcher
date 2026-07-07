#include "LCMSData.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace mzmatch {

// ── helpers ─────────────────────────────────────────────────────────────────

static double parse_first_column(const std::string& line) {
    std::istringstream ss(line);
    std::string cell;
    std::getline(ss, cell, ',');
    // Remove any surrounding whitespace / quotes
    cell.erase(std::remove(cell.begin(), cell.end(), '"'), cell.end());
    cell.erase(std::remove(cell.begin(), cell.end(), ' '), cell.end());
    return std::stod(cell);
}

// ── LCMSData ────────────────────────────────────────────────────────────────

LCMSData LCMSData::from_csv(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open LCMS file: " + path.string());

    LCMSData data;
    data.filename = path.stem().string();

    std::string line;
    // Skip the first line (header).
    if (!std::getline(file, line))
        return data;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            double mz = parse_first_column(line);
            data.mz_values.push_back(mz);
            data.mz_rounded.push_back(std::round(mz));
            data.mz_truncated.push_back(std::floor(mz));
        } catch (...) {
            // Skip unparseable rows silently.
        }
    }
    return data;
}

// ── Directory loader ────────────────────────────────────────────────────────

std::vector<LCMSData> load_lcms_directory(const std::filesystem::path& dir) {
    if (!std::filesystem::is_directory(dir))
        throw std::runtime_error("LCMS directory does not exist: " + dir.string());

    std::vector<std::filesystem::path> csv_paths;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv")
            csv_paths.push_back(entry.path());
    }
    std::sort(csv_paths.begin(), csv_paths.end());

    std::vector<LCMSData> result;
    result.reserve(csv_paths.size());
    for (const auto& p : csv_paths) {
        result.push_back(LCMSData::from_csv(p));
        std::cout << "  [LCMS] Loaded " << p.filename().string()
                  << "  (" << result.back().mz_values.size() << " values)\n";
    }
    return result;
}

}  // namespace mzmatch
