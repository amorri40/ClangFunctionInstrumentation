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



    void StaticFunctionData::create_tables() {
        char * sErrMsg = 0;
        std::ostringstream oss;
        oss << "CREATE TABLE IF NOT EXISTS \"" << func_name  << "_unique\" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ";
        sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
        oss.str(""); //clear string stream
        oss << "CREATE TABLE IF NOT EXISTS \"" << func_name  << "_all\" (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)";
        sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
    }
    
    void StaticFunctionData::flush_to_db() {
        
        if (all_function_executions.empty()) return;
        /* Open database */
        int rc = sqlite3_open("enigma_compiler.sqlite", &ali__log__db);
        if( rc ){
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ali__log__db));
            exit(0);
        }
        std::cout << "flush_to_db";
        char * sErrMsg = 0;
        sqlite3_stmt * stmt_all, *stmt_unique;
        const char * tail = 0;
        const char * tail_unique = 0;
        char sSQL_all [BUFFER_SIZE] = "\0";
        char sSQL_unique [BUFFER_SIZE] = "\0";
        std::ostringstream oss;
        create_tables();
        
        oss << "INSERT INTO \"" << func_name << "_all\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
        sprintf(sSQL_all, oss.str().c_str());
        sqlite3_prepare_v2(ali__log__db,  sSQL_all, BUFFER_SIZE, &stmt_all, &tail);
        oss.str("");
        oss << "INSERT INTO \"" << func_name << "_unique\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
        sprintf(sSQL_unique, oss.str().c_str());
        sqlite3_prepare_v2(ali__log__db,  sSQL_unique, BUFFER_SIZE, &stmt_unique, &tail_unique);
        
        
        sqlite3_exec(ali__log__db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
        std::cout << "\nFunction: " << func_name << " \n";
        
        
        for(std::vector<map_of_vector_of_change>::size_type execution = 0; execution != all_function_executions.size(); execution++) {
            map_of_vector_of_change line_data = all_function_executions[execution];
            std::ostringstream special_id;
            for(map_of_vector_of_change::iterator line = line_data.begin(); line != line_data.end(); ++line) { //for(map_of_vector_of_string::size_type i = 0; i != line_data.size(); i++) {
                //if (line_data[i].empty()) continue;
                int line_num = (line->first);
                std::cout << "  " << line_num << ": ";
                special_id << line_num << "_";
                std::string previous_r_value;
                std::ostringstream unique_special_id;
                std::ostringstream current_line_names;
                std::ostringstream current_line_values;
                std::ostringstream current_line_types;
                int tim=0;
                unique_special_id << special_id.str();
                for(std::vector<Change>::reverse_iterator it2 = line->second.rbegin(); it2 != line->second.rend(); ++it2) {
                    /*
                     All the changes that happened on this line during 1 execution of the function
                     */
                    
                    //unique_special_id << special_id.str();
                    Change c = *it2;
                    
                    current_line_types << "(" << c.start_loc << ":" << c.end_loc << "=" << c.value << c.type_of_var << ")";
                    current_line_values << c.value;
                    current_line_names << "";
                    unique_special_id << current_line_types.str();
                    
                    /*if (c.type == CHANGE_LHS) {
                        //unique_special_id << "L";
                        current_line_names << "->";
                        current_line_types << "->";
                        current_line_values << "->";
                        unique_special_id << c.name_of_var << "=";
                        current_line_names << c.name_of_var;
                    } else if (c.type == CHANGE_RHS) {
                        //right hand side
                        unique_special_id << c.value;
                        current_line_names << c.name_of_var;
                    } else if (c.type == CHANGE_FUNCTIONCALL) {
                        current_line_names <<"(" << c.name_of_var << "())";
                    }*/
                    tim = c.time_of_change;
                    
                }
                
                // << "_" << tim;
                sqlite3_bind_text(stmt_all, 1, unique_special_id.str().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt_all, 2, current_line_types.str().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt_all, 3, current_line_names.str().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt_all, 4, current_line_values.str().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt_all, 5, line_num);
                sqlite3_bind_int(stmt_all, 6, tim);
                sqlite3_step(stmt_all);
                
                sqlite3_bind_text(stmt_unique, 1, unique_special_id.str().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt_unique, 2, current_line_types.str().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt_unique, 3, current_line_names.str().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt_unique, 4, current_line_values.str().c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt_unique, 5, line_num);
                sqlite3_bind_int(stmt_unique, 6, tim);
                sqlite3_step(stmt_unique);
                
                
                sqlite3_clear_bindings(stmt_all);
                sqlite3_reset(stmt_all);
                sqlite3_clear_bindings(stmt_unique);
                sqlite3_reset(stmt_unique);
                //std::cout << "\n";
            }
        }
        
        sqlite3_exec(ali__log__db, "END TRANSACTION", NULL, NULL, &sErrMsg);
        sqlite3_close(ali__log__db);
        all_function_executions.clear(); //now that they have been writtern destroy them
    }
    

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
