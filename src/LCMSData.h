#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace mzmatch {

/// Holds the m/z values extracted from a single LCMS CSV file.
struct LCMSData {
    std::string                filename;       // stem of the source file
    std::vector<double>        mz_values;      // raw m/z values
    std::vector<double>        mz_rounded;     // rounded to nearest integer
    std::vector<double>        mz_truncated;   // truncated (floor) to integer

    /// Parse an LCMS CSV file (first column = m/z).
    /// @param path  Path to a CSV file.
    /// @return Populated LCMSData.
    static LCMSData from_csv(const std::filesystem::path& path);
};

/// Read every .csv inside @p dir and return a vector of LCMSData.
std::vector<LCMSData> load_lcms_directory(const std::filesystem::path& dir);

}  // namespace mzmatch
