#include <fstream>
#include <sstream>
#include <array>
#include "LCMS_MZ_Manipulator.h"
#include "MALDI_MZ_Manipulator.h"
#include "MZ_Analyzer.h"

int main() {

    LCMS_MzManip YAD_YC, OAD_YAD, OAD_OC, OC_YC;
    std::array <std::string, 4> LCMS_names = {"YAD_YC", "OAD_YAD", "OAD_OC", "OC_YC"};


    MALDI_MZManip YFM1CP, YFM2CP, YFM3CP, YFM4CP, YFM5CP, YFM6CP, YFM2ADP, YFM3ADP, YFM4ADP, YFM5ADP, YFM6ADP,
        OFM1CP, OFM2CP, OFM3CP, OFM4CP, OFM5CP, OFM6CP, OFMEXCP, OFM1ADP, OFM2ADP, OFM3ADP, OFM4ADP, OFM5ADP, OFM6ADP;

    // }
    std::array<MALDI_MZManip, 11> YoungAD_YoungControl = {YFM1CP, YFM2CP, YFM3CP, YFM4CP, YFM5CP, YFM6CP,
        YFM2ADP, YFM3ADP, YFM4ADP, YFM5ADP, YFM6ADP};
    std::array<std::string, 11> YoungAD_YoungControl_names {"YFM1CP","YFM2CP","YFM3CP","YFM4CP","YFM5CP","YFM6CP",
        "YFM2ADP","YFM3ADP","YFM4ADP","YFM5ADP","YFM6ADP"};


    std::array<MALDI_MZManip, 13> OldAD_OldControl = {OFM1CP, OFM2CP, OFM3CP, OFM4CP, OFM5CP, OFM6CP, OFMEXCP,
        OFM1ADP, OFM2ADP, OFM3ADP, OFM4ADP, OFM5ADP, OFM6ADP};
    std::array <std::string, 13> OldAD_OldControl_names {"OFM1CP","OFM2CP","OFM3CP", "OFM4CP","OFM5CP","OFM6CP","OFMEXCP",
        "OFM1ADP","OFM2ADP","OFM3ADP","OFM4ADP","OFM5ADP","OFM6ADP"};


    std::array<MALDI_MZManip, 11> OldAD_YoungAD = { YFM2ADP, YFM3ADP, YFM4ADP, YFM5ADP, YFM6ADP,
        OFM1ADP, OFM2ADP, OFM3ADP, OFM4ADP, OFM5ADP, OFM6ADP};
    std::array <std::string, 11> OldAD_YoungAD_names { "YFM2ADP","YFM3ADP","YFM4ADP","YFM5ADP","YFM6ADP",
        "OFM1ADP","OFM2ADP","OFM3ADP","OFM4ADP","OFM5ADP","OFM6ADP"};


    std::array<MALDI_MZManip, 13> OldControl_YoungControl = {YFM1CP, YFM2CP, YFM3CP, YFM4CP, YFM5CP, YFM6CP,
        OFM1CP, OFM2CP, OFM3CP, OFM4CP, OFM5CP, OFM6CP, OFMEXCP};
    std::array <std::string, 13>  OldControl_YoungControl_names {"YFM1CP","YFM2CP","YFM3CP","YFM4CP","YFM5CP","YFM6CP",
        "OFM1CP","OFM2CP","OFM3CP", "OFM4CP","OFM5CP","OFM6CP","OFMEXCP"};


    MZ_Analyzer YADYC;
    std::string filepath1 = "/Users/amin/Desktop/c++/MZ_Matcher/files/Results/YoungAD_YoungControl_Results.csv";
    MZ_Analyzer::CSV_File_Creator(filepath1);

    for(int i=0; i <YoungAD_YoungControl.size();i++)
    {

        YADYC.LCMS_attribute_setter( "/Users/amin/Desktop/c++/MZ_Matcher/files/LCMS/YAD_YC.csv","YAD_YC" );
        YADYC.LCMS_Mz_list_getter();
        YADYC.LCMS_Mz_list_rounder();
        YADYC.LCMS_Mz_list_trunkator();
        //YAD_YC.LCMS_Mz_list_display();
        YADYC.MALDI_attribute_setter( "/Users/amin/Desktop/c++/MZ_Matcher/files/MALDI_MS/"+YoungAD_YoungControl_names[i]+".csv",YoungAD_YoungControl_names[i] );
        YADYC.MALDI_Mz_list_getter();
        YADYC.MALDI_Mz_list_rounder();
        YADYC.MALDI_Mz_list_trunkator();
        YADYC.MALDI_Mz_list_display();
        YADYC.MZ_comparator();
        YADYC.Results_writer(filepath1);
        YADYC.LCMS_Attribute_restarter();
        YADYC.MALDI_Attribute_restarter();
        YADYC.MZ_analyzer_Attribute_restarter();

    }

    MZ_Analyzer OADOC;
    std::string filepath2 = "/Users/amin/Desktop/c++/MZ_Matcher/files/Results/OldAD_OldControl_Results.csv";
    MZ_Analyzer::CSV_File_Creator(filepath2);
    for(int i=0; i <OldAD_OldControl.size();i++)
    {
        OADOC.LCMS_attribute_setter( "/Users/amin/Desktop/c++/MZ_Matcher/files/LCMS/OAD_OC.csv","OAD_OC" );
        OADOC.LCMS_Mz_list_getter();
        OADOC.LCMS_Mz_list_rounder();
        OADOC.LCMS_Mz_list_trunkator();
        //OADOC.LCMS_Mz_list_display();
        OADOC.MALDI_attribute_setter( "/Users/amin/Desktop/c++/MZ_Matcher/files/MALDI_MS/"+OldAD_OldControl_names[i]+".csv",OldAD_OldControl_names[i] );
        OADOC.MALDI_Mz_list_getter();
        OADOC.MALDI_Mz_list_rounder();
        OADOC.MALDI_Mz_list_trunkator();
        //OADOC.MALDI_Mz_list_display();
        OADOC.MZ_comparator();
        OADOC.Results_writer(filepath2);
        OADOC.LCMS_Attribute_restarter();
        OADOC.MALDI_Attribute_restarter();
        OADOC.MZ_analyzer_Attribute_restarter();
    }


    MZ_Analyzer OADYAD;
    std::string filepath3 = "/Users/amin/Desktop/c++/MZ_Matcher/files/Results/OldAD_YoungAD_Results.csv";
    MZ_Analyzer::CSV_File_Creator(filepath3);
    for(int i=0; i <OldAD_YoungAD.size();i++)
    {
        OADYAD.LCMS_attribute_setter( "/Users/amin/Desktop/c++/MZ_Matcher/files/LCMS/OAD_YAD.csv","OAD_YAD" );
        OADYAD.LCMS_Mz_list_getter();
        OADYAD.LCMS_Mz_list_rounder();
        OADYAD.LCMS_Mz_list_trunkator();
        //OADYAD.LCMS_Mz_list_display();
        OADYAD.MALDI_attribute_setter( "/Users/amin/Desktop/c++/MZ_Matcher/files/MALDI_MS/"+OldAD_YoungAD_names[i]+".csv",OldAD_YoungAD_names[i] );
        OADYAD.MALDI_Mz_list_getter();
        OADYAD.MALDI_Mz_list_rounder();
        OADYAD.MALDI_Mz_list_trunkator();
        OADYAD.MZ_comparator();
        OADYAD.Results_writer(filepath3);
        OADYAD.LCMS_Attribute_restarter();
        OADYAD.MALDI_Attribute_restarter();
        OADYAD.MZ_analyzer_Attribute_restarter();
    }


    MZ_Analyzer OCYC;
    std::string filepath4 = "/Users/amin/Desktop/c++/MZ_Matcher/files/Results/OldControl_YoungControl_Results.csv";
    MZ_Analyzer::CSV_File_Creator(filepath4);
    for(int i=0; i <OldControl_YoungControl.size();i++)
    {
        OCYC.LCMS_attribute_setter( "/Users/amin/Desktop/c++/MZ_Matcher/files/LCMS/OC_YC.csv","OC_YC" );
        OCYC.LCMS_Mz_list_getter();
        OCYC.LCMS_Mz_list_rounder();
        OCYC.LCMS_Mz_list_trunkator();
        //OCYC.LCMS_Mz_list_display();
        OCYC.MALDI_attribute_setter( "/Users/amin/Desktop/c++/MZ_Matcher/files/MALDI_MS/"+OldControl_YoungControl_names[i]+".csv",OldControl_YoungControl_names[i] );
        OCYC.MALDI_Mz_list_getter();
        OCYC.MALDI_Mz_list_rounder();
        OCYC.MALDI_Mz_list_trunkator();
        OCYC.MZ_comparator();
        OCYC.Results_writer(filepath4);
        OCYC.LCMS_Attribute_restarter();
        OCYC.MALDI_Attribute_restarter();
        OCYC.MZ_analyzer_Attribute_restarter();
    }

    return 0;
}

