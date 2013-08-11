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
    int ALI_GLOBAL_MAX_EX = 300;

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
{ LOGPARAMETER(19,22,34,db_name); } {
        
        /* Open database */
        int rc = sqlite3_open(db_name.c_str(), &ali__log__db);
        if(   /*(int)*/ LValueToRValue (23, 13, 15,  (  /*(_Bool)*/ IntegralToBoolean (23, 13, 15,  (rc)) ))  ){
             (CALL( fprintf(stderr,   ARG_UNKNOWN (24, 29, 56,  ("Can't open database: %s\n")) ,   ARG_UNKNOWN (24, 58, 86,  (sqlite3_errmsg(ali__log__db))) ))) ;
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
{ LOGPARAMETER(29,23,35,table_name);  LOGPARAMETER(29,47,59,table_name_suffix);  LOGPARAMETER(29,78,90,schema); } {
        
        std::ostringstream oss;
        oss << "CREATE TABLE IF NOT EXISTS \"" << table_name << table_name_suffix  << "\" "<< schema;
        char * sErrMsg = 0;
         IntegralOrEnumType (34, 9, 76,  ( IntegralOrEnumType (34, 9, 76,  (sqlite3_exec(  ARG_UNKNOWN (34, 22, 34,  (ali__log__db)) ,   ARG_UNKNOWN (34, 36, 53,  (oss.str().c_str())) , NULL, NULL,   ARG_UNKNOWN (34, 67, 75,  (&sErrMsg)) ))) )) ;
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
{ LOGPARAMETER(38,33,45,table_name);  LOGPARAMETER(38,57,69,table_name_suffix);  LOGPARAMETER(38,88,100,schema); } {
        sqlite3_stmt * stmt;
        const char * tail = 0;
        
        std::ostringstream oss;
        oss << "INSERT INTO \"" << table_name << table_name_suffix  << "\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
         IntegralOrEnumType (44, 9, 95,  ( IntegralOrEnumType (44, 9, 95,  (sqlite3_prepare_v2(  ARG_UNKNOWN (44, 28, 40,  (ali__log__db)) ,    ARG_UNKNOWN (44, 43, 60,  (oss.str().c_str())) ,  IntegralOrEnumType (44, 62, 80,  (oss.str().length())) ,   ARG_UNKNOWN (44, 82, 87,  (&stmt)) ,   ARG_UNKNOWN (44, 89, 94,  (&tail)) ))) )) ;
        return stmt;
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
{ LOGPARAMETER(48,25,40,stmt);  LOGPARAMETER(48,46,58,unique_id);  LOGPARAMETER(48,69,81,types);  LOGPARAMETER(48,88,100,names);  LOGPARAMETER(48,107,119,values);  LOGPARAMETER(48,127,131,line_num);  LOGPARAMETER(48,141,145,tim); } {
         IntegralOrEnumType (49, 9, 76,  (sqlite3_bind_text(  ARG_UNKNOWN (49, 27, 31,  (stmt)) ,  IntegralOrEnumType (49, 33, 34,  (1)) ,   ARG_UNKNOWN (49, 36, 53,  (unique_id.c_str())) ,  IntegralOrEnumType (49, 55, 57,  (-1)) , SQLITE_TRANSIENT))) ;
         IntegralOrEnumType (50, 9, 72,  (sqlite3_bind_text(  ARG_UNKNOWN (50, 27, 31,  (stmt)) ,  IntegralOrEnumType (50, 33, 34,  (2)) ,   ARG_UNKNOWN (50, 36, 49,  (types.c_str())) ,  IntegralOrEnumType (50, 51, 53,  (-1)) , SQLITE_TRANSIENT))) ;
         IntegralOrEnumType (51, 9, 72,  (sqlite3_bind_text(  ARG_UNKNOWN (51, 27, 31,  (stmt)) ,  IntegralOrEnumType (51, 33, 34,  (3)) ,   ARG_UNKNOWN (51, 36, 49,  (names.c_str())) ,  IntegralOrEnumType (51, 51, 53,  (-1)) , SQLITE_TRANSIENT))) ;
         IntegralOrEnumType (52, 9, 73,  (sqlite3_bind_text(  ARG_UNKNOWN (52, 27, 31,  (stmt)) ,  IntegralOrEnumType (52, 33, 34,  (4)) ,   ARG_UNKNOWN (52, 36, 50,  (values.c_str())) ,  IntegralOrEnumType (52, 52, 54,  (-1)) , SQLITE_TRANSIENT))) ;
         IntegralOrEnumType (53, 9, 44,  (sqlite3_bind_int(  ARG_UNKNOWN (53, 26, 30,  (stmt)) ,  IntegralOrEnumType (53, 32, 33,  (5)) ,  IntegralOrEnumType (53, 35, 43,  (line_num)) ))) ;
         IntegralOrEnumType (54, 9, 39,  (sqlite3_bind_int(  ARG_UNKNOWN (54, 26, 30,  (stmt)) ,  IntegralOrEnumType (54, 32, 33,  (6)) ,  IntegralOrEnumType (54, 35, 38,  (tim)) ))) ;
        
        int result = sqlite3_step(stmt);
         IntegralOrEnumType (57, 9, 37,  (sqlite3_clear_bindings(  ARG_UNKNOWN (57, 32, 36,  (stmt)) ))) ;
         IntegralOrEnumType (58, 9, 28,  (sqlite3_reset(  ARG_UNKNOWN (58, 23, 27,  (stmt)) ))) ;
        return result;
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
        create_table(func_name, "_executions_unique", " (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Start_Time INTEGER, End_Time INTEGER) ");
        create_table(func_name, "_executions_all", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Start_Time INTEGER, End_Time INTEGER PRIMARY KEY)");
        created_database=true;
    }
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
         (CALL( open_sqlite( StringType (63, 21, 45,  ("enigma_compiler.sqlite")) ))) ;
         (CALL( create_table( StringType (64, 22, 31,  (func_name)) ,  StringType (64, 33, 50,  ("_changes_unique")) ,  StringType (64, 52, 175,  (" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ")) ))) ;
        //create_table(func_name, "_changes_unique", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
         (CALL( create_table( StringType (66, 22, 31,  (func_name)) ,  StringType (66, 33, 53,  ("_executions_unique")) ,  StringType (66, 55, 181,  (" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Start_Time INTEGER, End_Time INTEGER) ")) ))) ;
         (CALL( create_table( StringType (67, 22, 31,  (func_name)) ,  StringType (67, 33, 50,  ("_executions_all")) ,  StringType (67, 52, 177,  (" (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Start_Time INTEGER, End_Time INTEGER PRIMARY KEY)")) ))) ;
        created_database=true;
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
                    /*
                     All the changes that happened on this line during 1 execution of the function
                     */
                    
                
                    
                    unique_special_id << "[" << c.line_num << "," << c.start_loc << "," << c.end_loc << ",\"" << c.value << "\", \""<< c.type_of_var << "\"]";
                    
                    end_time = c.time_of_change;
                //std::cout << unique_special_id.str();
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
                
                execution_id << insert_row_id << ","; //unique_special_id.str();
                    
                }
            
                int unique_ex_result = bind_change_sql(stmt_ex_unique,execution_id.str(), "", "", "", start_time, end_time);
           
                unsigned long unique_that_was_just_inserted = sqlite3_last_insert_rowid(ali__log__db);
                //all_execution_id << unique_that_was_just_inserted;
                bind_change_sql(stmt_ex_all,execution_id.str()/*all_execution_id.str()*/, "", "", "", start_time, end_time);
            
            
                
            }
        
        
        sqlite3_exec(ali__log__db, "END TRANSACTION", NULL, NULL, &sErrMsg);
        //sqlite3_close(ali__log__db);
        all_function_executions.clear(); //now that they have been writtern destroy them
    }
 #if NO_INSTRUMENT == false 
 else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
        if ( IntegralOrEnumType (72, 13, 44,  (all_function_executions.empty())) ) return;
        if (!  /*(_Bool)*/ LValueToRValue (73, 14, 30,  (created_database)) )  (CALL( create_tables())) ;
        this->execution_number++;
        //std::cout << "\n\n" << func_name << "\n";
        
        char * sErrMsg = 0;
        sqlite3_stmt *stmt_unique , *stmt_ex_unique, *stmt_ex_all;
        
        
        //stmt_all = start_insert(func_name,"_changes_all"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
         ExprWithCleanups (82, 9, 105,  (stmt_unique =   CALLR (82, 23, 105,  (start_insert( StringType (82, 36, 45,  (func_name)) , StringType (82, 46, 63,  ("_changes_unique")) , StringType (82, 64, 104,  (" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ))) )) ;
         ExprWithCleanups (83, 9, 105,  (stmt_ex_all =   CALLR (83, 23, 105,  (start_insert( StringType (83, 36, 45,  (func_name)) , StringType (83, 46, 63,  ("_executions_all")) , StringType (83, 64, 104,  (" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ))) )) ;
         ExprWithCleanups (84, 9, 111,  (stmt_ex_unique =   CALLR (84, 26, 111,  (start_insert( StringType (84, 39, 48,  (func_name)) , StringType (84, 49, 69,  ("_executions_unique")) , StringType (84, 70, 110,  (" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ))) )) ;
        
         IntegralOrEnumType (86, 9, 78,  (sqlite3_exec(  ARG_UNKNOWN (86, 22, 34,  (ali__log__db)) ,   ARG_UNKNOWN (86, 36, 55,  ("BEGIN TRANSACTION")) , NULL, NULL,   ARG_UNKNOWN (86, 69, 77,  (&sErrMsg)) ))) ;
        
        for(std::vector<vector_of_change>::size_type execution = 0;  IntegralOrEnumType (88, 69, 78,  (execution))  !=  IntegralOrEnumType (88, 82, 112,  (all_function_executions.size())) ; execution++) {
            vector_of_change line_data = all_function_executions[execution];
            std::ostringstream special_id;
            std::ostringstream execution_id, all_execution_id;
            
            unsigned long start_time=0,end_time=0;
            for(vector_of_change::iterator it2 = line_data.begin(); it2 != line_data.end(); ++it2) { //
               Change c = *it2;
                
                if (it2 == line_data.begin())
                start_time=  /*(unsigned long)*/ LValueToRValue (98, 28, 44,  (c.time_of_change)) ;
                
                std::ostringstream current_line_names;
                std::ostringstream current_line_values;
                std::ostringstream current_line_types;
                std::ostringstream unique_special_id;
                    /*
                     All the changes that happened on this line during 1 execution of the function
                     */
                    
                
                    
                    unique_special_id << "[" << c.line_num << "," << c.start_loc << "," << c.end_loc << ",\"" << c.value << "\", \""<< c.type_of_var << "\"]";
                    
                    end_time =   /*(unsigned long)*/ LValueToRValue (112, 32, 48,  (c.time_of_change)) ;
                //std::cout << unique_special_id.str();
                long long insert_row_id=-1;
                
                if (  IntegralOrEnumType (116, 22, 90,  (map_of_sqlrows.find(unique_special_id.str()) == map_of_sqlrows.end()))  ) {
                    // not found so add to db
                    int change_result = bind_change_sql(stmt_unique,unique_special_id.str(), current_line_types.str(), current_line_names.str(), current_line_values.str(), c.line_num, end_time);
                    long long last_row;
                    if ( IntegralOrEnumType (120, 25, 38,  (change_result))  == SQLITE_DONE)
                    last_row =  IntegralOrEnumType (121, 32, 71,  (sqlite3_last_insert_rowid(  ARG_UNKNOWN (121, 58, 70,  (ali__log__db)) ))) ;
                    else
                        last_row = -1;
                    
                    if ( IntegralOrEnumType (125, 25, 33,  (last_row))  !=-1) {
                         IntegralOrEnumType (126, 25, 75,  (map_of_sqlrows[unique_special_id.str()] =   /*(long long)*/ LValueToRValue (126, 67, 75,  (last_row)) )) ; insert_row_id =   /*(long long)*/ LValueToRValue (126, 93, 101,  (last_row)) ;}
                    else
                    { IntegralOrEnumType (128, 22, 66,  (map_of_sqlrows[unique_special_id.str()] = -1)) ; insert_row_id = -1;}
                } else {
                     IntegralOrEnumType (130, 21, 76,  (insert_row_id =   /*(mapped_type)*/ LValueToRValue (130, 37, 76,  (map_of_sqlrows[unique_special_id.str()])) )) ;
                }
                
                execution_id << insert_row_id << ","; //unique_special_id.str();
                    
                }
            
                int unique_ex_result = bind_change_sql(stmt_ex_unique,execution_id.str(), "", "", "", start_time, end_time);
           
                unsigned long unique_that_was_just_inserted = sqlite3_last_insert_rowid(ali__log__db);
                //all_execution_id << unique_that_was_just_inserted;
                 IntegralOrEnumType (141, 17, 124,  ( IntegralOrEnumType (141, 17, 124,  (bind_change_sql(  ARG_UNKNOWN (141, 33, 44,  (stmt_ex_all)) , StringType (141, 45, 63,  (execution_id.str())) /*all_execution_id.str()*/,  StringType (141, 91, 93,  ("")) ,  StringType (141, 95, 97,  ("")) ,  StringType (141, 99, 101,  ("")) ,  IntegralOrEnumType (141, 103, 113,  (start_time)) ,  IntegralOrEnumType (141, 115, 123,  (end_time)) ))) )) ;
            
            
                
            }
        
        
         IntegralOrEnumType (148, 9, 76,  (sqlite3_exec(  ARG_UNKNOWN (148, 22, 34,  (ali__log__db)) ,   ARG_UNKNOWN (148, 36, 53,  ("END TRANSACTION")) , NULL, NULL,   ARG_UNKNOWN (148, 67, 75,  (&sErrMsg)) ))) ;
        //sqlite3_close(ali__log__db);
         (CALL( all_function_executions.clear())) ; //now that they have been writtern destroy them
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
{ LOGPARAMETER(154,39,42,t); } {
    std::ostringstream oss;
      /*BinaryOp*/ LHS (157, 5, 8,  (oss))  << t;
    return oss.str();
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
{ LOGPARAMETER(161,39,42,t); } {
    std::ostringstream oss;
      /*BinaryOp*/ LHS (164, 5, 8,  (oss))  << t;
    return oss.str();
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
    if(  IntegralOrEnumType (175, 9, 13,  (kerr))  == KERN_SUCCESS ) {
        //printf("Memory in use (in bytes): %u", info.resident_size);
        return info.resident_size;
    } else {
         (CALL( printf(  ARG_UNKNOWN (179, 16, 44,  ("Error with task_info(): %s")) ,   ARG_UNKNOWN (179, 46, 69,  (mach_error_string(kerr))) ))) ;
        return -1;
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
{ LOGPARAMETER(185,23,27,sig); } {
    void *array[10];
    size_t size;
    
    // get void*'s for all entries on the stack
    size =  IntegralOrEnumType (190, 12, 32,  (  /*(size_t)*/ IntegralCast (190, 12, 32,  (backtrace(  ARG_UNKNOWN (190, 22, 27,  (array)) ,  IntegralOrEnumType (190, 29, 31,  (10)) ))) )) ;
    
    // print out all the frames to stderr
     (CALL( fprintf(stderr,   ARG_UNKNOWN (193, 21, 42,  ("Error: signal %d:\n")) ,  IntegralOrEnumType (193, 44, 47,  (sig)) ))) ;
     (CALL( backtrace_symbols_fd(  ARG_UNKNOWN (194, 26, 31,  (array)) ,  IntegralOrEnumType (194, 33, 37,  (size)) , STDERR_FILENO))) ;
     (CALL( exit( IntegralOrEnumType (195, 10, 11,  (1)) ))) ;
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
          CALLR (198, 9, 42,  (signal(SIGSEGV,   ARG_UNKNOWN (198, 25, 41,  (segfault_handler)) ))) ;
    }
}
#endif 
}
}

