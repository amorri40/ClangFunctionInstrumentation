//
//  log_functions.h
//  TestAliClang
//
//  Created by Alasdair Morrison on 24/07/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//
#define _Bool bool
#include <vector>
#include <map>

#ifndef TestAliClang_log_functions_h
#define TestAliClang_log_functions_h
#import <iostream>
#import <time.h>
#include <sstream>
#include "sqlite3.h"
#import <mach/mach.h>
#include <execinfo.h>
#include <signal.h>

#define SSTR( x ) (dynamic_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str())

#define NO_INSTRUMENT false

namespace ali_clang_plugin_runtime {

    struct Change {
        bool lhs; //left or right (right is new value, left is previous)
        std::string type_of_var;
        std::string name_of_var;
        std::string value;
        unsigned long time_of_change;
        
    };

    //typedef std::vector<std::vector<std::string> > vector_of_vector_of_string;
    typedef std::map<int, std::vector<Change> > map_of_vector_of_change;

    extern sqlite3 *ali__log__db; //global statebase (sqlite)


    extern bool ALI_GLOBAL_DEBUG;

    unsigned long report_memory(void);
    //template <class T> std::string TToStr(T& t);
    //template <class T> std::string TToStr(T* t);

    #define BUFFER_SIZE 256

    struct StaticFunctionData {
        std::string func_name, file_name;
        //function_line_data lines[];
        int start_of_function_line_number;
        std::vector<map_of_vector_of_change> all_function_executions;
        
        StaticFunctionData(std::string the_func_name, int the_line_number, std::string the_file_name) : func_name(the_func_name), start_of_function_line_number(the_line_number), file_name(the_file_name) {
            //possibly take in number of lines
            //possibly create sqlite db
        }
        
        void create_tables() {
            char * sErrMsg = 0;
            std::ostringstream oss;
            oss << "CREATE TABLE IF NOT EXISTS \"" << func_name  << "_unique\" (Special_id TEXT PRIMARY KEY, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER) ";
            sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
            oss.str(""); //clear string stream
            oss << "CREATE TABLE IF NOT EXISTS \"" << func_name  << "_all\" (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)";
            sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
            
        }
        
        void flush_to_db() {
            if (all_function_executions.empty()) return;
            /* Open database */
            int rc = sqlite3_open("enigma_compiler.sqlite", &ali__log__db);
            if( rc ){
                fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ali__log__db));
                exit(0);
            }
            char * sErrMsg = 0;
            sqlite3_stmt * stmt_all, *stmt_unique;
            const char * tail = 0;
            const char * tail_unique = 0;
            char sSQL_all [BUFFER_SIZE] = "\0";
            char sSQL_unique [BUFFER_SIZE] = "\0";
            std::ostringstream oss;
            create_tables();
            
            oss << "INSERT INTO " << func_name << "_all VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
            sprintf(sSQL_all, oss.str().c_str());
            sqlite3_prepare_v2(ali__log__db,  sSQL_all, BUFFER_SIZE, &stmt_all, &tail);
            oss.str("");
            oss << "INSERT INTO " << func_name << "_unique VALUES (@SP, @TY, @NA, @VA, @LI, @TI)";
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
                    //std::cout << "  " << line_num << ": ";
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
                        current_line_names << c.name_of_var;
                        if (c.lhs) {
                            //unique_special_id << "L";
                            current_line_names << "->";
                            current_line_types << "->";
                            current_line_values << "->";
                            unique_special_id << c.name_of_var << "=";
                        } else {
                        //right hand side
                            unique_special_id << c.value;
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
        
        ~StaticFunctionData() {
            flush_to_db();
        }
        
        
        
    };




    template <class T>
    struct LineData {
        int line_number;
        std::string type_name, var_name;
        T value;
        bool lhs;
        
        LineData(int num, std::string type_str, std::string var_str, T val, bool leftHandSide) : line_number(num), type_name(type_str), var_name(var_str), value(val), lhs(leftHandSide)  {}
    };

