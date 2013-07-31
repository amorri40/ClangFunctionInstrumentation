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

    void open_sqlite(std::string db_name) { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
        
        /* Open database */
        int rc = sqlite3_open(db_name.c_str(), &ali__log__db);
        if( rc ){
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ali__log__db));
            exit(0);
        }
    }
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(db_name); } {
        
        /* Open database */
         /*DECL*/ int rc = sqlite3_open(db_name.c_str(), &ali__log__db);
        if( rc ){
             CALLR(( fprintf(stderr,  CALL_ARG(("Can't open database: %s\n")) ,  CALLR((  CALL_ARG((sqlite3_errmsg( ARG_UNKNOWN((ali__log__db)) )))  )) ) )) ;
             CALLR(( exit( CALL_ARG((0)) ) )) ;
        }
    }
}
#endif 
}
    
    void create_table(std::string table_name, std::string table_name_suffix, std::string schema) { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
        
        std::ostringstream oss;
        oss << "CREATE TABLE IF NOT EXISTS \"" << table_name << table_name_suffix  << "\" "<< schema;
        char * sErrMsg = 0;
        sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
    }
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(table_name);  LOGPARAMETER(table_name_suffix);  LOGPARAMETER(schema); } {
        
         /*DECL*/ std::ostringstream oss;
        OPERATOR_LHS_ARG (31, 9, 12,  (OPERATOR_LHS_ARG (31, 9, 47,  (OPERATOR_LHS_ARG (31, 9, 61,  (OPERATOR_LHS_ARG (31, 9, 82,  (OPERATOR_LHS_ARG (31, 9, 92,  (oss))  <<  OPERATOR_RHS_ARG_CANONICAL (31, 16, 47,  ("CREATE TABLE IF NOT EXISTS \"")) ))  << OPERATOR_LHS_ARG (31, 51, 61,  (table_name)) ))  << OPERATOR_LHS_ARG (31, 65, 82,  (table_name_suffix)) ))   <<  OPERATOR_RHS_ARG_CANONICAL (31, 87, 92,  ("\" ")) )) << OPERATOR_LHS_ARG (31, 95, 101,  (schema)) ;
         /*DECL*/ char * sErrMsg = 0;
         CALLR(( sqlite3_exec( ARG_UNKNOWN((ali__log__db)) ,  MEMBER_CALL((  ARG_UNKNOWN((oss.str().c_str()))  )) , NULL, NULL,  CALL_ARG((&sErrMsg)) ) )) ;
    }
}
#endif 
}
    
    sqlite3_stmt * start_insert(std::string table_name, std::string table_name_suffix, std::string schema) { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
        char sSQL_all [BUFFER_SIZE] = "\0";
        sqlite3_stmt * stmt;
        const char * tail = 0;
        
        std::ostringstream oss;
        oss << "INSERT INTO \"" << table_name << table_name_suffix  << "\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
        sprintf(sSQL_all, oss.str().c_str());
        sqlite3_prepare_v2(ali__log__db,  sSQL_all, BUFFER_SIZE, &stmt, &tail);
        return stmt;
    }
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(table_name);  LOGPARAMETER(table_name_suffix);  LOGPARAMETER(schema); } {
         /*DECL*/ char sSQL_all [BUFFER_SIZE] = "\0";
         /*DECL*/ sqlite3_stmt * stmt;
         /*DECL*/ const char * tail = 0;
        
         /*DECL*/ std::ostringstream oss;
        OPERATOR_LHS_ARG (42, 9, 12,  (OPERATOR_LHS_ARG (42, 9, 32,  (OPERATOR_LHS_ARG (42, 9, 46,  (OPERATOR_LHS_ARG (42, 9, 67,  (oss))  <<  OPERATOR_RHS_ARG_CANONICAL (42, 16, 32,  ("INSERT INTO \"")) ))  << OPERATOR_LHS_ARG (42, 36, 46,  (table_name)) ))  << OPERATOR_LHS_ARG (42, 50, 67,  (table_name_suffix)) ))   <<  OPERATOR_RHS_ARG_CANONICAL (42, 72, 114,  ("\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ;
         CALLR(( sprintf( CALL_ARG((sSQL_all)) ,  MEMBER_CALL((  ARG_UNKNOWN((oss.str().c_str()))  )) ) )) ;
         CALLR(( sqlite3_prepare_v2( ARG_UNKNOWN((ali__log__db)) ,   CALL_ARG((sSQL_all)) , BUFFER_SIZE,  ARG_UNKNOWN((&stmt)) ,  CALL_ARG((&tail)) ) )) ;
        return stmt;
    }
}
#endif 
}
    
    void bind_change_sql(sqlite3_stmt * stmt, std::string unique_id, std::string types, std::string names, std::string values, int line_num, int tim ) { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
        sqlite3_bind_text(stmt, 1, unique_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, types.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, names.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, values.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, line_num);
        sqlite3_bind_int(stmt, 6, tim);
        sqlite3_step(stmt);
    }
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{ LOGPARAMETER(stmt);  LOGPARAMETER(unique_id);  LOGPARAMETER(types);  LOGPARAMETER(names);  LOGPARAMETER(values);  LOGPARAMETER(line_num);  LOGPARAMETER(tim); } {
         CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt)) ,  CALL_ARG((1)) ,  MEMBER_CALL((  ARG_UNKNOWN((unique_id.c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
         CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt)) ,  CALL_ARG((2)) ,  MEMBER_CALL((  ARG_UNKNOWN((types.c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
         CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt)) ,  CALL_ARG((3)) ,  MEMBER_CALL((  ARG_UNKNOWN((names.c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
         CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt)) ,  CALL_ARG((4)) ,  MEMBER_CALL((  ARG_UNKNOWN((values.c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
         CALLR(( sqlite3_bind_int( ARG_UNKNOWN((stmt)) ,  CALL_ARG((5)) ,  CALL_ARG((line_num)) ) )) ;
         CALLR(( sqlite3_bind_int( ARG_UNKNOWN((stmt)) ,  CALL_ARG((6)) ,  CALL_ARG((tim)) ) )) ;
         CALLR(( sqlite3_step( ARG_UNKNOWN((stmt)) ) )) ;
    }
}
#endif 
}

    void StaticFunctionData::create_tables() { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
        open_sqlite("enigma_compiler.sqlite");
        create_table(func_name, "_changes_unique", " (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ");
        //create_table(func_name, "_changes_all", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
        create_table(func_name, "_executions_unique", " (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ");
        create_table(func_name, "_executions_all", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
    }
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
         CALLR(( open_sqlite( ARG_UNKNOWN(("enigma_compiler.sqlite")) ) )) ;
         CALLR(( create_table( MEMBER_EXPR((  ARG_UNKNOWN((func_name))  )) ,  ARG_UNKNOWN(("_changes_unique")) ,  ARG_UNKNOWN((" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ")) ) )) ;
        //create_table(func_name, "_changes_all", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
         CALLR(( create_table( MEMBER_EXPR((  ARG_UNKNOWN((func_name))  )) ,  ARG_UNKNOWN(("_executions_unique")) ,  ARG_UNKNOWN((" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ")) ) )) ;
         CALLR(( create_table( MEMBER_EXPR((  ARG_UNKNOWN((func_name))  )) ,  ARG_UNKNOWN(("_executions_all")) ,  ARG_UNKNOWN((" (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)")) ) )) ;
    }
}
#endif 
}
    
    void StaticFunctionData::flush_to_db() { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
        if (all_function_executions.empty()) return;
        
        char * sErrMsg = 0;
        sqlite3_stmt * stmt_all, *stmt_unique , *stmt_ex_unique, *stmt_ex_all;
        const char * tail = 0;
        const char * tail_unique = 0;
        char sSQL_all [BUFFER_SIZE] = "\0";
        char sSQL_unique [BUFFER_SIZE] = "\0";
        
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
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
        if ( MEMBER_CALL(( all_function_executions.empty() )) ) return;
        
         /*DECL*/ char * sErrMsg = 0;
        sqlite3_stmt * stmt_all, *stmt_unique , *stmt_ex_unique, *stmt_ex_all;
         /*DECL*/ const char * tail = 0;
         /*DECL*/ const char * tail_unique = 0;
         /*DECL*/ char sSQL_all [BUFFER_SIZE] = "\0";
         /*DECL*/ char sSQL_unique [BUFFER_SIZE] = "\0";
        
         MEMBER_CALL(( create_tables() )) ;
        
        //stmt_all = start_insert(func_name,"_changes_all"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        LHS (79, 9, 20,  (stmt_unique))  =  CALLR(( start_insert( MEMBER_EXPR((  ARG_UNKNOWN((func_name))  )) , ARG_UNKNOWN(("_changes_unique")) , ARG_UNKNOWN((" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ) )) ;
        LHS (80, 9, 20,  (stmt_ex_all))  =  CALLR(( start_insert( MEMBER_EXPR((  ARG_UNKNOWN((func_name))  )) , ARG_UNKNOWN(("_executions_all")) , ARG_UNKNOWN((" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ) )) ;
        LHS (81, 9, 23,  (stmt_ex_unique))  =  CALLR(( start_insert( MEMBER_EXPR((  ARG_UNKNOWN((func_name))  )) , ARG_UNKNOWN(("_executions_unique")) , ARG_UNKNOWN((" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ) )) ;
        
         CALLR(( sqlite3_exec( ARG_UNKNOWN((ali__log__db)) ,  CALL_ARG(("BEGIN TRANSACTION")) , NULL, NULL,  CALL_ARG((&sErrMsg)) ) )) ;
        
        for( /*DECL*/ std::vector<vector_of_change>::size_type execution = 0; LHS (85, 69, 78,  (execution))  !=  MEMBER_CALL(( all_function_executions.size() )) ; execution++) {
             /*DECL*/ vector_of_change line_data = all_function_executions[execution];
             /*DECL*/ std::ostringstream special_id;
             /*DECL*/ std::ostringstream execution_id;
             /*DECL*/ int tim=0;
            for( /*DECL*/ vector_of_change::iterator it2 = line_data.begin(); OPERATOR_LHS_ARG (90, 69, 72,  (it2))  !=  MEMBER_CALL(( OPERATOR_LHS_ARG (90, 76, 91,  (line_data.end()))  )) ; ++OPERATOR_LHS_ARG (90, 95, 98,  (it2)) ) { //
               // int line_num = line_data.l;
               // std::cout << "  " << line_num << ": ";
               // special_id << line_num << "_";
                //std::string previous_r_value;
                
                
                 /*DECL*/ std::ostringstream current_line_names;
                 /*DECL*/ std::ostringstream current_line_values;
                 /*DECL*/ std::ostringstream current_line_types;
                
                //unique_special_id << special_id.str();
                //for(std::vector<Change>::reverse_iterator it2 = line->second.rbegin(); it2 != line->second.rend(); ++it2) {
                     /*DECL*/ std::ostringstream unique_special_id;
                    /*
                     All the changes that happened on this line during 1 execution of the function
                     */
                    
                     /*DECL*/ Change c = *it2;
                    
                    OPERATOR_LHS_ARG (110, 21, 38,  (OPERATOR_LHS_ARG (110, 21, 45,  (OPERATOR_LHS_ARG (110, 21, 59,  (OPERATOR_LHS_ARG (110, 21, 66,  (OPERATOR_LHS_ARG (110, 21, 81,  (OPERATOR_LHS_ARG (110, 21, 88,  (OPERATOR_LHS_ARG (110, 21, 101,  (OPERATOR_LHS_ARG (110, 21, 108,  (OPERATOR_LHS_ARG (110, 21, 119,  (OPERATOR_LHS_ARG (110, 21, 127,  (OPERATOR_LHS_ARG (110, 21, 143,  (unique_special_id))  <<  OPERATOR_RHS_ARG_CANONICAL (110, 42, 45,  ("(")) ))  <<  MEMBER_EXPR((  OPERATOR_RHS_ARG_CANONICAL (110, 49, 59,  (c.line_num)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL (110, 63, 66,  (":")) ))  <<  MEMBER_EXPR((  OPERATOR_RHS_ARG_CANONICAL (110, 70, 81,  (c.start_loc)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL (110, 85, 88,  (":")) ))  <<  MEMBER_EXPR((  OPERATOR_RHS_ARG_CANONICAL (110, 92, 101,  (c.end_loc)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL (110, 105, 108,  ("=")) ))  <<  MEMBER_EXPR(( OPERATOR_LHS_ARG (110, 112, 119,  (c.value)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL (110, 123, 127,  (" (")) )) <<  MEMBER_EXPR(( OPERATOR_LHS_ARG (110, 130, 143,  (c.type_of_var)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL (110, 147, 150,  (")")) ;
                    //current_line_values << c.value;
                    //current_line_names << "";
                    //unique_special_id << current_line_types.str();
                    
                    LHS (115, 21, 24,  (tim))  =  MEMBER_EXPR(( c.time_of_change )) ;
                    
                    //bind_change_sql(stmt_all,unique_special_id.str(), current_line_types.str(), current_line_names.str(), current_line_values.str(), c.line_num, tim);
                     CALLR(( bind_change_sql( ARG_UNKNOWN((stmt_unique)) , MEMBER_CALL((  ARG_UNKNOWN((unique_special_id.str()))  )) ,  MEMBER_CALL((  ARG_UNKNOWN((current_line_types.str()))  )) ,  MEMBER_CALL((  ARG_UNKNOWN((current_line_names.str()))  )) ,  MEMBER_CALL((  ARG_UNKNOWN((current_line_values.str()))  )) ,  MEMBER_EXPR((  CALL_ARG((c.line_num))  )) ,  CALL_ARG((tim)) ) )) ;
                    
                    OPERATOR_LHS_ARG (120, 21, 33,  (execution_id))  <<  MEMBER_CALL(( OPERATOR_LHS_ARG (120, 37, 60,  (unique_special_id.str()))  )) ;
                    
                }
                
                 CALLR(( bind_change_sql( ARG_UNKNOWN((stmt_ex_all)) , MEMBER_CALL((  ARG_UNKNOWN((execution_id.str()))  )) ,  ARG_UNKNOWN(("")) ,  ARG_UNKNOWN(("")) ,  ARG_UNKNOWN(("")) ,  CALL_ARG((0)) ,  CALL_ARG((tim)) ) )) ; //this is per line
                 CALLR(( bind_change_sql( ARG_UNKNOWN((stmt_ex_unique)) , MEMBER_CALL((  ARG_UNKNOWN((execution_id.str()))  )) ,  ARG_UNKNOWN(("")) ,  ARG_UNKNOWN(("")) ,  ARG_UNKNOWN(("")) ,  CALL_ARG((0)) ,  CALL_ARG((tim)) ) )) ;
                
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
                 CALLR(( sqlite3_clear_bindings( ARG_UNKNOWN((stmt_unique)) ) )) ;
                 CALLR(( sqlite3_reset( ARG_UNKNOWN((stmt_unique)) ) )) ;
                
            }
        
        
         CALLR(( sqlite3_exec( ARG_UNKNOWN((ali__log__db)) ,  CALL_ARG(("END TRANSACTION")) , NULL, NULL,  CALL_ARG((&sErrMsg)) ) )) ;
         CALLR(( sqlite3_close( ARG_UNKNOWN((ali__log__db)) ) )) ;
         MEMBER_CALL(( all_function_executions.clear() )) ; //now that they have been writtern destroy them
    }
}
#endif 
}
    

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
    LHS (161, 5, 8,  (oss))  << t;
    return  MEMBER_CALL(( oss.str() )) ;
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
    LHS (168, 5, 8,  (oss))  << t;
    return  MEMBER_CALL(( oss.str() )) ;
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
     /*DECL*/ kern_return_t kerr = task_info(mach_task_self(),
                                   TASK_BASIC_INFO,
                                   (task_info_t)&info,
                                   &size);
    if( LHS (179, 9, 13,  (kerr))  == KERN_SUCCESS ) {
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
    LHS (194, 5, 9,  (size))  =  CALLR(( backtrace( CALL_ARG((array)) ,  CALL_ARG((10)) ) )) ;
    
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
