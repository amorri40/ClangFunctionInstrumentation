//Debug file auto generated from clanginstrumentation 
extern void start_log_function(); extern void end_log_function(); 
#include <log_functions.h> 
//
//  log_functions.cpp
//  EnigmaCompiler
//
//  Created by Alasdair Morrison on 26/07/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#include "log_functions.h"
#define NO_INSTRUMENT true
#include <sstream>

bool ALI_GLOBAL_DEBUG = true;
sqlite3 *ali__log__db;


template <class T> std::string TToStr(const T& t)
{ 
 #if NO_INSTRUMENT == false 
 if (!ALI_GLOBAL_DEBUG || NO_INSTRUMENT)  {
    std::ostringstream oss;
    oss << t;
    return oss.str();
} else {static StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); InstrumentFunctionDB inst_func_db(&ali_function_db);  
 #endif 
{
    /*DECL*/std::ostringstream oss;
    oss << t;
    return oss.str();
}
 #if NO_INSTRUMENT == false 
 } 
 #endif 
}

unsigned long report_memory(void) { 
 #if NO_INSTRUMENT == false 
 if (!ALI_GLOBAL_DEBUG || NO_INSTRUMENT)  {
    struct task_basic_info info;
    mach_msg_type_number_t size = sizeof(info);
    kern_return_t kerr = task_info(mach_task_self(),
                                   TASK_BASIC_INFO,
                                   (task_info_t)&info,
                                   &size);
    if( kerr == KERN_SUCCESS ) {
        //printf("Memory in use (in bytes): %u", info.resident_size);
        return info.resident_size;
    } else {
        printf("Error with task_info(): %s", mach_error_string(kerr));
        return -1;
    }
} else {static StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); InstrumentFunctionDB inst_func_db(&ali_function_db);  
 #endif 
{
    /*DECL*/struct task_basic_info info;
    /*DECL*/mach_msg_type_number_t size = sizeof(info);
    /*DECL*/kern_return_t kerr = task_info(mach_task_self(),
                                   TASK_BASIC_INFO,
                                   (task_info_t)&info,
                                   &size);
    if( kerr == KERN_SUCCESS ) /*Compound*/{
        //printf("Memory in use (in bytes): %u", info.resident_size);
        return info.resident_size;
    } else /*Compound*/{
        printf("Error with task_info(): %s", mach_error_string(kerr));
        return -1;
    }
}
 #if NO_INSTRUMENT == false 
 } 
 #endif 
}
