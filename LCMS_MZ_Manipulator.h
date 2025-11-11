//
// Created by Amin on 8/20/24.
//

#ifndef LCMS_MZ_MANIPULATOR_H
#define LCMS_MZ_MANIPULATOR_H
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;


class LCMS_MzManip
{
    std::string LCMS_name{};
    std::string LCMS_file_path{};
    std::vector<std::string> LCMS_mzname{};
    std::vector<float> LCMS_mz{};
    std::vector<float> LCMS_fold_change{};
    std::vector<float> LCMS_p_value{};
    std::vector<float> LCMS_rounded_mz_data{};
    std::vector<float> LCMS_truncated_mz_data{};
public:

    void attribute_setter(const std::string&, const std::string&);
    int Mz_list_getter();
    void Mz_list_rounder( );
    void Mz_list_trunkator( );
    void Mz_list_display() const;
    void Attribute_restarter();

    friend class MZ_Analyzer;
};


#endif //LCMS_MZ_MANIPULATOR_H
