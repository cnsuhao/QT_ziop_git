#include "StdAfx.h"
#include "RCParserThread.h"
#include "CommandLineHelper.h"

// TODO:
// 1) verbos output

int main(int argc, char* argv[])
{
    CommandLineHelper command_line_helper( argc, argv );
    ParameterPtrList parameter_list = command_line_helper.get_parameter_list();
    RCParserThreadPtrList parser_threads;

    for ( size_t i = 0; i < parameter_list.size(); ++i )
    {
        parser_threads.push_back( RCParserThreadPtr( new RCParserThread( parameter_list[i] ) ) );
    }

	return 0;
}
