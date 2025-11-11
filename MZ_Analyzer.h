//
// Created by Amin on 8/20/24.
//

#ifndef MZ_ANALYZER_H
#define MZ_ANALYZER_H
#include "LCMS_MZ_Manipulator.h"
#include "MALDI_MZ_Manipulator.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//#include <filesystem>


class MZ_Analyzer: public MALDI_MZManip, public LCMS_MzManip {
    std::vector<std::string> animalname_MALDI{};
    std::vector<std::string> mzname{};
    std::vector<float> common_mz{};
    std::vector<float> common_LCMS_mz{};
    std::vector<float> common_MALDI_mz{};
    std::vector<float> common_MALDI_max_intensity{};
    std::vector<float> common_fold_change{};
    std::vector<float> common_p_value{};
    std::vector<std::string> mz_reduction_method{};

public:

    //LCMS
    void LCMS_attribute_setter(const std::string&, const std::string&);
    int LCMS_Mz_list_getter();
    void LCMS_Mz_list_rounder( );
    void LCMS_Mz_list_trunkator( );
    void LCMS_Mz_list_display() const;
    void LCMS_Attribute_restarter();

    //MALDI
    void MALDI_attribute_setter(const std::string&, const std::string&);
    int MALDI_Mz_list_getter();
    void MALDI_Mz_list_display() const;
    void MALDI_Mz_list_rounder();
    void MALDI_Mz_list_trunkator();
    void MALDI_Attribute_restarter();

    //new
    static void CSV_File_Creator(const std::string& ) ;
    void MZ_comparator();
    void Results_writer(const std::string& ) const;
    void MZ_analyzer_Attribute_restarter();


};



#endif //MZ_ANALYZER_H
