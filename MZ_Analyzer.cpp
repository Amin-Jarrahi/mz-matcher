//
// Created by Amin on 8/20/24.
//

#include "MZ_Analyzer.h"

//LCMS
void  MZ_Analyzer::LCMS_attribute_setter(const std::string& path, const std::string& name)
{
    this->LCMS_name = name;
    LCMS_file_path=path;
    std::cout<<path <<std::endl;
}

int MZ_Analyzer::LCMS_Mz_list_getter( )
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

void MZ_Analyzer::LCMS_Mz_list_rounder( )
{
    LCMS_rounded_mz_data = LCMS_mz;
    for(float &each_mz: LCMS_rounded_mz_data)
    {
        each_mz = std::round(each_mz * 100.0f) / 100.0f;
    }
}

void MZ_Analyzer::LCMS_Mz_list_trunkator( )
{
    LCMS_truncated_mz_data = LCMS_mz;
    for(float &each_mz: LCMS_truncated_mz_data)
    {
        each_mz = std::truncf(each_mz * 100.0f) / 100.0f;
    }
}

void MZ_Analyzer::LCMS_Mz_list_display() const
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

void MZ_Analyzer::LCMS_Attribute_restarter()
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


//MALDI
void  MZ_Analyzer::MALDI_attribute_setter(const std::string& path, const std::string& name)
{
    this->MALDI_name = name;
    MALDI_file_path=path;
    std::cout<<path <<std::endl;
}

int MZ_Analyzer::MALDI_Mz_list_getter( )
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

void MZ_Analyzer::MALDI_Mz_list_rounder( )
{
    MALDI_rounded_mz_data = MALDI_mz;
    for(float &each_mz: MALDI_rounded_mz_data)
    {
        each_mz = std::round(each_mz * 100.0f) / 100.0f;
    }
}

void MZ_Analyzer::MALDI_Mz_list_trunkator( )
{
    MALDI_truncated_mz_data = MALDI_mz;
    for(float &each_mz: MALDI_truncated_mz_data)
    {
        each_mz = std::truncf(each_mz * 100.0f) / 100.0f;
    }
}

void MZ_Analyzer::MALDI_Mz_list_display() const
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

void MZ_Analyzer::MALDI_Attribute_restarter()
{
    MALDI_name.clear();
    MALDI_mz.clear();
    MALDI_max_intensity.clear();
    MALDI_rounded_mz_data.clear();
    MALDI_truncated_mz_data.clear();
}



//NEW
void MZ_Analyzer::CSV_File_Creator(const std::string& filename)
{
    if (std::ofstream outputFile(filename); outputFile.is_open()) {
        outputFile <<"Animal"<<","
            << "mzname" << ","
            << "common_mz" << ","
            << "LCMS mz" << ","
            << "MALDI mz" << ","
            << "MALDI max_intensity" << ","
            << "fold change" << ","
            << "p_value"  << ","
            << "Reduction Method" << std::endl;

        outputFile.close();
        std::cout << "Data written to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file " << filename << std::endl;
    }
}

