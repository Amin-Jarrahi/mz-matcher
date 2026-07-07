#include "MALDIData.h"

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
    cell.erase(std::remove(cell.begin(), cell.end(), '"'), cell.end());
    cell.erase(std::remove(cell.begin(), cell.end(), ' '), cell.end());
    return std::stod(cell);
}

// ── MALDIData ───────────────────────────────────────────────────────────────

MALDIData MALDIData::from_csv(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open MALDI file: " + path.string());

    MALDIData data;
    data.filename = path.stem().string();

    std::string line;
    // Skip header
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
            // Skip unparseable rows.
        }
    }
    return data;
}

// ── Directory loader ────────────────────────────────────────────────────────

std::vector<MALDIData> load_maldi_directory(const std::filesystem::path& dir) {
    if (!std::filesystem::is_directory(dir))
        throw std::runtime_error("MALDI directory does not exist: " + dir.string());

    std::vector<std::filesystem::path> csv_paths;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv")
            csv_paths.push_back(entry.path());
    }
    std::sort(csv_paths.begin(), csv_paths.end());

    std::vector<MALDIData> result;
    result.reserve(csv_paths.size());
    for (const auto& p : csv_paths) {
        result.push_back(MALDIData::from_csv(p));
        std::cout << "  [MALDI] Loaded " << p.filename().string()
                  << "  (" << result.back().mz_values.size() << " values)\n";
    }
    return result;
}

}  // namespace mzmatch
