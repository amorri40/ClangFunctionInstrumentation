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
//#define CALLR(arg) (arg) //call with return
#define CALLR(argument) inst_func_db.log_rhs(0,0,0,(argument))
#define MEMBER_CALL(arg) (arg)
#define MEMBER_EXPR(arg) (arg)
#define OPERATOR_LHS_ARG(line,beg,end,arg) (arg)
#define OPERATOR_RHS_ARG_CANONICAL(line,beg,end,arg) inst_func_db.log_rhs(line,beg,end,(arg))
#define OPERATOR_RHS_ARG_NOTCANONICAL(line,beg,end,arg) (arg)
#define CALL_ARG(argument) inst_func_db.log_call_arg(0,0,(argument))
//#define CALL_ARG(argument) inst_func_db.log_rhs(0,0,0,(argument))
#define RHS(line,beg,end,arg) inst_func_db.log_rhs(line,beg,end,(arg))
#define LHS(line,beg,end,arg) inst_func_db.log_rhs(line,beg,end,(arg))
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

    #define BUFFER_SIZE 256

    struct StaticFunctionData {
        std::string func_name, file_name;
        //function_line_data lines[];
        int start_of_function_line_number;
        std::vector<vector_of_change> all_function_executions;
        std::map<std::string, long long> map_of_sqlrows;
        
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
    
   /* namespace CHECK  // namespace to let "operator ==" not become global
    {
        typedef char no[7];
        typedef char print[7];
        template<typename T> no& operator == (const T&, const T&);
        template<typename T> print& operator << (std::ostream& o, const T&);
        
        template <typename T>
        struct opEqualExists // *(T*)(0) can be replaced by *new T[1] also
        {
            enum { value = (sizeof(*(T*)(0) == *(T*)(0)) != sizeof(no)) };
        };
        
        template <typename T>
        struct opPrintExists // *(T*)(0) can be replaced by *new T[1] also
        {
            enum { value = (sizeof(*(std::ostream&) == *(T*)(0)) != sizeof(print)) };
        };
    }*/
    
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

    struct InstrumentFunctionDB {
        double start_time, end_time, time_difference;
        long start_mem, end_mem, mem_difference;
        
        StaticFunctionData* ali_function_db;
        vector_of_change line_data;
        
        InstrumentFunctionDB(StaticFunctionData* the_db) : ali_function_db(the_db)  {
            start_time = clock();
            start_mem = report_memory();
            printf("\n >> Log Constructor: %s Mem:%ld bytes \n",the_db->func_name.c_str(),start_mem);
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
            
           // printf(" >> Log Destructor: Mem diff:%ld bytes Time:%f \n",mem_difference,time_difference);
        }

        
        /*void log_call(std::string var_name, int line_number) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            
            line_data[line_number].push_back((Change){CHANGE_FUNCTIONCALL,"",var_name,"",clock()});
            
            ali_clang_flush_db_on_each_change
        }
        
        void log_argument(std::string var_name, int line_number) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            
            line_data[line_number].push_back((Change){CHANGE_FUNCTIONCALL,"",var_name,"",clock()});
            
            ali_clang_flush_db_on_each_change
        }*/
        
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
         First primitive overloads
         */
        
        bool& log_rhs(int line_num, int start_loc, int end_loc, bool& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,"(bool)",line_num,start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        char& log_rhs(int line_num, int start_loc, int end_loc, char& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,"(byte)",line_num,start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        int& log_rhs(int line_num, int start_loc, int end_loc, int& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,"(int)",line_num,start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        unsigned int& log_rhs(int line_num, int start_loc, int end_loc, unsigned int& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,"(uint)",line_num,start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        long& log_rhs(int line_num, int start_loc, int end_loc, long& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,"(long)",line_num, start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        unsigned long& log_rhs(int line_num, int start_loc, int end_loc, unsigned long& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,"(ulong)",line_num, start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        double& log_rhs(int line_num, int start_loc, int end_loc, double& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,"(double)", line_num, start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        char*& log_rhs(int line_num, int start_loc, int end_loc, char*& val) {
            if (val == NULL) return val;
            line_data.push_back((Change){CHANGE_RHS,"(char*)",line_num, start_loc,end_loc,val,clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        /*
         constant versions
         */
        
        const char*& log_rhs(int line_num, int start_loc, int end_loc, const char*& val) {
            if (val == NULL) return val;
            line_data.push_back((Change){CHANGE_RHS,"(const char*)",line_num,start_loc,end_loc,val,clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        
        const bool& log_rhs(int line_num, int start_loc, int end_loc, const bool& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,"(const bool)",line_num,start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        const char& log_rhs(int line_num, int start_loc, int end_loc, const char& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,"(const char)", line_num, start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        
        /*
         
         Template's for other types
         
         */
        
        template <class T, class T2> std::string& log_rhs(int line_num, int start_loc, int end_loc, std::string& val) {
            std::ostringstream var_value; var_value << val;
            line_data.push_back((Change){CHANGE_RHS,typeid(val).name(),line_num,start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        template <class T, class T2> std::map<T,T2>& log_rhs(int line_num, int start_loc, int end_loc, std::map<T,T2>& val) {
            std::ostringstream var_value; var_value << " Map<" << typeid(T).name() << "," << typeid(T2).name() << "> (" << val.size() << ")";
            line_data.push_back((Change){CHANGE_RHS,typeid(val).name(),line_num,start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        /*
         This is the default case for unknown types
         */
        template <class T> T& log_rhs(int line_num, int start_loc, int end_loc, T& val) {
            std::ostringstream var_value; //var_value << " Ref(" << sizeof(T) <<"," << typeid(val).name() << ")";
            
            // if ( has_insertion_operator_impl::has_insertion_operator<T&>::debug__value);
            if (&val == NULL) return val;
            //if (sizeof(T) >= sizeof(short int)) {
            
            short buf[sizeof(T)];
            //short int buf;
            
            memcpy(&buf,&val,sizeof(T));
                var_value << buf ;
            //} else {
            //    var_value << " Ref(" << sizeof(T) <<"," << typeid(val).name() << ")";
            //}
            
            
            line_data.push_back((Change){CHANGE_RHS,typeid(val).name(),line_num,start_loc,end_loc,var_value.str(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        template <class T> const T& log_rhs(int line_num, int start_loc, int end_loc, const T& val) {
            std::ostringstream var_value; var_value << " ConstRef(" << sizeof(T) <<"," << typeid(val).name() << ")";
            line_data.push_back((Change){CHANGE_RHS,var_value.str(),line_num, start_loc,end_loc,typeid(val).name(),clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        template <class T> T*& log_rhs(int line_num, int start_loc, int end_loc, T*& val) {
            //pointer so just check if valid
            std::string var_value;
            if (val == NULL) var_value = "cNULL"; else var_value = "cVALID";
            line_data.push_back((Change){CHANGE_RHS,typeid(val).name(),line_num, start_loc,end_loc,var_value,clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        template <class T> const T*& log_rhs(int line_num, int start_loc, int end_loc, const T*& val) {
            //pointer so just check if valid
            std::string var_value;
            if (val == NULL) var_value = "cNULL"; else var_value = "cVALID";
            line_data.push_back((Change){CHANGE_RHS,typeid(val).name(),line_num, start_loc,end_loc,var_value,clock()});
            ali_clang_flush_db_on_each_change
            return val;
        }
        
        /*
         Older templates
         */
        
       /* template <class T> T& log_change_lhs(std::string var_name, int line_number, T& val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            /*std::ostringstream var_value;
            std::cout << CHECK::opEqualExists<T>::value <<std::endl;
            if (typeid(T) == typeid(int)) {
                var_value << val;
            } else {
                var_value << "unknown";
            }
            line_data[line_number].push_back((Change){CHANGE_LHS,typeid(val).name(),var_name,"var_value_lhs_unknown",clock()});
            
            ali_clang_flush_db_on_each_change
            
            return val;
        }*/
        
        
        
        /*template <class T> T* log_change_lhs(std::string var_name, int line_number, T* val) {
            line_number = line_number - ali_function_db->start_of_function_line_number ;
            std::ostringstream var_value;
            
            if (val == NULL) var_value << "cNULL"; else var_value << "cVALID";
            line_data[line_number].push_back((Change){CHANGE_LHS,typeid(val).name(),var_name,var_value.str(),clock()});
            
            ali_clang_flush_db_on_each_change
            
            return val;
        }*/
        
        /*
         old RHS overloads
         */
       /* template <class T> T& log_change_rhs(std::string var_name, int line_number, T& val) {
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
        struct is_pointer<T*> { static const bool value = true; };*/
        
        //const version
        /*template <class T> const T& log_change_rhs(std::string var_name, int line_number, const T& val) {
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
        */
        
        
    };

    
}

#endif
