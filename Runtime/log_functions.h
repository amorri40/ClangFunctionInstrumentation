//
//  log_functions.h
//  TestAliClang
//
//  Created by Alasdair Morrison on 24/07/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//
#define _Bool bool
#include <vector>
#ifndef TestAliClang_log_functions_h
#define TestAliClang_log_functions_h
#import <iostream>
#import <time.h>
#include <sstream>


#import <mach/mach.h>

#define NO_INSTRUMENT false

typedef std::vector<std::vector<std::string>> vector_of_vector_of_string;
// ...

extern bool ALI_GLOBAL_DEBUG;

unsigned long report_memory(void);
template <class T> std::string TToStr(const T& t);


struct StaticFunctionData {
    std::string func_name;
    //function_line_data lines[];
    int start_of_function_line_number;
    std::vector<vector_of_vector_of_string> all_function_executions;
    
    StaticFunctionData(std::string the_func_name, int the_line_number) : func_name(the_func_name), start_of_function_line_number(the_line_number) {
        //possibly take in number of lines
        //possibly create sqlite db
    }
    
    ~StaticFunctionData() {
        
        std::cout << "\nFunction: " << func_name << " \n";
        
        for(std::vector<vector_of_vector_of_string>::size_type execution = 0; execution != all_function_executions.size(); execution++) {
            vector_of_vector_of_string line_data = all_function_executions[execution];
            
            for(vector_of_vector_of_string::size_type i = 0; i != line_data.size(); i++) {
                if (line_data[i].empty()) continue;
                std::cout << "  " << i << ": ";
                for(std::vector<std::string>::reverse_iterator it2 = line_data[i].rbegin(); it2 != line_data[i].rend(); ++it2) {
                    std::cout << *it2;
                }
                std::cout << "\n";
            }
        }
        
        
    }
    
