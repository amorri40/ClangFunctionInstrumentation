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
#define SEGFAULTHANDLE ali_clang_plugin_runtime::install_handlers();
#define CALL(arg) (arg)
#define CALLR(arg) (arg) //call with return
#define MEMBER_CALL(arg) (arg)
#define MEMBER_EXPR(arg) (arg)
#define OPERATOR_LHS_ARG(arg) (arg)
#define OPERATOR_RHS_ARG_CANONICAL(arg) inst_func_db.log_rhs(0,0,(arg))
#define OPERATOR_RHS_ARG_NOTCANONICAL(arg) (arg)
#define CALL_ARG(argument) inst_func_db.log_call_arg(0,0,(argument))
#define RHS(arg) inst_func_db.log_rhs(0,0,(arg))
#define LHS(arg) inst_func_db.log_rhs(0,0,(arg))
//#define LOGRETURN(arg) (arg)
#define LOGPARAMETER(arg) inst_func_db.log_parameter(0,0,(arg))
#define ARG_UNKNOWN(arg) (arg)  

#define FLUSH_DB_FOR_EACH_CHANGE true 
#define ali_clang_flush_db_on_each_change if (FLUSH_DB_FOR_EACH_CHANGE) ali_function_db->flush_to_db();
//slower but effective for segfaults

namespace ali_clang_plugin_runtime {
    
    enum ChangeTypes {
        CHANGE_RHS, CHANGE_LHS, CHANGE_FUNCTIONCALL};
    
    struct Change {
        ChangeTypes type;
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
    void segfault_handler(int sig);
    void install_handlers();

    #define BUFFER_SIZE 256

    struct StaticFunctionData {
        std::string func_name, file_name;
        //function_line_data lines[];
        int start_of_function_line_number;
        std::vector<map_of_vector_of_change> all_function_executions;
        
        StaticFunctionData(std::string the_func_name, int the_line_number, std::string the_file_name) : func_name(the_func_name), start_of_function_line_number(the_line_number), file_name(the_file_name) {
            //possibly take in number of lines
            //possibly create sqlite db
            SEGFAULTHANDLE //temp for library
        }
        
        void create_tables();
        
        void flush_to_db();
        
        ~StaticFunctionData() {
            flush_to_db();
        }
        
    };
    
  /*  namespace CHECK  // namespace to let "operator ==" not become global
    {
        typedef char no[7];
        template<typename T> no& operator == (const T&, const T&);
        
        template <typename T>
        struct opEqualExists // *(T*)(0) can be replaced by *new T[1] also
        {
            enum { value = (sizeof(*(T*)(0) == *(T*)(0)) != sizeof(no)) };
        };
    }*/

    struct InstrumentFunctionDB {
        double start_time, end_time, time_difference;
        long start_mem, end_mem, mem_difference;
        
        StaticFunctionData* ali_function_db;
        map_of_vector_of_change line_data;
        
        InstrumentFunctionDB(StaticFunctionData* the_db) : ali_function_db(the_db)  {
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

        
        void log_call(std::string var_name, int line_number) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            
            line_data[line_number].push_back((Change){CHANGE_FUNCTIONCALL,"",var_name,"",clock()});
            
            ali_clang_flush_db_on_each_change
        }
        
        void log_argument(std::string var_name, int line_number) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            
            line_data[line_number].push_back((Change){CHANGE_FUNCTIONCALL,"",var_name,"",clock()});
            
            ali_clang_flush_db_on_each_change
        }
        
        /*template <class T> void log_lhs(int start_loc, int end_loc, T& param) {
            
        }*/
        
        template <class T> void log_parameter(int start_loc, int end_loc, T& param) {
            
        }
        
        template <class T> T& log_call_arg(int start_loc, int end_loc, T& param) {
            return param;
        }
        
        template <class T> const T& log_call_arg(int start_loc, int end_loc, const T& param) {
            return param;
        }
        
