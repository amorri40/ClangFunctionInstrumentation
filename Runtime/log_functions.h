//
//  log_functions.h
//  TestAliClang
//
//  Created by Alasdair Morrison on 24/07/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//
#ifdef __cplusplus
#define _Bool bool
#define size_type __size_type
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

#define private public
//#define class struct

#define SSTR( x ) (dynamic_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str())

#define NO_INSTRUMENT false
#define SEGFAULTHANDLE ali_clang_plugin_runtime::install_handlers();

#define LHS(line,beg,end,arg) (stdlogger,inst_func_db.log_lvalue(line,beg,end,(arg)))//(stdlogger, inst_func_db.log_change(line,beg,end,(arg)))
#define ARG_UNKNOWN(line,beg,end,arg) (stdlogger, inst_func_db.log_change(line,beg,end,(arg)))
#define LOGPARAMETER(line,beg,end,arg) (stdlogger, inst_func_db.log_change(line,beg,end,(arg)))
#define RHS(line,beg,end,arg) (stdlogger, inst_func_db.log_change(line,beg,end,(arg)))
#define CALL_ARG(arg) (stdlogger,inst_func_db.log_change(0,0,0,(arg)))
#define CALL_LVALUE_ARG(line,beg,end,arg) (stdlogger,inst_func_db.log_lvalue(line,beg,end,(arg))) //(std::cout << __PRETTY_FUNCTION__ << "\n",arg)
#define OPERATOR_RHS_ARG_CANONICAL(line,beg,end,arg) (stdlogger, inst_func_db.log_change(line,beg,end,(arg)))
#define MEMBER_CALL(arg) (stdlogger,arg)
#define MEMBER_EXPR(arg) (stdlogger,arg)
#define OPERATOR_RHS_ARG_NOTCANONICAL(line,beg,end,arg) (stdlogger, inst_func_db.log_change(line,beg,end,(arg)))
#define OPERATOR_LHS_ARG(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg)))
#define CALL(arg) (stdlogger,(arg),0)
//#define CALLR(line,beg,end,arg) (arg)
#define CALLR(line,beg,end,arg) (inst_func_db.log_change(line,beg,end,(arg)))
#define BOOLEXP(line,beg,end,arg) (stdlogger,inst_func_db.log_boolexp(line,beg,end,(arg)))

#define RVALUE_PAREN(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg))) //(stdlogger,inst_func_db.log_boolexp(line,beg,end,(arg)))
#define LVALUE_PAREN(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg)))

#define NoOp(line,beg,end,arg) arg //(stdlogger,arg)
#define IntegralCast(line,beg,end,arg) arg
#define IntegralToBoolean(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg)))
#define LValueToRValue(line,beg,end,arg) arg //(std::cout << __PRETTY_FUNCTION__ << "\n",stdlogger,arg, inst_func_db.log_change(line,beg,end,(arg)))
#define PointerToBoolean(line,beg,end,arg) (stdlogger,arg)
#define FloatingCast(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg)))
#define IntegralToFloating(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg)))
#define DerivedToBase(line,beg,end,arg) (stdlogger,arg)
#define ConstructorConversion(line,beg,end,arg) (stdlogger,arg)
#define ArrayToPointerDecay(line,beg,end,arg) (stdlogger,arg)
#define NullToPointer(line,beg,end,arg) NULL
#define UserDefinedConversion(line,beg,end,arg) (stdlogger,arg)
#define FloatingToIntegral(line,beg,end,arg) (arg)//((int)(inst_func_db.log_change(line,beg,end,(arg))))

#define ExprWithCleanupsCall(line,beg,end,arg) arg
#define ExprWithCleanups(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg))) //(stdlogger,arg)
#define ConditionalOperator(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg))) //(stdlogger,arg)
#define CXXConstructExpr(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg))) //(stdlogger,arg)
#define ArraySubscriptExpr(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg))) //(stdlogger,arg)
#define CXXFunctionalCastExpr(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg)))  //(stdlogger,arg)
#define CXXBindTemporaryExpr(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg)))  //(stdlogger,arg)
#define MaterializeTemporaryExpr(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg))) //(stdlogger,arg)
#define CStyleCastExpr(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg))) //(stdlogger,arg)
#define CXXThisExpr(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg))) //(stdlogger,arg)
#define RETURN_VAL(line,beg,end,arg) arg //(stdlogger,arg)

