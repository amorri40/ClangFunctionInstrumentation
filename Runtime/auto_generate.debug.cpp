//Debug file auto generated from clanginstrumentation 
extern void start_log_function(); extern void end_log_function(); 
#include <log_functions.h> 
//
//  auto_generate.cpp
//  EnigmaCompiler
//
//  Created by Alasdair Morrison on 02/08/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

//#include <stdio.h>
#include <System/token.h>
/*#include <System/token.h>

jdip::TOKEN_TYPE log_change(int line_num, int start_loc, int end_loc, jdip::TOKEN_TYPE val) { stdlogger;
    //std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("jdi::TOKEN_TYPE",ali_clang_value.str())
    return val;
}*/

std::ostream& operator<<( std::ostream& oStrStream, jdip::TOKEN_TYPE testEnum )
{
    oStrStream << "TestEnum";
    return oStrStream;
}
