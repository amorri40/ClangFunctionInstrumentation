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


/*
 defines that don't do anything yet
 */
#define LHS(line,beg,end,arg) (inst_func_db.log_change(line,beg,end,(arg)))
#define ARG_UNKNOWN(arg) (arg)
#define LOGPARAMETER(arg) (arg)
#define RHS(line,beg,end,arg) (inst_func_db.log_change(line,beg,end,(arg)))
#define CALL_ARG(arg) (arg)
#define OPERATOR_RHS_ARG_CANONICAL(line,beg,end,arg) (inst_func_db.log_change(line,beg,end,(arg)))
#define MEMBER_CALL(arg) (arg)
#define MEMBER_EXPR(arg) (arg)
#define OPERATOR_RHS_ARG_NOTCANONICAL(line,beg,end,arg) (inst_func_db.log_change(line,beg,end,(arg)))
#define OPERATOR_LHS_ARG(line,beg,end,arg) (arg)
#define CALL(arg) (arg)
#define CALLR(line,beg,end,arg) (arg)
//#define CALLR(line,beg,end,arg) (inst_func_db.log_change(line,beg,end,(arg)))

/*
 Main defines
 */
#define ali_clang_add_to_map(type,val) line_data.push_back((Change){CHANGE_RHS,type,line_num, start_loc,end_loc,val,clock()});
#define FLUSH_DB_FOR_EACH_CHANGE false 
#define ali_clang_flush_db_on_each_change {if (FLUSH_DB_FOR_EACH_CHANGE) {ali_function_db->all_function_executions.push_back(line_data); ali_function_db->flush_to_db(); line_data.clear();}}
//slower but effective for segfaults

namespace ali_clang_plugin_runtime {
    
    enum ChangeTypes {
        CHANGE_RHS, CHANGE_LHS, CHANGE_FUNCTIONCALL};
    
    struct Change {
        ChangeTypes type;
        std::string type_of_var;
        int line_num;
        int start_loc;
        int end_loc;
        std::string value;
        unsigned long time_of_change;
    };

    //typedef std::vector<std::vector<std::string> > vector_of_vector_of_string;
    //typedef std::map<int, std::vector<Change> > map_of_vector_of_change;
    typedef std::vector<Change> vector_of_change;

    extern sqlite3 *ali__log__db; //global statebase (sqlite)


    extern bool ALI_GLOBAL_DEBUG;

    unsigned long report_memory(void);
    void segfault_handler(int sig);
    void install_handlers();

    struct StaticFunctionData {
        std::string func_name, file_name;
        //function_line_data lines[];
        int start_of_function_line_number;
        std::vector<vector_of_change> all_function_executions;
        std::map<std::string, long long> map_of_sqlrows;
        
        StaticFunctionData(std::string the_func_name, int the_line_number, std::string the_file_name) : func_name(the_func_name), start_of_function_line_number(the_line_number), file_name(the_file_name) {
            //possibly take in number of lines
            //possibly create sqlite db
           // SEGFAULTHANDLE //temp for library
            create_tables();
        }
        
        void create_tables();
        
        void flush_to_db();
        
        ~StaticFunctionData() {
            //flush_to_db();
        }
        
    };
    
    namespace has_insertion_operator_impl {
        typedef char no;
        typedef char yes[2];
        
        struct any_t {
            template<typename T> any_t( T const& );
        };
        
        no operator<<( std::ostream const&, any_t const& );
        
        yes& test( std::ostream& );
        no test( no );
        
        template<typename T>
        struct has_insertion_operator {
            static std::ostream &s;
            static T const &t;
            static bool const debug__value = sizeof( test(s << t) ) == sizeof( yes );
        };
    }
    
    template <typename T>
    struct is_iterator {
        template <typename U>
        static char test(typename std::iterator_traits<U>::pointer* x);
        
        template <typename U>
        static long test(U*);
        
        static const bool value = sizeof(test<T>(nullptr)) == 1;
    };

    struct InstrumentFunctionDB {
        double start_time, end_time, time_difference;
        long start_mem, end_mem, mem_difference;
        
        StaticFunctionData* ali_function_db;
        vector_of_change line_data;
        
        InstrumentFunctionDB(StaticFunctionData* the_db) : ali_function_db(the_db)  {
            start_time = clock();
            start_mem = report_memory();
            //printf("\n >> Log Constructor: %s Mem:%ld bytes \n",the_db->func_name.c_str(),start_mem);
            //LOG_CONSTURCTOR
            //line_data.push_back((Change){CHANGE_RHS,"",ali_function_db->start_of_function_line_number,0,0,std::string("Function enter"),clock()});
            
        }
        ~InstrumentFunctionDB() {
            end_time = clock();
            end_mem = report_memory();
            //line_data.push_back((Change){CHANGE_RHS,"",ali_function_db->start_of_function_line_number,0,0,"Function exit",clock()});
            
            time_difference = (double)(end_time - start_time) / CLOCKS_PER_SEC; //move this to another thread later
            mem_difference = end_mem - start_mem;
            if (! line_data.empty()) {
                ali_function_db->all_function_executions.push_back(line_data);
                ali_function_db->flush_to_db();
            }
            
            
            
            //printf(" >> Log Destructor: %s Mem diff:%ld bytes Time:%f \n",ali_function_db->func_name.c_str(),mem_difference,time_difference);
        }
        