    struct InstrumentFunctionDB {
        double start_time, end_time, time_difference;
        long start_mem, end_mem, mem_difference;
        //const char* function_name;
        //int start_of_function_line_number;
        StaticFunctionData* ali_function_db;
        map_of_vector_of_change line_data;
        //memory, cpu time, battery etc
        
        InstrumentFunctionDB(StaticFunctionData* the_db) : ali_function_db(the_db)  {
            //line_data = std::vector<std::vector<std::string> >(1, std::vector<std::string>(1));
            start_time = clock();
            start_mem = report_memory();
            //printf("\n >> Log Constructor: %s Mem:%ld bytes \n",the_function_name,start_mem);
        }
        ~InstrumentFunctionDB() {
            end_time = clock();
            end_mem = report_memory();
            
            time_difference = (double)(end_time - start_time) / CLOCKS_PER_SEC; //move this to another thread later
            mem_difference = end_mem - start_mem;
            if (! line_data.empty()) {
                ali_function_db->all_function_executions.push_back(line_data);
                ali_function_db->flush_to_db();
            }
            
            printf(" >> Log Destructor: Mem diff:%ld bytes Time:%f \n",mem_difference,time_difference);
        }
        
        template <class T> T& log_change_lhs(std::string var_name, int line_number, T& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            var_value << val;
            line_data[line_number].push_back((Change){true,typeid(val).name(),var_name,var_value.str(),clock()});
            return val;
        }
        
        /*
         RHS overloads
         */
        template <class T> T& log_change_rhs(std::string var_name, int line_number, T& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            var_value << val;
            line_data[line_number].push_back((Change){false,typeid(val).name(),var_name,var_value.str(),clock()});
            return val;
        }
        
        template<typename T>
        struct is_pointer { static const bool value = false; };
        
        template<typename T>
        struct is_pointer<T*> { static const bool value = true; };
        
        //const version
        template <class T> const T& log_change_rhs(std::string var_name, int line_number, const T& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            if (is_pointer<T>::value) {
                if (val == NULL) var_value << "cNULL"; else var_value << "cVALID";
            } else {
            var_value << val << "_const";
            }
            line_data[line_number].push_back((Change){false,typeid(val).name(),var_name,var_value.str(),clock()});
            return val;
        }
        
        
        //pointer versions
        template <class T> T*& log_change_rhs(std::string var_name, int line_number, T*& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::string strvalue; if (val == NULL) strvalue = "NULL"; else strvalue = "VALID";
            line_data[line_number].push_back((Change){false,typeid(val).name(),var_name,strvalue,clock()});
            return val;
        }
        
        //const pointer versions
        template <class T> const T*& log_change_rhs(std::string var_name, int line_number, const T*& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::string strvalue; if (val == NULL) strvalue = "NULL"; else strvalue = "cVALID";
            line_data[line_number].push_back((Change){false,typeid(val).name(),var_name,strvalue,clock()});
            
            return val;
        }
        
        char* log_change_rhs(std::string var_name, int line_number, char* val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            
            line_data[line_number].push_back((Change){false,typeid(val).name(),var_name,val,clock()});
            return val;
        }
        
        const char* log_change_rhs(std::string var_name, int line_number, const char* val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            
            line_data[line_number].push_back((Change){false,typeid(val).name(),var_name,val,clock()});
            return val;
        }
        
        
        
        /*int& log_change_rhs(std::string var_name, int line_number, int& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            var_value << val;
            line_data[line_number].push_back((Change){false,typeid(val).name(),var_name,var_value.str(),clock()});
            return val;
        }
        
        int log_change_rhs(std::string var_name, int line_number, const int& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            var_value << val;
            line_data[line_number].push_back((Change){false,typeid(val).name(),var_name,var_value.str(),clock()});
            return val;
        }*/
        
        
        
    };

    void segfault_handler(int sig);
}

#endif
