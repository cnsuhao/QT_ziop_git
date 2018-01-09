#include <iostream>
#include <fstream>
#include <string>
#include "ParamConvertor.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtilInit.h"

int main( int argc, char** argv )
{
    TA_Base_Core::RunParams::getInstance().parseCmdLine(argc,argv);
    std::string runParam = "--debug-file=MFTToQtGUIConvertorLog.log --debug-level=DEBUG";
    //runParam += argv[1];
    TA_Base_Core::RunParams::getInstance().parseCmdLine( runParam.c_str() );
    TA_Base_Core::gSetDebugUtilFromRunParams();

    try
    {
        ParamConvertor oParamConvertor;
        oParamConvertor.readConfigure();
        oParamConvertor.readRc();
        oParamConvertor.convert();
    }
    catch( ... )
    {
    }
    return 0;
}
