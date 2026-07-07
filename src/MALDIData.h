#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace mzmatch {

/// Holds the m/z values extracted from a single MALDI-MS CSV file.
struct MALDIData {
    std::string                filename;       // stem of the source file
    std::vector<double>        mz_values;      // raw m/z values
    std::vector<double>        mz_rounded;     // rounded to nearest integer
    std::vector<double>        mz_truncated;   // truncated (floor) to integer

    /// Parse a MALDI CSV file (first column = m/z).
    static MALDIData from_csv(const std::filesystem::path& path);
};

/// Read every .csv inside @p dir and return a vector of MALDIData.
std::vector<MALDIData> load_maldi_directory(const std::filesystem::path& dir);

}  // namespace mzmatch
