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
    int ALI_GLOBAL_MAX_EX = 100;

    void open_sqlite(std::string db_name) { 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_open_sqlite", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
 #endif 
 {
        
        /* Open database */
        int rc = sqlite3_open(db_name.c_str(), &ali__log__db);
        if( rc ){
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ali__log__db));
            //exit(0);
        }
    }
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(db_name); } {
        
        /* Open database */
        int rc = sqlite3_open(db_name.c_str(), &ali__log__db);
        if(  /*(int)*/ LValueToRValue (23, 13, 15,  ( /*(_Bool)*/ IntegralToBoolean (23, 13, 15,  (rc)) ))  ){
             CALL(( fprintf(stderr,  CALL_ARG(("Can't open database: %s\n")) ,  CALL_ARG((sqlite3_errmsg(ali__log__db))) ) )) ;
            //exit(0);
        }
    }
}
#endif 
}
    
    void create_table(std::string table_name, std::string table_name_suffix, std::string schema) { 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_create_table", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
 #endif 
 {
        
        std::ostringstream oss;
        oss << "CREATE TABLE IF NOT EXISTS \"" << table_name << table_name_suffix  << "\" "<< schema;
        char * sErrMsg = 0;
        sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
    }
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(table_name);  LOGPARAMETER(table_name_suffix);  LOGPARAMETER(schema); } {
        
        std::ostringstream oss;
        oss << "CREATE TABLE IF NOT EXISTS \"" << table_name << table_name_suffix  << "\" "<< schema;
        char * sErrMsg = 0;
         CALLR (34, 9, 76,  (sqlite3_exec( ARG_UNKNOWN((ali__log__db)) ,  ARG_UNKNOWN((oss.str().c_str())) , NULL, NULL,  CALL_ARG((&sErrMsg)) ))) ;
    }
}
#endif 
}
    
    
    sqlite3_stmt * start_insert(std::string table_name, std::string table_name_suffix, std::string schema) { 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_start_insert", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
 #endif 
 {
        sqlite3_stmt * stmt;
        const char * tail = 0;
        
        std::ostringstream oss;
        oss << "INSERT INTO \"" << table_name << table_name_suffix  << "\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
        sqlite3_prepare_v2(ali__log__db,  oss.str().c_str(), oss.str().length(), &stmt, &tail);
        return stmt;
    }
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(table_name);  LOGPARAMETER(table_name_suffix);  LOGPARAMETER(schema); } {
        sqlite3_stmt * stmt;
        const char * tail = 0;
        
        std::ostringstream oss;
        oss << "INSERT INTO \"" << table_name << table_name_suffix  << "\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
         CALLR (44, 9, 95,  (sqlite3_prepare_v2( ARG_UNKNOWN((ali__log__db)) ,   ARG_UNKNOWN((oss.str().c_str())) ,  CALL_ARG((oss.str().length())) ,  ARG_UNKNOWN((&stmt)) ,  CALL_ARG((&tail)) ))) ;
        return  /*(sqlite3_stmt *)*/ LValueToRValue (45, 16, 20,  (stmt)) ;
    }
}
#endif 
}
    
    int bind_change_sql(sqlite3_stmt * stmt, std::string unique_id, std::string types, std::string names, std::string values, int line_num, int tim ) { 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_bind_change_sql", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
 #endif 
 {
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
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(stmt);  LOGPARAMETER(unique_id);  LOGPARAMETER(types);  LOGPARAMETER(names);  LOGPARAMETER(values);  LOGPARAMETER(line_num);  LOGPARAMETER(tim); } {
         CALLR (49, 9, 76,  (sqlite3_bind_text( ARG_UNKNOWN((stmt)) ,  CALL_ARG((1)) ,  ARG_UNKNOWN((unique_id.c_str())) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT))) ;
         CALLR (50, 9, 72,  (sqlite3_bind_text( ARG_UNKNOWN((stmt)) ,  CALL_ARG((2)) ,  ARG_UNKNOWN((types.c_str())) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT))) ;
         CALLR (51, 9, 72,  (sqlite3_bind_text( ARG_UNKNOWN((stmt)) ,  CALL_ARG((3)) ,  ARG_UNKNOWN((names.c_str())) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT))) ;
         CALLR (52, 9, 73,  (sqlite3_bind_text( ARG_UNKNOWN((stmt)) ,  CALL_ARG((4)) ,  ARG_UNKNOWN((values.c_str())) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT))) ;
         CALLR (53, 9, 44,  (sqlite3_bind_int( ARG_UNKNOWN((stmt)) ,  CALL_ARG((5)) ,  CALL_ARG((line_num)) ))) ;
         CALLR (54, 9, 39,  (sqlite3_bind_int( ARG_UNKNOWN((stmt)) ,  CALL_ARG((6)) ,  CALL_ARG((tim)) ))) ;
        
        int result = sqlite3_step(stmt);
         CALLR (57, 9, 37,  (sqlite3_clear_bindings( ARG_UNKNOWN((stmt)) ))) ;
         CALLR (58, 9, 28,  (sqlite3_reset( ARG_UNKNOWN((stmt)) ))) ;
        return  /*(int)*/ LValueToRValue (59, 16, 22,  (result)) ;
    }
}
#endif 
}

    void StaticFunctionData::create_tables() { 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_create_tables", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
 #endif 
 {
        open_sqlite("enigma_compiler.sqlite");
        create_table(func_name, "_changes_unique", " (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ");
        //create_table(func_name, "_changes_unique", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
        create_table(func_name, "_executions_unique", " (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ");
        create_table(func_name, "_executions_all", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
    }
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
         CALL(( open_sqlite( ARG_UNKNOWN(("enigma_compiler.sqlite")) ) )) ;
         CALL(( create_table( ARG_UNKNOWN((func_name)) ,  ARG_UNKNOWN(("_changes_unique")) ,  ARG_UNKNOWN((" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ")) ) )) ;
        //create_table(func_name, "_changes_unique", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
         CALL(( create_table( ARG_UNKNOWN((func_name)) ,  ARG_UNKNOWN(("_executions_unique")) ,  ARG_UNKNOWN((" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ")) ) )) ;
         CALL(( create_table( ARG_UNKNOWN((func_name)) ,  ARG_UNKNOWN(("_executions_all")) ,  ARG_UNKNOWN((" (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)")) ) )) ;
    }
}
#endif 
}
    
    void StaticFunctionData::flush_to_db() { 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_flush_to_db", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
 #endif 
 {
        if (all_function_executions.empty()) return;
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
            all_execution_id << "[";
            int tim=0;
            for(vector_of_change::iterator it2 = line_data.begin(); it2 != line_data.end(); ++it2) { //
               
                
                if (it2 != line_data.begin()) all_execution_id << ",";
                
                std::ostringstream current_line_names;
                std::ostringstream current_line_values;
                std::ostringstream current_line_types;
                std::ostringstream unique_special_id;
                    /*
                     All the changes that happened on this line during 1 execution of the function
                     */
                    
                    Change c = *it2;
                    
                    unique_special_id << "[" << c.line_num << "," << c.start_loc << "," << c.end_loc << ",\"" << c.value << "\", \""<< c.type_of_var << "\"]";
                    
                    tim = c.time_of_change;
                //std::cout << unique_special_id.str();
                long long insert_row_id=-1;
                
                if ( map_of_sqlrows.find(unique_special_id.str()) == map_of_sqlrows.end() ) {
                    // not found so add to db
                    int change_result = bind_change_sql(stmt_unique,unique_special_id.str(), current_line_types.str(), current_line_names.str(), current_line_values.str(), c.line_num, tim);
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
                
                execution_id << insert_row_id << ","; //unique_special_id.str();
                all_execution_id << "[" << insert_row_id << "," << tim << "]";
                    
                }
                all_execution_id << "]";
                int all_ex_result = bind_change_sql(stmt_ex_all,all_execution_id.str(), "", "", "", 0, tim); //this is per line
                bind_change_sql(stmt_ex_unique,execution_id.str(), "", "", "", 0, tim);
            
                
            }
        
        
        sqlite3_exec(ali__log__db, "END TRANSACTION", NULL, NULL, &sErrMsg);
        //sqlite3_close(ali__log__db);
        all_function_executions.clear(); //now that they have been writtern destroy them
    }
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
        if ( CALLR (71, 13, 44,  (all_function_executions.empty())) ) return;
        this->execution_number++;
        //std::cout << "\n\n" << func_name << "\n";
        
        char * sErrMsg = 0;
        sqlite3_stmt *stmt_unique , *stmt_ex_unique, *stmt_ex_all;
        
        
        //stmt_all = start_insert(func_name,"_changes_all"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        stmt_unique =  CALLR (80, 23, 105,  (start_insert( ARG_UNKNOWN((func_name)) , ARG_UNKNOWN(("_changes_unique")) , ARG_UNKNOWN((" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ))) ;
        stmt_ex_all =  CALLR (81, 23, 105,  (start_insert( ARG_UNKNOWN((func_name)) , ARG_UNKNOWN(("_executions_all")) , ARG_UNKNOWN((" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ))) ;
        stmt_ex_unique =  CALLR (82, 26, 111,  (start_insert( ARG_UNKNOWN((func_name)) , ARG_UNKNOWN(("_executions_unique")) , ARG_UNKNOWN((" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ))) ;
        
         CALLR (84, 9, 78,  (sqlite3_exec( ARG_UNKNOWN((ali__log__db)) ,  CALL_ARG(("BEGIN TRANSACTION")) , NULL, NULL,  CALL_ARG((&sErrMsg)) ))) ;
        
        for(std::vector<vector_of_change>::size_type execution = 0;  /*BinaryOp*/ LHS (86, 69, 78,  (execution))  !=  CALLR (86, 82, 112,  (all_function_executions.size())) ; execution++) {
            vector_of_change line_data = all_function_executions[execution];
            std::ostringstream special_id;
            std::ostringstream execution_id, all_execution_id;
            all_execution_id << "[";
            int tim=0;
            for(vector_of_change::iterator it2 = line_data.begin(); it2 != line_data.end(); ++it2) { //
               
                
                if (it2 != line_data.begin()) all_execution_id << ",";
                
                std::ostringstream current_line_names;
                std::ostringstream current_line_values;
                std::ostringstream current_line_types;
                std::ostringstream unique_special_id;
                    /*
                     All the changes that happened on this line during 1 execution of the function
                     */
                    
                    Change c = *it2;
                    
                    unique_special_id << "[" << c.line_num << "," << c.start_loc << "," << c.end_loc << ",\"" << c.value << "\", \""<< c.type_of_var << "\"]";
                    
                    tim =  /*(unsigned long)*/ LValueToRValue (109, 27, 43,  ( /*(int)*/ IntegralCast (109, 27, 43,  (c.time_of_change)) )) ;
                //std::cout << unique_special_id.str();
                long long insert_row_id=-1;
                
                if ( map_of_sqlrows.find(unique_special_id.str()) == map_of_sqlrows.end() ) {
                    // not found so add to db
                    int change_result = bind_change_sql(stmt_unique,unique_special_id.str(), current_line_types.str(), current_line_names.str(), current_line_values.str(), c.line_num, tim);
                    long long last_row;
                    if ( /*BinaryOp*/ LHS (117, 25, 38,  (change_result))  == SQLITE_DONE)
                    last_row =  CALLR (118, 32, 71,  (sqlite3_last_insert_rowid( ARG_UNKNOWN((ali__log__db)) ))) ;
                    else
                        last_row =  /*(long long)*/ IntegralCast (120, 36, 38,  (-1)) ;
                    
                    if ( /*BinaryOp*/ LHS (122, 25, 33,  (last_row))  != /*(long long)*/ IntegralCast (122, 36, 38,  (-1)) ) {
                        map_of_sqlrows[unique_special_id.str()] =  /*(long long)*/ LValueToRValue (123, 67, 75,  (last_row)) ; insert_row_id =  /*(long long)*/ LValueToRValue (123, 93, 101,  (last_row)) ;}
                    else
                    {map_of_sqlrows[unique_special_id.str()] =  /*(mapped_type)*/ IntegralCast (125, 64, 66,  (-1)) ; insert_row_id =  /*(long long)*/ IntegralCast (125, 84, 86,  (-1)) ;}
                } else {
                    insert_row_id =  /*(mapped_type)*/ LValueToRValue (127, 37, 76,  (map_of_sqlrows[unique_special_id.str()])) ;
                }
                
                execution_id << insert_row_id << ","; //unique_special_id.str();
                all_execution_id << "[" << insert_row_id << "," << tim << "]";
                    
                }
                all_execution_id << "]";
                int all_ex_result = bind_change_sql(stmt_ex_all,all_execution_id.str(), "", "", "", 0, tim); //this is per line
                 CALLR (136, 17, 87,  (bind_change_sql( ARG_UNKNOWN((stmt_ex_unique)) , ARG_UNKNOWN((execution_id.str())) ,  ARG_UNKNOWN(("")) ,  ARG_UNKNOWN(("")) ,  ARG_UNKNOWN(("")) ,  CALL_ARG((0)) ,  CALL_ARG((tim)) ))) ;
            
                
            }
        
        
         CALLR (142, 9, 76,  (sqlite3_exec( ARG_UNKNOWN((ali__log__db)) ,  CALL_ARG(("END TRANSACTION")) , NULL, NULL,  CALL_ARG((&sErrMsg)) ))) ;
        //sqlite3_close(ali__log__db);
         CALL(( all_function_executions.clear() )) ; //now that they have been writtern destroy them
    }
}
#endif 
}
    

template <class T> std::string TToStr(T& t)
{ 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_TToStr", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
 #endif 
 {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(t); } {
    std::ostringstream oss;
     /*BinaryOp*/ LHS (151, 5, 8,  (oss))  << t;
    return  CALLR (152, 12, 21,  ( /*(const class std::basic_string<char>)*/ NoOp (152, 12, 21,  (oss.str())) )) ;
}
}
#endif 
}

template <class T> std::string TToStr(T* t)
{ 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_TToStr", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
 #endif 
 {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(t); } {
    std::ostringstream oss;
     /*BinaryOp*/ LHS (158, 5, 8,  (oss))  << t;
    return  CALLR (159, 12, 21,  ( /*(const class std::basic_string<char>)*/ NoOp (159, 12, 21,  (oss.str())) )) ;
}
}
#endif 
}

unsigned long report_memory(void) { 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_report_memory", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
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
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
    struct task_basic_info info;
    mach_msg_type_number_t size = sizeof(info);
    kern_return_t kerr = task_info(mach_task_self(),
                                   TASK_BASIC_INFO,
                                   (task_info_t)&info,
                                   &size);
    if(  /*BinaryOp*/ LHS (169, 9, 13,  (kerr))  == KERN_SUCCESS ) {
        //printf("Memory in use (in bytes): %u", info.resident_size);
        return  /*(vm_size_t)*/ LValueToRValue (171, 16, 34,  (info.resident_size)) ;
    } else {
         CALL(( printf( CALL_ARG(("Error with task_info(): %s")) ,  CALL_ARG((mach_error_string(kerr))) ) )) ;
        return  /*(unsigned long)*/ IntegralCast (174, 16, 18,  (-1)) ;
    }
}
}
#endif 
}
#include <unistd.h>

void segfault_handler(int sig) { 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_segfault_handler", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
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
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(sig); } {
    void *array[10];
    size_t size;
    
    // get void*'s for all entries on the stack
    size =  CALLR (184, 12, 32,  ( /*(size_t)*/ IntegralCast (184, 12, 32,  (backtrace( CALL_ARG((array)) ,  CALL_ARG((10)) ))) )) ;
    
    // print out all the frames to stderr
     CALL(( fprintf(stderr,  CALL_ARG(("Error: signal %d:\n")) ,  CALL_ARG((sig)) ) )) ;
     CALL(( backtrace_symbols_fd( CALL_ARG((array)) ,  CALL_ARG((size)) , STDERR_FILENO) )) ;
     CALL(( exit( CALL_ARG((1)) ) )) ;
}
}
#endif 
}
    void install_handlers() { 
 #if NO_INSTRUMENT == false 
 static ali_clang_plugin_runtime::StaticFunctionData ali_function_db("/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp_install_handlers", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) 
 #endif 
 {
        signal(SIGSEGV, segfault_handler);
    }
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
         CALLR (192, 9, 42,  (signal(SIGSEGV,  CALL_ARG((segfault_handler)) ))) ;
    }
}
#endif 
}
}
