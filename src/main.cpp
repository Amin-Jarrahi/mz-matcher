#include "MZMatcher.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

// ═════════════════════════════════════════════════════════════════════════════
//  Usage
// ═════════════════════════════════════════════════════════════════════════════

static void print_usage(const char* prog) {
    std::cout
        << "\n"
        << "╔═══════════════════════════════════════════════════════════════╗\n"
        << "║                       mz-matcher v2.0                       ║\n"
        << "║   LCMS / MALDI m/z matching and deduplication tool          ║\n"
        << "║   Author: Alex(Amin) Jarrahi                                ║\n"
        << "╚═══════════════════════════════════════════════════════════════╝\n"
        << "\n"
        << "Usage:\n"
        << "  " << prog << " <lcms_dir> <maldi_dir> [results_dir]\n"
        << "\n"
        << "Arguments:\n"
        << "  lcms_dir      Directory containing LCMS .csv files\n"
        << "  maldi_dir     Directory containing MALDI-MS .csv files\n"
        << "  results_dir   Output directory (default: ./results)\n"
        << "\n"
        << "Examples:\n"
        << "  " << prog << " data/LCMS data/MALDI_MS\n"
        << "  " << prog << " data/LCMS data/MALDI_MS my_results\n"
        << "\n"
        << "CSV format expected:\n"
        << "  - First column contains m/z values.\n"
        << "  - First row is treated as a header and skipped.\n"
        << "  - Additional columns are ignored.\n"
        << "\n";
}

// ═════════════════════════════════════════════════════════════════════════════
//  Main
// ═════════════════════════════════════════════════════════════════════════════

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    fs::path lcms_dir    = argv[1];
    fs::path maldi_dir   = argv[2];
    fs::path results_dir = (argc >= 4) ? fs::path(argv[3]) : fs::path("results");

    // Validate inputs.
    if (!fs::is_directory(lcms_dir)) {
        std::cerr << "Error: LCMS directory not found: " << lcms_dir << "\n";
        return EXIT_FAILURE;
    }
    if (!fs::is_directory(maldi_dir)) {
        std::cerr << "Error: MALDI directory not found: " << maldi_dir << "\n";
        return EXIT_FAILURE;
    }

    try {
        mzmatch::MZMatcher matcher;
        matcher.run(lcms_dir, maldi_dir, results_dir);
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
