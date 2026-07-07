#pragma once

#include "LCMSData.h"
#include "MALDIData.h"

#include <filesystem>
#include <string>
#include <vector>

namespace mzmatch {

/// A single matched row in the results CSV.
struct MatchRow {
    std::string lcms_file;
    std::string maldi_file;
    double      lcms_mz    = 0.0;
    double      maldi_mz   = 0.0;
    double      matched_at = 0.0;   // the integer value they matched on
    std::string method;             // "rounded" or "truncated"
};

/// Core matching engine. Compares every LCMS sample against every MALDI sample
/// using both rounded and truncated integer m/z values.
class MZMatcher {
public:
    /// Run the full pipeline: match → write raw results → deduplicate → write clean results.
    /// @param lcms_dir     Directory containing LCMS .csv files.
    /// @param maldi_dir    Directory containing MALDI .csv files.
    /// @param results_dir  Directory where result CSVs will be written.
    void run(const std::filesystem::path& lcms_dir,
             const std::filesystem::path& maldi_dir,
             const std::filesystem::path& results_dir);

private:
    // ── Matching ────────────────────────────────────────────────────────────
    /// Compare one LCMS sample against one MALDI sample; append matches.
    static void match_pair(const LCMSData& lcms,
                           const MALDIData& maldi,
                           std::vector<MatchRow>& out);

    // ── CSV I/O ─────────────────────────────────────────────────────────────
    /// Write a vector of MatchRows as a CSV file.
    static void write_csv(const std::filesystem::path& path,
                          const std::vector<MatchRow>& rows);

    /// Read a results CSV back into MatchRows (used by the dedup step).
    static std::vector<MatchRow> read_csv(const std::filesystem::path& path);

    // ── Deduplication ───────────────────────────────────────────────────────
    /// Remove duplicate rows (same LCMS m/z ↔ MALDI m/z match appearing
    /// via both rounding and truncation, or repeated across files).
    static std::vector<MatchRow> deduplicate(const std::vector<MatchRow>& rows);
};

}  // namespace mzmatch
