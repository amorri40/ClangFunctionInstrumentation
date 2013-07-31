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

    void open_sqlite(std::string db_name) {
        
        /* Open database */
        int rc = sqlite3_open(db_name.c_str(), &ali__log__db);
        if( rc ){
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ali__log__db));
            exit(0);
        }
    }
    
    void create_table(std::string table_name, std::string table_name_suffix, std::string schema) {
        
        std::ostringstream oss;
        oss << "CREATE TABLE IF NOT EXISTS \"" << table_name << table_name_suffix  << "\" "<< schema;
        char * sErrMsg = 0;
        sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
    }
    
    sqlite3_stmt * start_insert(std::string table_name, std::string table_name_suffix, std::string schema) {
        char sSQL_all [BUFFER_SIZE] = "\0";
        sqlite3_stmt * stmt;
        const char * tail = 0;
        
        std::ostringstream oss;
        oss << "INSERT INTO \"" << table_name << table_name_suffix  << "\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
        sprintf(sSQL_all, oss.str().c_str());
        sqlite3_prepare_v2(ali__log__db,  sSQL_all, BUFFER_SIZE, &stmt, &tail);
        return stmt;
    }
    
    void bind_change_sql(sqlite3_stmt * stmt, std::string unique_id, std::string types, std::string names, std::string values, int line_num, int tim ) {
        sqlite3_bind_text(stmt, 1, unique_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, types.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, names.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, values.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, line_num);
        sqlite3_bind_int(stmt, 6, tim);
        sqlite3_step(stmt);
    }

    void StaticFunctionData::create_tables() {
        open_sqlite("enigma_compiler.sqlite");
        create_table(func_name, "_changes_unique", " (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ");
        //create_table(func_name, "_changes_all", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
        create_table(func_name, "_executions_unique", " (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ");
        create_table(func_name, "_executions_all", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
    }
    
    void StaticFunctionData::flush_to_db() {
        if (all_function_executions.empty()) return;
        
        char * sErrMsg = 0;
        sqlite3_stmt *stmt_unique , *stmt_ex_unique, *stmt_ex_all;
        
        create_tables();
        
        //stmt_all = start_insert(func_name,"_changes_all"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        stmt_unique = start_insert(func_name,"_changes_unique"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        stmt_ex_all = start_insert(func_name,"_executions_all"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        stmt_ex_unique = start_insert(func_name,"_executions_unique"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        
        sqlite3_exec(ali__log__db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
        
        for(std::vector<vector_of_change>::size_type execution = 0; execution != all_function_executions.size(); execution++) {
            vector_of_change line_data = all_function_executions[execution];
            std::ostringstream special_id;
            std::ostringstream execution_id;
            int tim=0;
            for(vector_of_change::iterator it2 = line_data.begin(); it2 != line_data.end(); ++it2) { //
               // int line_num = line_data.l;
               // std::cout << "  " << line_num << ": ";
               // special_id << line_num << "_";
                //std::string previous_r_value;
                
                
                std::ostringstream current_line_names;
                std::ostringstream current_line_values;
                std::ostringstream current_line_types;
                
                //unique_special_id << special_id.str();
                //for(std::vector<Change>::reverse_iterator it2 = line->second.rbegin(); it2 != line->second.rend(); ++it2) {
                    std::ostringstream unique_special_id;
                    /*
                     All the changes that happened on this line during 1 execution of the function
                     */
                    
                    Change c = *it2;
                    
                    unique_special_id << "(" << c.line_num << ":" << c.start_loc << ":" << c.end_loc << "=" << c.value << " ("<< c.type_of_var << ")";
                    //current_line_values << c.value;
                    //current_line_names << "";
                    //unique_special_id << current_line_types.str();
                    
                    tim = c.time_of_change;
                    
                    //bind_change_sql(stmt_all,unique_special_id.str(), current_line_types.str(), current_line_names.str(), current_line_values.str(), c.line_num, tim);
                    bind_change_sql(stmt_unique,unique_special_id.str(), current_line_types.str(), current_line_names.str(), current_line_values.str(), c.line_num, tim);
                    
                    execution_id << unique_special_id.str();
                    
                }
                
                bind_change_sql(stmt_ex_all,execution_id.str(), "", "", "", 0, tim); //this is per line
                bind_change_sql(stmt_ex_unique,execution_id.str(), "", "", "", 0, tim);
                
                /*sqlite3_bind_text(stmt_all, 1, unique_special_id.str().c_str(), -1, SQLITE_TRANSIENT);
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
                sqlite3_step(stmt_unique);*/
                
                
                //sqlite3_clear_bindings(stmt_all);
                //sqlite3_reset(stmt_all);
                sqlite3_clear_bindings(stmt_unique);
                sqlite3_reset(stmt_unique);
                
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
