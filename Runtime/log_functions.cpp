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
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

template <class T> std::string TToStr(T* t)
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

unsigned long report_memory(void) {
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
#include <unistd.h>

void segfault_handler(int sig) {
    void *array[10];
    size_t size;
    
    // get void*'s for all entries on the stack
    size = backtrace(array, 10);
    
    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
    void install_handlers() {
        signal(SIGSEGV, segfault_handler);
    }
}