    void write_to_db() {}
    
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
    std::vector<std::vector<std::string> > line_data;
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
        }
        
        //printf(" >> Log Destructor: Mem diff:%ld bytes Time:%f \n",mem_difference,time_difference);
    }
    
    template <class T> T log_change_lhs(std::string var_name, int line_number, T val) {
        
        line_number = line_number - ali_function_db->start_of_function_line_number ;
        
        std::ostringstream oss;
        oss << " LHS(" << (typeid(val).name()) << " " << var_name << " = " << val << ") ";
        
        if (line_data.size() <= line_number) {
            line_data.resize(line_number+1);
        }
        line_data[line_number].push_back(oss.str());
        return val;
    }
    template <class T> T& log_change_rhs(std::string var_name, int line_number, T& val) {
        line_number = line_number - ali_function_db->start_of_function_line_number ;
        
        std::ostringstream oss;
        oss << " RHS(" << (typeid(val).name()) << " " << var_name << " = " << val << ") ";
        
        if (line_data.size() <= line_number) {
            line_data.resize(line_number+1);
        }
        line_data[line_number].push_back(oss.str());
        
        return val;
    }
    
    //const version
    template <class T> const T& log_change_rhs(std::string var_name, int line_number, const T& val) {
        line_number = line_number - ali_function_db->start_of_function_line_number ;
        //std::cout << "set to const value" << " in ("<< __PRETTY_FUNCTION__ << ") ";// << (line_number-start_of_function_line_number) << " " << var_name << " changed in " << function_name << " value:" << val << "";
        std::ostringstream oss;
        oss << " RHS(" << (typeid(val).name()) << " " << var_name << " = " << val << ") ";
        
        if (line_data.size() <= line_number) {
            line_data.resize(line_number+1);
        }
        line_data[line_number].push_back(oss.str());
        
        return val;
    }
    
    
    //pointer versions
    template <class T> T*& log_change_rhs(std::string var_name, int line_number, T*& val) {
        line_number = line_number - ali_function_db->start_of_function_line_number ;
        
        std::ostringstream oss;
        oss << " RHS(" << (typeid(val).name()) << " " << var_name << " = " << val << ") ";
        
        if (line_data.size() <= line_number) {
            line_data.resize(line_number+1);
        }
        line_data[line_number].push_back(oss.str());
        
        return val;
        
    }
    
    //const pointer versions
    template <class T> const T* log_change_rhs(std::string var_name, int line_number, const T* val) {
        line_number = line_number - ali_function_db->start_of_function_line_number ;
        
        std::ostringstream oss;
        oss << " RHS(" << (typeid(val).name()) << " " << var_name << " = " << val << ") ";
        
        if (line_data.size() <= line_number) {
            line_data.resize(line_number+1);
        }
        line_data[line_number].push_back(oss.str());
        
        return val;
    }
    
    /*template <class T> const T* log_change_end(std::string var_name, int line_number, const T val) {
     std::cout << (line_number-start_of_function_line_number) << " " << var_name << " changed in " << function_name << " value:" << val << "";
     
     return val;
     }*/
    
    
    /*const int& log_change_start(std::string var_name, int line_number, const int& val) { return val; }
     
     const int& log_change_end(std::string var_name, int line_number, const int& val) {
     std::cout << "log_change_const_int_end";
     return val;
     }
     
     int& log_change_start(std::string var_name, int line_number, int& val) { return val; }
     
     int& log_change_end(std::string var_name, int line_number, int& val) {
     std::cout << "log_change_int_end";
     return val;
     }*/
    
    //const double& log_change_start(std::string var_name, int line_number, const double& val) { return val; }
    
    /*const double& log_change_end(std::string var_name, int line_number, const double& val) {
     std::cout << "log_change_end_const_double";
     return val;
     }*/
    
    
    int& log_change_rhs(std::string var_name, int line_number, int& val) {
        line_number = line_number - ali_function_db->start_of_function_line_number ;
        
        std::ostringstream oss;
        oss << " RHS(" << (typeid(val).name()) << " " << var_name << " = " << val << ") ";
        
        if (line_data.size() <= line_number) {
            line_data.resize(line_number+1);
        }
        line_data[line_number].push_back(oss.str());
        
        return val;
    }
    
    int log_change_rhs(std::string var_name, int line_number, const int& val) {
        line_number = line_number - ali_function_db->start_of_function_line_number ;
        std::ostringstream oss;
        oss << " RHS(" << (typeid(val).name()) << " " << var_name << " = " << val << ") ";
        
        if (line_data.size() <= line_number) {
            line_data.resize(line_number+1);
        }
        line_data[line_number].push_back(oss.str());
        
        return val;
    }
    
    /*
     double& log_change_start(std::string var_name, int line_number, double& val) { return val; }
     
     double& log_change_end(std::string var_name, int line_number, double& val) {
     std::cout << "log_change_end_double";
     return val;
     }
     
     int& log_change_start(std::string var_name, int line_number, int& val) { return val; }
     
     int& log_change_end(std::string var_name, int line_number, int& val) {
     std::cout << "log_change_end_double";
     return val;
     }
     
     char& log_change_end(std::string var_name, int line_number, char& val) {
     std::cout << "log_change_end_double";
     return val;
     }
     
     unsigned long& log_change_end(std::string var_name, int line_number, unsigned long& val) {
     std::cout << "log_change_end_double";
     return val;
     }
     
     unsigned int& log_change_end(std::string var_name, int line_number, unsigned int& val) {
     std::cout << "log_change_end_double";
     return val;
     }
     
     unsigned char& log_change_end(std::string var_name, int line_number, unsigned char& val) {
     std::cout << "log_change_end_double";
     return val;
     }
     
     const unsigned char& log_change_end(std::string var_name, int line_number, const unsigned char& val) {
     std::cout << "log_change_end_double";
     return val;
     }
     
     const unsigned long& log_change_end(std::string var_name, int line_number, const unsigned long& val) {
     std::cout << "log_change_end_double";
     return val;
     }
     */
    
    /*int log_change_end(std::string var_name, int line_number,  const int& val) {
     std::cout << "log_change_end_double";
     return val;
     }*/
    
};



#endif
