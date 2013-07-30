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



    void StaticFunctionData::create_tables() { 
 #if NO_INSTRUMENT == false 
 if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) 
 #endif 
 {
        char * sErrMsg = 0;
        std::ostringstream oss;
        oss << "CREATE TABLE IF NOT EXISTS \"" << func_name  << "_unique\" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ";
        sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
        oss.str(""); //clear string stream
        oss << "CREATE TABLE IF NOT EXISTS \"" << func_name  << "_all\" (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)";
        sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
    }
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
         /*DECL*/ char * sErrMsg = 0;
         /*DECL*/ std::ostringstream oss;
         OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG((oss))  <<  OPERATOR_RHS_ARG_CANONICAL(("CREATE TABLE IF NOT EXISTS \"")) ))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((func_name)) ))  ))   <<  OPERATOR_RHS_ARG_CANONICAL(("_unique\" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ")) ;
         CALLR(( sqlite3_exec( ARG_UNKNOWN((ali__log__db)) ,  MEMBER_CALL((  ARG_UNKNOWN((oss.str().c_str()))  )) , NULL, NULL,  CALL_ARG((&sErrMsg)) ) )) ;
         MEMBER_CALL(( oss.str( ARG_UNKNOWN(("")) ) )) ; //clear string stream
         OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG((oss))  <<  OPERATOR_RHS_ARG_CANONICAL(("CREATE TABLE IF NOT EXISTS \"")) ))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((func_name)) ))  ))   <<  OPERATOR_RHS_ARG_CANONICAL(("_all\" (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)")) ;
         CALLR(( sqlite3_exec( ARG_UNKNOWN((ali__log__db)) ,  MEMBER_CALL((  ARG_UNKNOWN((oss.str().c_str()))  )) , NULL, NULL,  CALL_ARG((&sErrMsg)) ) )) ;
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
                    
                    current_line_types << c.type_of_var;
                    current_line_values << c.value;
                    
                    if (c.type == CHANGE_LHS) {
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
                    }
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
 #if NO_INSTRUMENT == false 
 else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); 
{} {
        
        if ( MEMBER_CALL(( all_function_executions.empty() )) ) return;
        /* Open database */
         /*DECL*/ int rc = sqlite3_open("enigma_compiler.sqlite", &ali__log__db);
        if( rc ){
             CALLR(( fprintf(stderr,  CALL_ARG(("Can't open database: %s\n")) ,  CALLR((  CALL_ARG((sqlite3_errmsg( ARG_UNKNOWN((ali__log__db)) )))  )) ) )) ;
             CALLR(( exit( CALL_ARG((0)) ) )) ;
        }
         OPERATOR_LHS_ARG((std::cout))  <<  OPERATOR_RHS_ARG_CANONICAL(("flush_to_db")) ;
         /*DECL*/ char * sErrMsg = 0;
        sqlite3_stmt * stmt_all, *stmt_unique;
         /*DECL*/ const char * tail = 0;
         /*DECL*/ const char * tail_unique = 0;
         /*DECL*/ char sSQL_all [BUFFER_SIZE] = "\0";
         /*DECL*/ char sSQL_unique [BUFFER_SIZE] = "\0";
         /*DECL*/ std::ostringstream oss;
         MEMBER_CALL(( create_tables() )) ;
        
         OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG((oss))  <<  OPERATOR_RHS_ARG_CANONICAL(("INSERT INTO \"")) ))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((func_name)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL(("_all\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ;
         CALLR(( sprintf( CALL_ARG((sSQL_all)) ,  MEMBER_CALL((  ARG_UNKNOWN((oss.str().c_str()))  )) ) )) ;
         CALLR(( sqlite3_prepare_v2( ARG_UNKNOWN((ali__log__db)) ,   CALL_ARG((sSQL_all)) , BUFFER_SIZE,  ARG_UNKNOWN((&stmt_all)) ,  CALL_ARG((&tail)) ) )) ;
         MEMBER_CALL(( oss.str( ARG_UNKNOWN(("")) ) )) ;
         OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG((oss))  <<  OPERATOR_RHS_ARG_CANONICAL(("INSERT INTO \"")) ))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((func_name)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL(("_unique\" VALUES (@SP, @TY, @NA, @VA, @LI, @TI)")) ;
         CALLR(( sprintf( CALL_ARG((sSQL_unique)) ,  MEMBER_CALL((  ARG_UNKNOWN((oss.str().c_str()))  )) ) )) ;
         CALLR(( sqlite3_prepare_v2( ARG_UNKNOWN((ali__log__db)) ,   CALL_ARG((sSQL_unique)) , BUFFER_SIZE,  ARG_UNKNOWN((&stmt_unique)) ,  CALL_ARG((&tail_unique)) ) )) ;
        
        
         CALLR(( sqlite3_exec( ARG_UNKNOWN((ali__log__db)) ,  CALL_ARG(("BEGIN TRANSACTION")) , NULL, NULL,  CALL_ARG((&sErrMsg)) ) )) ;
         OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG((std::cout))  <<  OPERATOR_RHS_ARG_CANONICAL(("\nFunction: ")) ))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((func_name)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL((" \n")) ;
        
        
        for( /*DECL*/ std::vector<map_of_vector_of_change>::size_type execution = 0;  LHS(( execution))  !=  MEMBER_CALL(( all_function_executions.size() )) ; execution++) {
             /*DECL*/ map_of_vector_of_change line_data = all_function_executions[execution];
             /*DECL*/ std::ostringstream special_id;
            for( /*DECL*/ map_of_vector_of_change::iterator line = line_data.begin();  OPERATOR_LHS_ARG((line))  !=  MEMBER_CALL((  OPERATOR_LHS_ARG((line_data.end()))  )) ; ++ OPERATOR_LHS_ARG((line)) ) { //for(map_of_vector_of_string::size_type i = 0; i != line_data.size(); i++) {
                //if (line_data[i].empty()) continue;
                 /*DECL*/ int line_num = (line->first);
                 OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG((std::cout))  <<  OPERATOR_RHS_ARG_CANONICAL(("  ")) ))  <<  OPERATOR_RHS_ARG_CANONICAL((line_num)) ))  <<  OPERATOR_RHS_ARG_CANONICAL((": ")) ;
                 OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG((special_id))  <<  OPERATOR_RHS_ARG_CANONICAL((line_num)) ))  <<  OPERATOR_RHS_ARG_CANONICAL(("_")) ;
                 /*DECL*/ std::string previous_r_value;
                 /*DECL*/ std::ostringstream unique_special_id;
                 /*DECL*/ std::ostringstream current_line_names;
                 /*DECL*/ std::ostringstream current_line_values;
                 /*DECL*/ std::ostringstream current_line_types;
                 /*DECL*/ int tim=0;
                 OPERATOR_LHS_ARG((unique_special_id))  <<  MEMBER_CALL((  OPERATOR_LHS_ARG((special_id.str()))  )) ;
                for( /*DECL*/ std::vector<Change>::reverse_iterator it2 = line->second.rbegin();  OPERATOR_LHS_ARG((it2))  !=  MEMBER_CALL((  OPERATOR_LHS_ARG((line->second.rend()))  )) ; ++ OPERATOR_LHS_ARG((it2)) ) {
                    /*
                     All the changes that happened on this line during 1 execution of the function
                     */
                    
                    //unique_special_id << special_id.str();
                     /*DECL*/ Change c = *it2;
                    
                     OPERATOR_LHS_ARG((current_line_types))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((c.type_of_var))  )) ;
                     OPERATOR_LHS_ARG((current_line_values))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((c.value))  )) ;
                    
                    if ( MEMBER_EXPR((  LHS(( c.type))  ))  ==  RHS(( CHANGE_LHS)) ) {
                        //unique_special_id << "L";
                         OPERATOR_LHS_ARG((current_line_names))  <<  OPERATOR_RHS_ARG_CANONICAL(("->")) ;
                         OPERATOR_LHS_ARG((current_line_types))  <<  OPERATOR_RHS_ARG_CANONICAL(("->")) ;
                         OPERATOR_LHS_ARG((current_line_values))  <<  OPERATOR_RHS_ARG_CANONICAL(("->")) ;
                         OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG((unique_special_id))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((c.name_of_var)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL(("=")) ;
                         OPERATOR_LHS_ARG((current_line_names))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((c.name_of_var))  )) ;
                    } else if ( MEMBER_EXPR((  LHS(( c.type))  ))  ==  RHS(( CHANGE_RHS)) ) {
                        //right hand side
                         OPERATOR_LHS_ARG((unique_special_id))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((c.value))  )) ;
                         OPERATOR_LHS_ARG((current_line_names))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((c.name_of_var))  )) ;
                    } else if ( MEMBER_EXPR((  LHS(( c.type))  ))  ==  RHS(( CHANGE_FUNCTIONCALL)) ) {
                         OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG(( OPERATOR_LHS_ARG((current_line_names))  << OPERATOR_RHS_ARG_CANONICAL(("(")) ))  <<  MEMBER_EXPR((  OPERATOR_LHS_ARG((c.name_of_var)) ))  ))  <<  OPERATOR_RHS_ARG_CANONICAL(("())")) ;
                    }
                     LHS(( tim))  =  MEMBER_EXPR(( c.time_of_change )) ;
                    
                }
                
                // << "_" << tim;
                 CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt_all)) ,  CALL_ARG((1)) ,  MEMBER_CALL((  ARG_UNKNOWN((unique_special_id.str().c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
                 CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt_all)) ,  CALL_ARG((2)) ,  MEMBER_CALL((  ARG_UNKNOWN((current_line_types.str().c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
                 CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt_all)) ,  CALL_ARG((3)) ,  MEMBER_CALL((  ARG_UNKNOWN((current_line_names.str().c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
                 CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt_all)) ,  CALL_ARG((4)) ,  MEMBER_CALL((  ARG_UNKNOWN((current_line_values.str().c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
                 CALLR(( sqlite3_bind_int( ARG_UNKNOWN((stmt_all)) ,  CALL_ARG((5)) ,  CALL_ARG((line_num)) ) )) ;
                 CALLR(( sqlite3_bind_int( ARG_UNKNOWN((stmt_all)) ,  CALL_ARG((6)) ,  CALL_ARG((tim)) ) )) ;
                 CALLR(( sqlite3_step( ARG_UNKNOWN((stmt_all)) ) )) ;
                
                 CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt_unique)) ,  CALL_ARG((1)) ,  MEMBER_CALL((  ARG_UNKNOWN((unique_special_id.str().c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
                 CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt_unique)) ,  CALL_ARG((2)) ,  MEMBER_CALL((  ARG_UNKNOWN((current_line_types.str().c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
                 CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt_unique)) ,  CALL_ARG((3)) ,  MEMBER_CALL((  ARG_UNKNOWN((current_line_names.str().c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
                 CALLR(( sqlite3_bind_text( ARG_UNKNOWN((stmt_unique)) ,  CALL_ARG((4)) ,  MEMBER_CALL((  ARG_UNKNOWN((current_line_values.str().c_str()))  )) ,  CALL_ARG((-1)) , SQLITE_TRANSIENT) )) ;
                 CALLR(( sqlite3_bind_int( ARG_UNKNOWN((stmt_unique)) ,  CALL_ARG((5)) ,  CALL_ARG((line_num)) ) )) ;
                 CALLR(( sqlite3_bind_int( ARG_UNKNOWN((stmt_unique)) ,  CALL_ARG((6)) ,  CALL_ARG((tim)) ) )) ;
                 CALLR(( sqlite3_step( ARG_UNKNOWN((stmt_unique)) ) )) ;
                
                
                 CALLR(( sqlite3_clear_bindings( ARG_UNKNOWN((stmt_all)) ) )) ;
                 CALLR(( sqlite3_reset( ARG_UNKNOWN((stmt_all)) ) )) ;
                 CALLR(( sqlite3_clear_bindings( ARG_UNKNOWN((stmt_unique)) ) )) ;
                 CALLR(( sqlite3_reset( ARG_UNKNOWN((stmt_unique)) ) )) ;
                //std::cout << "\n";
            }
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
     LHS(( oss))  << t;
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
     LHS(( oss))  << t;
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