        template <class T> T*& log_call_arg(int start_loc, int end_loc, T*& param) {
            return param;
        }
        
        template <class T> const T*& log_call_arg(int start_loc, int end_loc, const T*& param) {
            return param;
        }
        
        template <class T> T** log_call_arg(int start_loc, int end_loc, T** param) {
            return param;
        }

        char*& log_call_arg(int start_loc, int end_loc, char *& param) {
            return param;
        }
        void* log_call_arg(int start_loc, int end_loc, void * param) {
            return param;
        }
        
        /*
         These templates log changes on each line
         */
        
        template <class T> T& log_rhs(int start_loc, int end_loc, T& param) {
            return param;
        }
        
        template <class T> const T& log_rhs(int start_loc, int end_loc, const T& param) {
            return param;
        }
        
        template <class T> T*& log_rhs(int start_loc, int end_loc, T*& param) {
            return param;
        }
        
        template <class T> const T*& log_rhs(int start_loc, int end_loc, const T*& param) {
            return param;
        }
        
        /*
         Older templates
         */
        
        template <class T> T& log_change_lhs(std::string var_name, int line_number, T& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            /*std::ostringstream var_value;
            std::cout << CHECK::opEqualExists<T>::value <<std::endl;
            if (typeid(T) == typeid(int)) {
                var_value << val;
            } else {
                var_value << "unknown";
            }*/
            line_data[line_number].push_back((Change){CHANGE_LHS,typeid(val).name(),var_name,"var_value_lhs_unknown",clock()});
            
            ali_clang_flush_db_on_each_change
            
            return val;
        }
        
        
        
        template <class T> T* log_change_lhs(std::string var_name, int line_number, T* val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            
            if (val == NULL) var_value << "cNULL"; else var_value << "cVALID";
            line_data[line_number].push_back((Change){CHANGE_LHS,typeid(val).name(),var_name,var_value.str(),clock()});
            
            ali_clang_flush_db_on_each_change
            
            return val;
        }
        
        /*
         RHS overloads
         */
        template <class T> T& log_change_rhs(std::string var_name, int line_number, T& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            var_value << val;
            line_data[line_number].push_back((Change){CHANGE_RHS,typeid(val).name(),var_name,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
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
            line_data[line_number].push_back((Change){CHANGE_RHS,typeid(val).name(),var_name,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        
        
        
        //pointer versions
        template <class T> T*& log_change_rhs(std::string var_name, int line_number, T*& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::string strvalue; if (val == NULL) strvalue = "NULL"; else strvalue = "VALID";
            line_data[line_number].push_back((Change){CHANGE_RHS,typeid(val).name(),var_name,strvalue,clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        //const pointer versions
        template <class T> const T*& log_change_rhs(std::string var_name, int line_number, const T*& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::string strvalue; if (val == NULL) strvalue = "NULL"; else strvalue = "cVALID";
            line_data[line_number].push_back((Change){CHANGE_RHS,typeid(val).name(),var_name,strvalue,clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        char* log_change_rhs(std::string var_name, int line_number, char* val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            
            line_data[line_number].push_back((Change){CHANGE_RHS,typeid(val).name(),var_name,val,clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        const char* log_change_rhs(std::string var_name, int line_number, const char* val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            
            line_data[line_number].push_back((Change){CHANGE_RHS,typeid(val).name(),var_name,val,clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        
        
        int& log_change_rhs(std::string var_name, int line_number, int& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            var_value << val;
            line_data[line_number].push_back((Change){CHANGE_RHS,typeid(val).name(),var_name,var_value.str(),clock()});
            return val;
        }
        
        int log_change_rhs(std::string var_name, int line_number, const int& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            var_value << val;
            line_data[line_number].push_back((Change){CHANGE_RHS,typeid(val).name(),var_name,var_value.str(),clock()});
            return val;
        }
        
        
        
    };

    
}

#endif