        /*
         constant overloads
         */
        
        const long double log_change(int line_num, int start_loc, int end_loc, const long double& val) { std::ostringstream value; value << val; ali_clang_add_to_map("long double",value.str())
            return val;
        }
        
        const long int log_change(int line_num, int start_loc, int end_loc, const long int& val) { std::ostringstream value; value << val; ali_clang_add_to_map("long int",value.str())
            return val;
        }
        
        const int log_change(int line_num, int start_loc, int end_loc, const int& val) { std::ostringstream value; value << val; ali_clang_add_to_map("int",value.str())
            return val;
        }
        
        const unsigned int log_change(int line_num, int start_loc, int end_loc, const unsigned int& val) { std::ostringstream value; value << val; ali_clang_add_to_map("unsigned int",value.str())
            return val;
        }
        
        const double log_change(int line_num, int start_loc, int end_loc, const double& val) { std::ostringstream value; value << val; ali_clang_add_to_map("double",value.str())
            return val;
        }
        
        const char log_change(int line_num, int start_loc, int end_loc, const char& val) { std::ostringstream value; value << val; ali_clang_add_to_map("char",value.str()) return val;
        }
        
        const float log_change(int line_num, int start_loc, int end_loc, const float& val) { std::ostringstream value; value << val; ali_clang_add_to_map("float",value.str()) return val;
        }
        
        const size_t log_change(int line_num, int start_loc, int end_loc, const size_t& val) { std::ostringstream value; value << val; ali_clang_add_to_map("size_t",value.str()) return val;
        }
        
        const std::string log_change(int line_num, int start_loc, int end_loc, const std::string& val) { ali_clang_add_to_map("string",val) return val;
        }
        
        const char* log_change(int line_num, int start_loc, int end_loc, const char* val) { ali_clang_add_to_map("char*",val) return val;
        }
        
        
        /*
         Non constant overloads
         */
        
        long double log_change(int line_num, int start_loc, int end_loc, long double& val) { std::ostringstream value; value << val; ali_clang_add_to_map("long double",value.str())
            return val;
        }
        
        long int log_change(int line_num, int start_loc, int end_loc, long int& val) { std::ostringstream value; value << val; ali_clang_add_to_map("long int",value.str())
            return val;
        }
        
        int log_change(int line_num, int start_loc, int end_loc, int& val) { std::ostringstream value; value << val; ali_clang_add_to_map("int",value.str())
            return val;
        }
        
        unsigned int log_change(int line_num, int start_loc, int end_loc, unsigned int& val) { std::ostringstream value; value << val; ali_clang_add_to_map("unsigned int",value.str())
            return val;
        }
        
        double log_change(int line_num, int start_loc, int end_loc, double& val) { std::ostringstream value; value << val; ali_clang_add_to_map("double",value.str())
            return val;
        }
        
        char log_change(int line_num, int start_loc, int end_loc, char& val) { std::ostringstream value; value << val; ali_clang_add_to_map("char",value.str()) return val;
        }
        
        float log_change(int line_num, int start_loc, int end_loc, float& val) { std::ostringstream value; value << val; ali_clang_add_to_map("float",value.str()) return val;
        }
        
        size_t log_change(int line_num, int start_loc, int end_loc, size_t& val) { std::ostringstream value; value << val; ali_clang_add_to_map("size_t",value.str()) return val;
        }
        
        std::string log_change(int line_num, int start_loc, int end_loc, std::string& val) { ali_clang_add_to_map("string",val) return val;
        }
        
        char* log_change(int line_num, int start_loc, int end_loc, char* val) { ali_clang_add_to_map("char*",val) return val;
        }
        
        /*
         Templates to catch the rest
         */
        
        template <typename T> const T log_change(int line_num, int start_loc, int end_loc, const T& val) {
            std::string value;
            if (is_iterator<T>::value) {
                std::ostringstream v;
                v<< "Iterator" << (*(int*)(&val));
                value = v.str();
            }
            else
            {
                value = "constT&";
                //std::cout << value;
            }
            
            ali_clang_add_to_map(typeid(T).name(),value)
            return val;//(T&)val;
        }
        
        
        
        template <typename T> T log_change(int line_num, int start_loc, int end_loc, T& val) {
            //std::cout << "T&";
            std::string value = "T&";
            ali_clang_add_to_map(typeid(T).name(),value)
            return val;
        }
        
        /*
         Pointer templates (just check if null or not for now
         */
        
        template <typename T> T* log_change(int line_num, int start_loc, int end_loc, T* val) {
            std::string value;
            if (val == NULL) value = "NULL"; else value = "VALID";
            ali_clang_add_to_map(typeid(T).name(),value)
            return val;
        }
        
        template <typename T> const T* log_change(int line_num, int start_loc, int end_loc, const T* val) {
            std::string value;
            if (val == NULL) value = "cNULL"; else value = "cVALID";
            ali_clang_add_to_map(typeid(T).name(),value)
            return val;
        }
        
        /*
         Special overloads
         */
        
        template <class T, class T2> std::map<T,T2> log_change(int line_num, int start_loc, int end_loc, std::map<T,T2> val) {
            std::ostringstream value; value << " Map<" << typeid(T).name() << "," << typeid(T2).name() << "> (" << val.size() << ")";
            ali_clang_add_to_map(typeid(T).name(),value.str())
            return val;
        }
        
        
    };

    
}

#endif
