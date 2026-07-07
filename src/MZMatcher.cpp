#include "MZMatcher.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <tuple>

namespace mzmatch {

// ═════════════════════════════════════════════════════════════════════════════
//  Public entry point
// ═════════════════════════════════════════════════════════════════════════════

void MZMatcher::run(const std::filesystem::path& lcms_dir,
                     const std::filesystem::path& maldi_dir,
                     const std::filesystem::path& results_dir) {
    // 1. Create results directory if needed.
    std::filesystem::create_directories(results_dir);

    auto raw_dir   = results_dir / "raw";
    auto clean_dir = results_dir / "clean";
    std::filesystem::create_directories(raw_dir);
    std::filesystem::create_directories(clean_dir);

    // 2. Load data.
    std::cout << "\n══ Loading LCMS data ══\n";
    auto lcms_samples = load_lcms_directory(lcms_dir);

    std::cout << "\n══ Loading MALDI data ══\n";
    auto maldi_samples = load_maldi_directory(maldi_dir);

    if (lcms_samples.empty()) {
        std::cerr << "Warning: no LCMS CSV files found in " << lcms_dir << "\n";
        return;
    }
    if (maldi_samples.empty()) {
        std::cerr << "Warning: no MALDI CSV files found in " << maldi_dir << "\n";
        return;
    }

    // 3. Match every LCMS × MALDI pair → one result file per LCMS sample.
    std::cout << "\n══ Matching ══\n";

    for (const auto& lcms : lcms_samples) {
        std::vector<MatchRow> all_matches;

        for (const auto& maldi : maldi_samples) {
            match_pair(lcms, maldi, all_matches);
        }

        // Write the raw results CSV.
        auto raw_path = raw_dir / (lcms.filename + "_matches.csv");
        write_csv(raw_path, all_matches);
        std::cout << "  [RAW]   " << raw_path.filename().string()
                  << "  (" << all_matches.size() << " rows)\n";

        // Deduplicate and write the clean CSV.
        auto clean_rows = deduplicate(all_matches);
        auto clean_path = clean_dir / (lcms.filename + "_matches_clean.csv");
        write_csv(clean_path, clean_rows);
        std::cout << "  [CLEAN] " << clean_path.filename().string()
                  << "  (" << clean_rows.size() << " rows)\n";
    }

    std::cout << "\n══ Done ══\n"
              << "Raw results   → " << std::filesystem::absolute(raw_dir) << "\n"
              << "Clean results → " << std::filesystem::absolute(clean_dir) << "\n\n";
}

// ═════════════════════════════════════════════════════════════════════════════
//  Matching logic
// ═════════════════════════════════════════════════════════════════════════════

void MZMatcher::match_pair(const LCMSData& lcms,
                            const MALDIData& maldi,
                            std::vector<MatchRow>& out) {
    // Compare rounded values.
    for (std::size_t i = 0; i < lcms.mz_rounded.size(); ++i) {
        for (std::size_t j = 0; j < maldi.mz_rounded.size(); ++j) {
            if (lcms.mz_rounded[i] == maldi.mz_rounded[j]) {
                out.push_back({
                    lcms.filename, maldi.filename,
                    lcms.mz_values[i], maldi.mz_values[j],
                    lcms.mz_rounded[i], "rounded"
                });
            }
        }
    }

    // Compare truncated values.
    for (std::size_t i = 0; i < lcms.mz_truncated.size(); ++i) {
        for (std::size_t j = 0; j < maldi.mz_truncated.size(); ++j) {
            if (lcms.mz_truncated[i] == maldi.mz_truncated[j]) {
                out.push_back({
                    lcms.filename, maldi.filename,
                    lcms.mz_values[i], maldi.mz_values[j],
                    lcms.mz_truncated[i], "truncated"
                });
            }
        }
    }
}

// ═════════════════════════════════════════════════════════════════════════════
//  CSV I/O
// ═════════════════════════════════════════════════════════════════════════════

static const char* CSV_HEADER =
    "LCMS_File,MALDI_File,LCMS_MZ,MALDI_MZ,Matched_At,Method";

void MZMatcher::write_csv(const std::filesystem::path& path,
                           const std::vector<MatchRow>& rows) {
    std::ofstream out(path);
    if (!out.is_open())
        throw std::runtime_error("Cannot write to: " + path.string());

    out << CSV_HEADER << "\n";
    out << std::fixed << std::setprecision(6);

    for (const auto& r : rows) {
        out << r.lcms_file  << ","
            << r.maldi_file << ","
            << r.lcms_mz    << ","
            << r.maldi_mz   << ","
            << r.matched_at << ","
            << r.method     << "\n";
    }
}

std::vector<MatchRow> MZMatcher::read_csv(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open results file: " + path.string());

    std::vector<MatchRow> rows;
    std::string line;
    std::getline(file, line);  // skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        MatchRow r;
        std::string token;

        std::getline(ss, r.lcms_file, ',');
        std::getline(ss, r.maldi_file, ',');

        std::getline(ss, token, ','); r.lcms_mz    = std::stod(token);
        std::getline(ss, token, ','); r.maldi_mz   = std::stod(token);
        std::getline(ss, token, ','); r.matched_at  = std::stod(token);
        std::getline(ss, r.method, ',');

        rows.push_back(std::move(r));
    }
    return rows;
}

// ═════════════════════════════════════════════════════════════════════════════
//  Deduplication
// ═════════════════════════════════════════════════════════════════════════════

std::vector<MatchRow> MZMatcher::deduplicate(const std::vector<MatchRow>& rows) {
    // A match is "duplicate" if the same (lcms_file, maldi_file, lcms_mz, maldi_mz)
    // appears more than once (e.g. via both rounded and truncated).
    // We keep the first occurrence.
    using Key = std::tuple<std::string, std::string, double, double>;
    std::set<Key> seen;
    std::vector<MatchRow> clean;
    clean.reserve(rows.size());

    for (const auto& r : rows) {
        Key key{r.lcms_file, r.maldi_file, r.lcms_mz, r.maldi_mz};
        if (seen.insert(key).second) {
            clean.push_back(r);
        }
    }
    return clean;
}

}  // namespace mzmatch