void MZ_Analyzer::MZ_comparator()
{

    for(int mzi = 0; mzi< LCMS_rounded_mz_data.size(); mzi++)
    {
        for(int mzj = 0; mzj< MALDI_rounded_mz_data.size(); mzj++)
        {
            if(LCMS_rounded_mz_data[mzi] == MALDI_rounded_mz_data[mzj])
            {
                animalname_MALDI.emplace_back(MALDI_name);
                mzname.emplace_back(LCMS_mzname[mzi]);
                common_mz.emplace_back(LCMS_rounded_mz_data[mzi]);
                common_LCMS_mz.emplace_back(LCMS_mz[mzi]);
                common_MALDI_mz.emplace_back(MALDI_mz[mzj]);
                common_MALDI_max_intensity.emplace_back(MALDI_max_intensity[mzj]);
                common_fold_change.emplace_back(LCMS_fold_change[mzi]);
                common_p_value.emplace_back(LCMS_p_value[mzi]);
                mz_reduction_method.emplace_back("RR");
            }
        }
    }

    for(int mzi = 0; mzi< LCMS_rounded_mz_data.size(); mzi++)
    {
        for(int mzj = 0; mzj< MALDI_truncated_mz_data.size(); mzj++)
        {
            if(LCMS_rounded_mz_data[mzi] == MALDI_truncated_mz_data[mzj])
            {
                animalname_MALDI.emplace_back(MALDI_name);
                mzname.emplace_back(LCMS_mzname[mzi]);
                common_mz.emplace_back(LCMS_rounded_mz_data[mzi]);
                common_LCMS_mz.emplace_back(LCMS_mz[mzi]);
                common_MALDI_mz.emplace_back(MALDI_mz[mzj]);
                common_MALDI_max_intensity.emplace_back(MALDI_max_intensity[mzj]);
                common_fold_change.emplace_back(LCMS_fold_change[mzi]);
                common_p_value.emplace_back(LCMS_p_value[mzi]);
                mz_reduction_method.emplace_back("RT");
            }
        }
    }


    for(int mzi = 0; mzi< LCMS_truncated_mz_data.size(); mzi++)
    {
        for(int mzj = 0; mzj< MALDI_rounded_mz_data.size(); mzj++)
        {
            if(LCMS_truncated_mz_data[mzi] == MALDI_rounded_mz_data[mzj])
            {
                animalname_MALDI.emplace_back(MALDI_name);
                mzname.emplace_back(LCMS_mzname[mzi]);
                common_mz.emplace_back(LCMS_truncated_mz_data[mzi]);
                common_LCMS_mz.emplace_back(LCMS_mz[mzi]);
                common_MALDI_mz.emplace_back(MALDI_mz[mzj]);
                common_MALDI_max_intensity.emplace_back(MALDI_max_intensity[mzj]);
                common_fold_change.emplace_back(LCMS_fold_change[mzi]);
                common_p_value.emplace_back(LCMS_p_value[mzi]);
                mz_reduction_method.emplace_back("TR");
            }
        }
    }


    for(int mzi = 0; mzi< LCMS_truncated_mz_data.size(); mzi++)
    {
        for(int mzj = 0; mzj< MALDI_truncated_mz_data.size(); mzj++)
        {
            if(LCMS_truncated_mz_data[mzi] == MALDI_truncated_mz_data[mzj])
            {
                animalname_MALDI.emplace_back(MALDI_name);
                mzname.emplace_back(LCMS_mzname[mzi]);
                common_mz.emplace_back(LCMS_truncated_mz_data[mzi]);
                common_LCMS_mz.emplace_back(LCMS_mz[mzi]);
                common_MALDI_mz.emplace_back(MALDI_mz[mzj]);
                common_MALDI_max_intensity.emplace_back(MALDI_max_intensity[mzj]);
                common_fold_change.emplace_back(LCMS_fold_change[mzi]);
                common_p_value.emplace_back(LCMS_p_value[mzi]);
                mz_reduction_method.emplace_back("TT");
            }
        }
    }
}

void MZ_Analyzer::Results_writer(const std::string& filename) const
{
    std::ofstream outputFile(filename, std::ios::app);

    if (outputFile.is_open()) {
        for (int i=0; i < mzname.size(); i++) {
            outputFile << animalname_MALDI[i] << ","
            << mzname[i] << ","
            << common_mz[i] << ","
            << common_LCMS_mz[i] << ","
            << common_MALDI_mz[i] << ","
            << common_MALDI_max_intensity[i] << ","
            << common_fold_change[i] << ","
            << common_p_value[i] << ","
            << mz_reduction_method[i] << "," << std::endl;
        }

        outputFile.close();
        std::cout << "Data written to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file " << filename << std::endl;
    }
    outputFile.close();
}

void MZ_Analyzer::MZ_analyzer_Attribute_restarter()
{
    animalname_MALDI.clear();
    mzname.clear();
    common_mz.clear();
    common_LCMS_mz.clear();
    common_MALDI_mz.clear();
    common_MALDI_max_intensity.clear();
    common_fold_change.clear();
    common_p_value.clear();
    mz_reduction_method.clear();
}