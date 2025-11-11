//
// Created by Amin on 8/20/24.
//

#include "MALDI_MZ_Manipulator.h"
#include <iostream>


void  MALDI_MZManip::attribute_setter(const std::string& path, const std::string& name)
{
    this->MALDI_name = name;
    MALDI_file_path=path;
    std::cout<<path <<std::endl;
}

int MALDI_MZManip::Mz_list_getter( )
{
    std::ifstream inputFile(MALDI_file_path);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open the file " << MALDI_file_path << std::endl;
        return 1;
    }
    std::string line{}, cell{};
    std::vector<float> mz_list {};
    std::vector<float> max_intensity_list{};

    int line_number = 0;
    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }
        if (line_number == 0) {
            line_number++;
            continue; // Skip first line
        }

        std::stringstream lineStream(line);
        int k = 0;
        while (std::getline(lineStream, cell, ',')) {

            if(k==0)
            {
                MALDI_mz.push_back(std::stof(cell));
            } else if(k==1)
            {
                MALDI_max_intensity.push_back(std::stof(cell));
            }
            k++;
        }
    }

    inputFile.close();
    return 0;
}

void MALDI_MZManip::Mz_list_rounder( )
{
    MALDI_rounded_mz_data = MALDI_mz;
    for(float &each_mz: MALDI_rounded_mz_data)
    {
        each_mz = std::round(each_mz * 100.0f) / 100.0f;
    }
}

void MALDI_MZManip::Mz_list_trunkator( )
{
    MALDI_truncated_mz_data = MALDI_mz;
    for(float &each_mz: MALDI_truncated_mz_data)
    {
        each_mz = std::truncf(each_mz * 100.0f) / 100.0f;
    }
}

void MALDI_MZManip::Mz_list_display() const
{
    std::cout << std::setfill ('=') << std::setw (80) <<"="<< std::endl;
    std::cout << std::setfill (' ');
    std::cout << std::setw (38) << std::right << MALDI_name << std::endl;
    std::cout << std::setfill (' ') << std::right << std::setw(7) << "m/z"
    << std::setw(22) << "Rounded" << std::setw(19) << "Truncated"
    << std::setw(25) << "Maximum Intensity" << std::endl;
    std::cout << std::left << std::setfill ('=') << std::setw (79) <<"=";
    std::cout << std::setfill (' ') << "=" << std::endl;
    std::cout <<"   ";
    for(int i =0; i<MALDI_mz.size(); i++)
    {
        std::cout <<"| " << std::setprecision(4) << std::fixed << std::setw(20) << MALDI_mz[i]
        <<"| " << std::setprecision(3) << std::fixed << std::setw(15) << MALDI_rounded_mz_data[i]
        <<"| " << std::setprecision(3) << std::fixed << std::setw(15) << MALDI_truncated_mz_data[i]
        <<"| " << std::setprecision(3) << std::fixed << std::setw(15) << MALDI_max_intensity[i] << "  |" << std::endl;
    }
}

void MALDI_MZManip::Attribute_restarter()
{
    MALDI_name.clear();
    MALDI_mz.clear();
    MALDI_max_intensity.clear();
    MALDI_rounded_mz_data.clear();
    MALDI_truncated_mz_data.clear();
}