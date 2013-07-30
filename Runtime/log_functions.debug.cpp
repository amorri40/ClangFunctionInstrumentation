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

namespace ali_clang_plugin_runtime {

bool ALI_GLOBAL_DEBUG = true;
sqlite3 *ali__log__db;


/*template <class T> std::string TToStr(const T& t)
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}*/

template <class T> std::string TToStr(T& t)
{ 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(t); } {
     /*DECL*/ std::ostringstream oss;
     LHS(( oss))  << t;
    return  MEMBER_CALL(( (/*CXXBindTemporaryExpr*/ oss.str() /*End CXXBindTemporaryExpr*/) )) ;
}
}
#endif 
}

template <class T> std::string TToStr(T* t)
{ 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(t); } {
     /*DECL*/ std::ostringstream oss;
     LHS(( oss))  << t;
    return  MEMBER_CALL(( (/*CXXBindTemporaryExpr*/ oss.str() /*End CXXBindTemporaryExpr*/) )) ;
}
}
#endif 
}

unsigned long report_memory(void) { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
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
}
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
     /*DECL*/ struct task_basic_info info;
     /*DECL*/ mach_msg_type_number_t size = sizeof(info);
     /*DECL*/ kern_return_t kerr =  CALLR(( task_info(mach_task_self(),
                                   TASK_BASIC_INFO,
                                    CALL_ARG(((task_info_t)&info)) ,
                                    CALL_ARG((&size)) ) )) ;
    if(  LHS(( kerr))  == KERN_SUCCESS ) {
        //printf("Memory in use (in bytes): %u", info.resident_size);
        return  MEMBER_EXPR(( info.resident_size )) ;
    } else {
         CALLR(( printf( CALL_ARG(("Error with task_info(): %s")) ,  CALLR((  CALL_ARG((mach_error_string( CALL_ARG((kerr)) )))  )) ) )) ;
        return -1;
    }
}
}
#endif 
}
#include <unistd.h>

void segfault_handler(int sig) { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
    void *array[10];
    size_t size;
    
    // get void*'s for all entries on the stack
    size = backtrace(array, 10);
    
    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(sig); } {
     /*DECL*/ void *array[10];
     /*DECL*/ size_t size;
    
    // get void*'s for all entries on the stack
     LHS(( size))  =  CALLR(( backtrace( CALL_ARG((array)) ,  CALL_ARG((10)) ) )) ;
    
    // print out all the frames to stderr
     CALLR(( fprintf(stderr,  CALL_ARG(("Error: signal %d:\n")) ,  CALL_ARG((sig)) ) )) ;
     CALLR(( backtrace_symbols_fd( CALL_ARG((array)) ,  CALL_ARG((size)) , STDERR_FILENO) )) ;
     CALLR(( exit( CALL_ARG((1)) ) )) ;
}
}
#endif 
}
    void install_handlers() { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
        signal(SIGSEGV, segfault_handler);
    }
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
         CALLR(( signal(SIGSEGV,  CALL_ARG((segfault_handler)) ) )) ;
    }
}
#endif 
}
}
