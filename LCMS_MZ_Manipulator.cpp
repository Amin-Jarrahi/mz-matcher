//
// Created by Amin on 8/20/24.
//

#include "LCMS_MZ_Manipulator.h"
#include <iostream>


void  LCMS_MzManip::attribute_setter(const std::string& path, const std::string& name)
{
    this->LCMS_name = name;
    LCMS_file_path=path;
    std::cout<<path <<std::endl;
}

int LCMS_MzManip::Mz_list_getter( )
{
    std::ifstream inputFile(LCMS_file_path);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open the file " << LCMS_file_path << std::endl;
        return 1;
    }
    std::string line{}, cell{};
    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }
        std::stringstream lineStream(line);
        int k = 0;
        while (std::getline(lineStream, cell, ',')) {

            if(k==0)
            {
                LCMS_mzname.push_back(cell);
            } else if(k==1)
            {
                LCMS_mz.push_back(std::stof(cell));
            } else if(k==2)
            {
                LCMS_fold_change.push_back(std::stof(cell));
            } else if(k==3)
            {
                LCMS_p_value.push_back(std::stof(cell));
            }
            k++;
        }
    }

    inputFile.close();
    return 0;
}

void LCMS_MzManip::Mz_list_rounder( )
{
    LCMS_rounded_mz_data = LCMS_mz;
    for(float &each_mz: LCMS_rounded_mz_data)
    {
        each_mz = std::round(each_mz * 100.0f) / 100.0f;
    }
}

void LCMS_MzManip::Mz_list_trunkator( )
{
    LCMS_truncated_mz_data = LCMS_mz;
    for(float &each_mz: LCMS_truncated_mz_data)
    {
        each_mz = std::truncf(each_mz * 100.0f) / 100.0f;
    }
}

void LCMS_MzManip::Mz_list_display() const
{
    std::cout << std::setfill ('=') << std::setw (150) <<"="<< std::endl;
    std::cout << std::setfill (' ');
    std::cout << std::setw (72) << std::right << LCMS_name << std::endl;
    std::cout << std::setfill (' ') << std::right << std::setw(15) << "Lipid Name"
    <<  std::setw(85) << "m/z" << std::setw(14) << "Rounded" << std::setw(12) << "Truncated"
    <<  std::setw(12) << "Fold Change" << std::setw(8) << "P-value" << std::endl;
    std::cout << std::left << std::setfill ('=') << std::setw (149) <<"=";
    std::cout << std::setfill (' ') << "=" << std::endl;
    std::cout <<"   ";
    for(int i =0; i<LCMS_mzname.size(); i++)
    {
        std::cout << "| " << std::setw(90) << LCMS_mzname[i]
        <<"| " << std::setprecision(4) << std::fixed << std::setw(12) << LCMS_mz[i]
        <<"| " << std::setprecision(3) << std::fixed << std::setw(9) << LCMS_rounded_mz_data[i]
        <<"| " << std::setprecision(3) << std::fixed << std::setw(9) << LCMS_truncated_mz_data[i]
        <<"| " << std::setprecision(2) << std::fixed << std::setw(7) << LCMS_fold_change[i]
        <<"| " << std::setprecision(4) << std::fixed << std::setw(8) << LCMS_p_value[i] << "|"<< std::endl;
    }
}

void LCMS_MzManip::Attribute_restarter()
{
    LCMS_name.clear();
    LCMS_file_path.clear();
    LCMS_mzname.clear();
    LCMS_mz.clear();
    LCMS_fold_change.clear();
    LCMS_p_value.clear();
    LCMS_rounded_mz_data.clear();
    LCMS_truncated_mz_data.clear();
}

