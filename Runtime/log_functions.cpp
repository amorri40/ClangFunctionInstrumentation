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

extern "C" {
    bool ALI_GLOBAL_DEBUG = true;
    sqlite3 *ali__log__db;
    int ALI_GLOBAL_MAX_EX = 200;
    int ALI_GLOBAL_MAX_CHANGES = 2000; //per function changes (useful for big loops)
    struct alang_StaticFunctionData {
        int execution_number;
        const char* the_func_name;
        int the_line_number;
        const char* the_file_name;
    };
    
    struct alang_StaticFunctionData* g_sfd;
    int g_current_change_number = 0;
    int g_prev_change_number = 0;
    
    void alang_push_ex(struct alang_StaticFunctionData* sfd) {
        //create a new object that gets deleted in the pop?
        g_sfd=sfd;
        sfd->execution_number++; //supposed to do in pop just a test
        g_prev_change_number = g_current_change_number;
        g_current_change_number = 0;
        //ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db);
        
    }
    
    int alang_log_data(int line_num, int start_loc, int end_loc, int val) {
        if (g_current_change_number > ALI_GLOBAL_MAX_CHANGES) return val;
        g_current_change_number++;
        //std::cerr << g_sfd->execution_number;
        std::ostringstream v;
        v << val;
        //ali_clang_add_to_map(typeid(T).name(),v.str())
        return val;
    }
    
    void alang_pop_ex(struct alang_StaticFunctionData* sfd) {
        g_current_change_number = g_prev_change_number;
    }

    
    void alang_flush_to_db(alang_StaticFunctionData* sfd) {
    
    }
}

namespace ali_clang_plugin_runtime {

    bool ALI_GLOBAL_DEBUG = true;
sqlite3 *ali__log__db;
    int ALI_GLOBAL_MAX_EX = 300;

    void open_sqlite(std::string db_name) {
        
        /* Open database */
        int rc = sqlite3_open(db_name.c_str(), &ali__log__db);
        if( rc ){
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ali__log__db));
            //exit(0);
        }
    }
    
    void create_table(std::string table_name, std::string table_name_suffix, std::string schema) {
        
        std::ostringstream oss;
        oss << "CREATE TABLE IF NOT EXISTS \"" << table_name << table_name_suffix  << "\" "<< schema;
        char * sErrMsg = 0;
        sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
    }
    
    
    sqlite3_stmt * start_insert(std::string table_name, std::string table_name_suffix, std::string schema) {
        sqlite3_stmt * stmt;
        const char * tail = 0;
        
        std::ostringstream oss;
        oss << "INSERT INTO \"" << table_name << table_name_suffix  << "\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
        sqlite3_prepare_v2(ali__log__db,  oss.str().c_str(), oss.str().length(), &stmt, &tail);
        return stmt;
    }
    
    int bind_change_sql(sqlite3_stmt * stmt, std::string unique_id, std::string types, std::string names, std::string values, int line_num, int tim ) {
        sqlite3_bind_text(stmt, 1, unique_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, types.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, names.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, values.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, line_num);
        sqlite3_bind_int(stmt, 6, tim);
        
        int result = sqlite3_step(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
        return result;
    }

    void StaticFunctionData::create_tables() {
        open_sqlite("enigma_compiler.sqlite");
        create_table(func_name, "_changes_unique", " (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ");
        //create_table(func_name, "_changes_unique", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
        create_table(func_name, "_executions_unique", " (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Start_Time INTEGER, End_Time INTEGER) ");
        create_table(func_name, "_executions_all", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Start_Time INTEGER, End_Time INTEGER PRIMARY KEY)");
        created_database=true;
    }
    
    bool created_segfault_handler=false;
    
    void StaticFunctionData::flush_to_db() {
        if (all_function_executions.empty()) return;
        //if (!created_segfault_handler) {SEGFAULTHANDLE; created_segfault_handler=true;}
        if (!created_database) create_tables();
        this->execution_number++;
        //std::cout << "\n\n" << func_name << "\n";
        
        char * sErrMsg = 0;
        sqlite3_stmt *stmt_unique , *stmt_ex_unique, *stmt_ex_all;
        
        
        //stmt_all = start_insert(func_name,"_changes_all"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        stmt_unique = start_insert(func_name,"_changes_unique"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        stmt_ex_all = start_insert(func_name,"_executions_all"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        stmt_ex_unique = start_insert(func_name,"_executions_unique"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        
        sqlite3_exec(ali__log__db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
        
        for(std::vector<vector_of_change>::size_type execution = 0; execution != all_function_executions.size(); execution++) {
            vector_of_change line_data = all_function_executions[execution];
            std::ostringstream special_id;
            std::ostringstream execution_id, all_execution_id;
            
            unsigned long start_time=0,end_time=0;
            for(vector_of_change::iterator it2 = line_data.begin(); it2 != line_data.end(); ++it2) { //
               Change c = *it2;
                
                if (it2 == line_data.begin())
                start_time=c.time_of_change;
                
                std::ostringstream current_line_names;
                std::ostringstream current_line_values;
                std::ostringstream current_line_types;
                std::ostringstream unique_special_id;
                    unique_special_id << "[" << c.line_num << "," << c.start_loc << "," << c.end_loc << ",\"" << c.value << "\", \""<< c.type_of_var << "\"]";
                    end_time = c.time_of_change;
                long long insert_row_id=-1;
                
                if ( map_of_sqlrows.find(unique_special_id.str()) == map_of_sqlrows.end() ) {
                    // not found so add to db
                    int change_result = bind_change_sql(stmt_unique,unique_special_id.str(), current_line_types.str(), current_line_names.str(), current_line_values.str(), c.line_num, end_time);
                    long long last_row;
                    if (change_result == SQLITE_DONE)
                    last_row = sqlite3_last_insert_rowid(ali__log__db);
                    else
                        last_row = -1;
                    if (last_row !=-1) {
                        map_of_sqlrows[unique_special_id.str()] = last_row; insert_row_id = last_row;}
                    else
                    {map_of_sqlrows[unique_special_id.str()] = -1; insert_row_id = -1;}
                } else {
                    insert_row_id = map_of_sqlrows[unique_special_id.str()];
                }
                execution_id << insert_row_id << ",";
            }
                int unique_ex_result = bind_change_sql(stmt_ex_unique,execution_id.str(), "", "", "", start_time, end_time);
                unsigned long unique_that_was_just_inserted = sqlite3_last_insert_rowid(ali__log__db);
                bind_change_sql(stmt_ex_all,execution_id.str(), "", "", "", start_time, end_time);
            }
        
        
        sqlite3_exec(ali__log__db, "END TRANSACTION", NULL, NULL, &sErrMsg);
        //sqlite3_close(ali__log__db);
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
