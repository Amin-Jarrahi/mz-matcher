# mz-matcher

**LCMS / MALDI-MS m/z value matching and deduplication tool.**

> **Author:** Alex(Amin) Jarrahi

`mz-matcher` reads m/z values from LCMS and MALDI-MS CSV files, identifies matches between them using both **rounding** and **truncation** to the nearest integer, and produces deduplicated result files — all in a single command.

---

## Table of Contents

- [Features](#features)
- [Quick Start](#quick-start)
- [Building from Source](#building-from-source)
- [Usage](#usage)
- [Input Format](#input-format)
- [Output](#output)
- [Project Structure](#project-structure)
- [How It Works](#how-it-works)
- [Contributing](#contributing)
- [License](#license)

---

## Features

| Feature | Description |
|---|---|
| **Dual matching** | Matches m/z values by both rounding (`std::round`) and truncation (`std::floor`). |
| **Automatic deduplication** | Removes duplicate matches where the same LCMS↔MALDI pair was found by both methods. |
| **Batch processing** | Processes entire directories of CSV files — every LCMS file is compared against every MALDI file. |
| **No hardcoded paths** | All paths are provided as command-line arguments. |
| **Clean + raw output** | Writes both raw match files and deduplicated versions. |
| **Cross-platform** | Standard C++20, builds on Linux, macOS, and Windows. |

---

## Quick Start

```bash
# Clone
git clone https://github.com/Amin-Jarrahi/mz-matcher.git
cd mz-matcher

# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run
./build/mz-matcher data/LCMS data/MALDI_MS results
```

---

## Building from Source

### Prerequisites

- A C++20 compatible compiler:
  - GCC ≥ 10
  - Clang ≥ 13
  - MSVC ≥ 19.29 (Visual Studio 2019 16.10+)
- CMake ≥ 3.20

### Build Steps

```bash
# Configure (Release mode recommended for large datasets)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# (Optional) Install system-wide
sudo cmake --install build
```

### Windows (Visual Studio)

```powershell
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\mz-matcher.exe data\LCMS data\MALDI_MS results
```

---

## Usage

```
mz-matcher <lcms_dir> <maldi_dir> [results_dir]
```

| Argument | Required | Description |
|---|---|---|
| `lcms_dir` | ✅ | Directory containing LCMS `.csv` files |
| `maldi_dir` | ✅ | Directory containing MALDI-MS `.csv` files |
| `results_dir` | ❌ | Output directory (default: `./results`) |

### Examples

```bash
# Minimal — results go to ./results/
mz-matcher data/LCMS data/MALDI_MS

# Custom output directory
mz-matcher data/LCMS data/MALDI_MS my_experiment_results

# Relative paths work too
mz-matcher ../shared_data/lcms ../shared_data/maldi ./output
```

---

## Input Format

Each input CSV file should have:

1. **A header row** (skipped automatically).
2. **m/z values in the first column.**
3. Additional columns are ignored.

### Example LCMS CSV

```csv
m/z,intensity,rt
123.456,50000,12.3
234.567,30000,15.7
345.678,80000,18.2
```

### Example MALDI CSV

```csv
m/z,intensity
123.8,42000
234.1,15000
500.9,60000
```

---

## Output

Results are written to the specified output directory with this structure:

```
results/
├── raw/                          # All matches (may contain duplicates)
│   ├── sample1_matches.csv
│   └── sample2_matches.csv
└── clean/                        # Deduplicated matches
    ├── sample1_matches_clean.csv
    └── sample2_matches_clean.csv
```

### Output CSV Columns

| Column | Description |
|---|---|
| `LCMS_File` | Name of the source LCMS file |
| `MALDI_File` | Name of the source MALDI file |
| `LCMS_MZ` | Original LCMS m/z value |
| `MALDI_MZ` | Original MALDI m/z value |
| `Matched_At` | The integer value they matched on |
| `Method` | `"rounded"` or `"truncated"` |

---

## Project Structure

```
mz-matcher/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── LICENSE                  # MIT License
├── .gitignore              # Git ignore rules
├── src/
│   ├── main.cpp            # CLI entry point
│   ├── LCMSData.h/.cpp     # LCMS CSV parser
│   ├── MALDIData.h/.cpp    # MALDI CSV parser
│   └── MZMatcher.h/.cpp    # Matching engine + deduplication + CSV writer
└── data/                   # Sample data (not tracked — add your own)
    ├── LCMS/
    └── MALDI_MS/
```

---

## How It Works

```
┌─────────────┐     ┌──────────────┐
│  LCMS CSVs  │     │  MALDI CSVs  │
└──────┬──────┘     └──────┬───────┘
       │                   │
       ▼                   ▼
   ┌───────────────────────────┐
   │     Load & Parse m/z      │
   │  (skip header, col 1)     │
   └─────────────┬─────────────┘
                 │
          ┌──────┴──────┐
          ▼             ▼
   ┌────────────┐ ┌────────────┐
   │  Round to  │ │ Truncate   │
   │  nearest   │ │ (floor)    │
   │  integer   │ │ to integer │
   └─────┬──────┘ └──────┬─────┘
         │               │
         └───────┬───────┘
                 ▼
   ┌─────────────────────────────┐
   │  Compare every LCMS value   │
   │  against every MALDI value  │
   │  (N × M per pair)           │
   └─────────────┬───────────────┘
                 │
                 ▼
   ┌─────────────────────────────┐
   │  Write raw results CSV      │
   └─────────────┬───────────────┘
                 │
                 ▼
   ┌─────────────────────────────┐
   │  Deduplicate                │
   │  (same LCMS↔MALDI m/z      │
   │   found by both methods)    │
   └─────────────┬───────────────┘
                 │
                 ▼
   ┌─────────────────────────────┐
   │  Write clean results CSV    │
   └─────────────────────────────┘
```

---

## Contributing

Contributions are welcome! Please:

1. Fork the repository.
2. Create a feature branch: `git checkout -b feature/my-improvement`
3. Commit your changes: `git commit -m "Add my improvement"`
4. Push to the branch: `git push origin feature/my-improvement`
5. Open a Pull Request.

### Ideas for Contributions

- [ ] Tolerance-based matching (e.g., ±0.5 Da or ppm-based)
- [ ] Support for TSV / tab-delimited input
- [ ] JSON or HTML report output
- [ ] Unit tests (Google Test / Catch2)
- [ ] Python bindings

---

## License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

© 2024–2026 Alex(Amin) Jarrahi