#define Atomic(line,beg,end,arg) (stdlogger,inst_func_db.log_builtin(line,beg,end,(arg)))
#define Fundamental(line,beg,end,arg) (stdlogger,inst_func_db.log_builtin(line,beg,end,(arg)))
#define EnumLog(line,beg,end,arg) (stdlogger,inst_func_db.log_builtin(line,beg,end,(arg)))
#define IntegralOrEnumType(line,beg,end,arg) (stdlogger,inst_func_db.log_builtin(line,beg,end,(arg)))
#define StringType(line,beg,end,arg) (stdlogger,inst_func_db.log_builtin(line,beg,end,(arg)))
#define ClassWithOperator(line,beg,end,arg) (stdlogger,inst_func_db.log_builtin(line,beg,end,(arg)))

#define TemporaryObjectExpr(line,beg,end,arg) (stdlogger,inst_func_db.log_change(line,beg,end,(arg)))

#define stdlogger 0
//#define stdlogger (std::cout << __FILE__ << ":" << __LINE__ << ":" << __PRETTY_FUNCTION__)

#define STRING_STREAM( data )                                                  \
((ostringstream&)( *( auto_ptr<ostringstream>(new ostringstream()) ) << data)).str()

/*
 Main defines
 */
#define ali_clang_add_to_map(type,val) {line_data.push_back((ali_clang_plugin_runtime::Change){ali_clang_plugin_runtime::CHANGE_RHS,type,line_num, start_loc,end_loc,(val),clock()}); ali_clang_flush_db_on_each_change}
#define FLUSH_DB_FOR_EACH_CHANGE false
#define ali_clang_flush_db_on_each_change {if (FLUSH_DB_FOR_EACH_CHANGE) {ali_function_db->all_function_executions.push_back(line_data); ali_function_db->flush_to_db(); line_data.clear();}}
//slower but effective for segfaults

