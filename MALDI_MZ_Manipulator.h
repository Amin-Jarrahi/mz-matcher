//
// Created by Amin on 8/20/24.
//

#ifndef MALDI_MZ_MANIPULATOR_H
#define MALDI_MZ_MANIPULATOR_H

#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class MALDI_MZManip {

    std::string MALDI_name{};
    std::string MALDI_file_path{};
    std::vector<float> MALDI_mz{};
    std::vector<float> MALDI_max_intensity{};
    std::vector<float> MALDI_rounded_mz_data{};
    std::vector<float> MALDI_truncated_mz_data{};

public:

    void attribute_setter(const std::string&, const std::string&);
    int Mz_list_getter();
    void Mz_list_display() const;
    void Mz_list_rounder();
    void Mz_list_trunkator();
    void Attribute_restarter();

    friend class MZ_Analyzer;

};



#endif //MALDI_MZ_MANIPULATOR_H