//#include "auto_generate.h"

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
    extern int ALI_GLOBAL_MAX_EX;

    unsigned long report_memory(void);
    void segfault_handler(int sig);
    void install_handlers();
    
   

    struct StaticFunctionData {
        std::string func_name, file_name;
        int execution_number;
        //function_line_data lines[];
        int start_of_function_line_number;
        std::vector<vector_of_change> all_function_executions;
        std::map<std::string, long long> map_of_sqlrows;
        bool created_database;
        
        StaticFunctionData(std::string the_func_name, int the_line_number, std::string the_file_name) : func_name(the_func_name), start_of_function_line_number(the_line_number), file_name(the_file_name) {
            //possibly take in number of lines
            //possibly create sqlite db
            //SEGFAULTHANDLE //temp for library
            execution_number=0;
            created_database=false;
            //created_segfault_handler=false;
        }
        
        void create_tables();
        
        void flush_to_db();
        
        ~StaticFunctionData() {
            std::cout << "deallocating staticfunction data and writing to db";
            flush_to_db();
        }
        
    };
    
    struct AllExecutionData {
        std::vector<StaticFunctionData*> allfunctionex;
        
        void write_database() {
        //loop through and call flush_to_db
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
            static bool const debug__ali_clang_value = sizeof( test(s << t) ) == sizeof( yes );
        };
    }
    
    /*template <typename T>
    struct is_iterator {
        template <typename U>
        static char test(typename std::iterator_traits<U>::pointer* x);
        
        template <typename U>
        static long test(U*);
        
        static const bool ali_clang_value = sizeof(test<T>(nullptr)) == 1;
    };*/
    
    struct cout_redirect {
        cout_redirect( std::streambuf * new_buffer )
        : old( std::cout.rdbuf( new_buffer ) )
        { }
        
        ~cout_redirect( ) {
            std::cout.rdbuf( old );
        }
        
    private:
        std::streambuf * old;
    };
    
    

    struct InstrumentFunctionDB {
        double start_time, end_time, time_difference;
        long start_mem, end_mem, mem_difference;
        int change_count = 0;
        
        StaticFunctionData* ali_function_db;
        vector_of_change line_data;
        
        InstrumentFunctionDB(StaticFunctionData* the_db) : ali_function_db(the_db)  {
            //std::cout << the_db->func_name << "constructor/n";
            /*start_time = clock();
            start_mem = report_memory();*/
            //printf("\n >> Log Constructor: %s Mem:%ld bytes \n",the_db->func_name.c_str(),start_mem);
            //LOG_CONSTURCTOR
            //line_data.push_back((Change){CHANGE_RHS,"",ali_function_db->start_of_function_line_number,0,0,std::string("Function enter"),clock()});
            line_data = vector_of_change();
            
        }
        ~InstrumentFunctionDB() {
            stdlogger;
            /*end_time = clock();
            end_mem = report_memory();
            //line_data.push_back((Change){CHANGE_RHS,"",ali_function_db->start_of_function_line_number,0,0,"Function exit",clock()});
            
            time_difference = (double)(end_time - start_time) / CLOCKS_PER_SEC; //move this to another thread later
            mem_difference = end_mem - start_mem;*/
            
            if (! line_data.empty()) {
                ali_function_db->all_function_executions.push_back(line_data);
                
                //ali_function_db->flush_to_db();
            }
            
            
            
            //printf(" >> Log Destructor: %s Mem diff:%ld bytes Time:%f \n",ali_function_db->func_name.c_str(),mem_difference,time_difference);
        }
        
        
        void log_custom(int line_num, int start_loc, int end_loc, std::string ty,  std::string val) { ali_clang_add_to_map(ty,val)
            ;
        }
        /*
         constant overloads
         */
        
        const long double log_change(int line_num, int start_loc, int end_loc, const long double&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("long double",ali_clang_value.str())
            return val;
        }
        
        const long int log_change(int line_num, int start_loc, int end_loc, const long int&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("long int",ali_clang_value.str())
            return val;
        }
        
        const int log_change(int line_num, int start_loc, int end_loc, const int&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("int",ali_clang_value.str())
            return val;
        }
        
        const unsigned int log_change(int line_num, int start_loc, int end_loc, const unsigned int&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("unsigned int",ali_clang_value.str())
            return val;
        }
        
        const double log_change(int line_num, int start_loc, int end_loc, const double&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("double",ali_clang_value.str())
            return val;
        }
        
        const char log_change(int line_num, int start_loc, int end_loc, const char&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("char",ali_clang_value.str()) return val;
        }
        
        const float log_change(int line_num, int start_loc, int end_loc, const float&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("float",ali_clang_value.str()) return val;
        }
        
        const size_t log_change(int line_num, int start_loc, int end_loc, const size_t&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("size_t",ali_clang_value.str()) return val;
        }
        
        const std::string log_change(int line_num, int start_loc, int end_loc, const std::string&& val) { stdlogger;ali_clang_add_to_map("string",val) return val;
        }
        
        const char* log_change(int line_num, int start_loc, int end_loc, const char* val) { stdlogger; ali_clang_add_to_map("const char*",val) return val;
        }
        
        
        /*
         Non constant overloads
         */
        
        long double log_change(int line_num, int start_loc, int end_loc, long double&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("long double",ali_clang_value.str())
            return val;
        }
        
        long int log_change(int line_num, int start_loc, int end_loc, long int&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("long int",ali_clang_value.str())
            return val;
        }
        
        int log_change(int line_num, int start_loc, int end_loc, int&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("int",ali_clang_value.str())
            return val;
        }
        
        unsigned int log_change(int line_num, int start_loc, int end_loc, unsigned int&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("unsigned int",ali_clang_value.str())
            return val;
        }
        
        double log_change(int line_num, int start_loc, int end_loc, double&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("double",ali_clang_value.str())
            return val;
        }
        
        char log_change(int line_num, int start_loc, int end_loc, char&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("char",ali_clang_value.str()) return val;
        }
        
        float log_change(int line_num, int start_loc, int end_loc, float&& val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("float",ali_clang_value.str()) return val;
        }
        
        /*size_t log_change(int line_num, int start_loc, int end_loc, size_t val) { std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("size_t",ali_clang_value.str()) return val;
        }*/
        
        bool log_change(int line_num, int start_loc, int end_loc, bool&& val) { stdlogger; std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("bool",ali_clang_value.str()) return val;
        }
        
        /*std::string log_change(int line_num, int start_loc, int end_loc, std::string val) { ali_clang_add_to_map("string",val) return val;
        }*/
        
        char* log_change(int line_num, int start_loc, int end_loc, char* val) { stdlogger;
            if (val == NULL) return NULL;
            if (&val == NULL) return NULL;
            if (val == 0) return val;
            if(*val == '\0') return val;
            ali_clang_add_to_map("char*",val)
            
            return val;
        }
        
        template <class T>
        std::basic_ios<T>& log_change(int line_num, int start_loc, int end_loc, std::basic_ios<T>& val) { ali_clang_add_to_map("std::ios","iostream") return val;
        }
        
        template <class T>
        const std::basic_ios<T>& log_change(int line_num, int start_loc, int end_loc, const std::basic_ios<T>& val) { ali_clang_add_to_map("std::ios","iostream") return val;
        }
        
        template <class T>
        std::basic_ostream<T>& log_change(int line_num, int start_loc, int end_loc, std::basic_ostream<T>& val) { ali_clang_add_to_map("std::ostream","ostream") return val;
        }
        
        template <class T>
        const std::basic_ostream<T>& log_change(int line_num, int start_loc, int end_loc, const std::basic_ostream<T>& val) { ali_clang_add_to_map("std::ios","iostream") return val;
        }
        
        template <class T>
        std::basic_istream<T>& log_change(int line_num, int start_loc, int end_loc, std::basic_istream<T>& val) { ali_clang_add_to_map("std::ostream","ostream") return val;
        }
        
        template <class T>
        const std::basic_istream<T>& log_change(int line_num, int start_loc, int end_loc, const std::basic_istream<T>& val) { ali_clang_add_to_map("std::ios","iostream") return val;
        }
        
        template <class T>
        std::basic_ofstream<T>& log_change(int line_num, int start_loc, int end_loc, std::basic_ofstream<T>& val) { ali_clang_add_to_map("std::ios","iostream") return val;
        }
        
        template <class T>
        const std::basic_ofstream<T>& log_change(int line_num, int start_loc, int end_loc, const std::basic_ofstream<T>& val) { ali_clang_add_to_map("std::ios","iostream") return val;
        }
        
        
        /*template <class T>
        T log_builtin(int line_num, int start_loc, int end_loc, T val) {
            std::ostringstream v;
            v << val;
            ali_clang_add_to_map(typeid(T).name(),v.str()) return val;
        }*/
        
        template <class T>
        const T& log_builtin(int line_num, int start_loc, int end_loc, const T& val) {
            std::ostringstream v;
            v << val;
            ali_clang_add_to_map(typeid(T).name(),v.str()) return val;
        }
        
        template <class T>
        T& log_builtin(int line_num, int start_loc, int end_loc, T& val) {
            std::ostringstream v;
            v << val;
            ali_clang_add_to_map(typeid(T).name(),v.str()) return val;
        }
        
        /*
         Templates to catch the rest
         */
        
        template <typename T> const T& log_change(int line_num, int start_loc, int end_loc, const T& val) {
            stdlogger;
            std::string ali_clang_value;
            /*if (is_iterator<T>::ali_clang_value) {
                std::ostringstream v;
                v<< "Iterator" << (*(int*)(&val));
                ali_clang_value = v.str();
            }
            else*/
            {
                ali_clang_value = "constT&";
                //std::cout << ali_clang_value;
            }
            
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value)
            return val;//(T&)val;
        }
        
        template <typename T> T& log_change(int line_num, int start_loc, int end_loc, T& val) {
            stdlogger;
            std::string ali_clang_value = "T&";
            if (sizeof(T) < 20) {
                short buf[sizeof(T)];
                memcpy(&buf,&val,sizeof(T)); //dest, source, size
                std::ostringstream v;
                v << buf;
                ali_clang_value = v.str();
            }
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value)
            
            return val;
        }
        
        
        template <typename T> T& log_change(int line_num, int start_loc, int end_loc, T&& val) {
            stdlogger;
            std::string ali_clang_value = "T&&";
            if (sizeof(T) < 20) {
                short buf[sizeof(T)];
                memcpy(&buf,&val,sizeof(T)); //dest, source, size
            std::ostringstream v;
                v << buf;
                ali_clang_value = v.str();
            }
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value)
            
            return val;
        }
        
        /*
         Pointer templates (just check if null or not for now
         */
        
        template <typename T> T* log_change(int line_num, int start_loc, int end_loc, T* val) {
            stdlogger;
            std::string ali_clang_value;
            if (val == NULL) ali_clang_value = "NULL"; else ali_clang_value = "VALID";
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value)
            return val;
        }
        
        template <typename T> const T* log_change(int line_num, int start_loc, int end_loc, const T* val) {
            stdlogger;
            std::string ali_clang_value;
            if (val == NULL) ali_clang_value = "cNULL"; else ali_clang_value = "cVALID";
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value)
            return val;
        }
        
        /*
         Special overloads
         */
        
        template <class T, class T2> std::map<T,T2> log_change(int line_num, int start_loc, int end_loc, std::map<T,T2> val) {
            stdlogger;
            std::ostringstream ali_clang_value; ali_clang_value << " Map<" << typeid(T).name() << "," << typeid(T2).name() << "> (" << val.size() << ")";
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value.str())
            return val;
        }
        template <class T, class T2> std::iterator<T,T2> log_change(int line_num, int start_loc, int end_loc, std::iterator<T,T2> val) {
            stdlogger;
            std::ostringstream ali_clang_value; ali_clang_value << " Iterator<" << typeid(T).name() << "," << typeid(T2).name() << "> (" << val << ")";
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value.str())
            return val;
        }
        
        
        template <class T> std::vector<T> log_change(int line_num, int start_loc, int end_loc, std::vector<T> val) {
            stdlogger;
            std::ostringstream ali_clang_value; ali_clang_value << " Vector<" << typeid(T).name() << "> (" << val.size() << ")";
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value.str())
            return val;
        }
        
        /*
         LValue
         */
        template <typename T> T& log_lvalue(int line_num, int start_loc, int end_loc, T& val) {
            stdlogger;
            std::string ali_clang_value = "LValueT&";
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value)
            //std::cout << "end of T&";
            return val;
        }
        
        template <typename T> T& log_lvalue(int line_num, int start_loc, int end_loc, T&& val) {
            stdlogger;
            std::string ali_clang_value = "LValueT&&";
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value)
            //std::cout << "end of T&";
            return val;
        }
        
        /*
         Boolean expressions
         */
        bool log_boolexp(int line_num, int start_loc, int end_loc, bool val) { stdlogger; std::ostringstream ali_clang_value; ali_clang_value << val; ali_clang_add_to_map("bool",ali_clang_value.str()) return val;
        }
        
        template <typename T> T log_uvalue(int line_num, int start_loc, int end_loc, T& val) {
            stdlogger;
            std::string ali_clang_value = "UValueT&";
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value)
            //std::cout << "end of T&";
            return val;
        }
        
        template <typename T> T log_uvalue(int line_num, int start_loc, int end_loc, T&& val) {
            stdlogger;
            std::string ali_clang_value = "UValueT&&";
            ali_clang_add_to_map(typeid(T).name(),ali_clang_value)
            //std::cout << "end of T&";
            return val;
        }
        
        
    };

    
}

#endif
#else
// we are in C-only code
#include "C_only_log_functions.h"
#endif //end if c++
