//
//  C_only_log_functions.h
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 12/08/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#ifndef ClangInstrumentationPlugin_C_only_log_functions_h
#define ClangInstrumentationPlugin_C_only_log_functions_h

#include <stdbool.h>

int alang_log_data(int line_num, int start_loc, int end_loc, int val);


struct alang_StaticFunctionData {
    int execution_number;
    const char* the_func_name;
    int the_line_number;
    const char* the_file_name;
};

void alang_push_ex(struct alang_StaticFunctionData* sfd);
void alang_pop_ex(struct alang_StaticFunctionData* sfd);

struct InstrumentFunctionDB {
    double start_time, end_time, time_difference;
    long start_mem, end_mem, mem_difference;
    
    struct alang_StaticFunctionData* ali_function_db;
    //vector_of_change line_data;
    
    /*InstrumentFunctionDB( StaticFunctionData* the_db)  {
        ali_function_db = (the_db);
    }
    ~InstrumentFunctionDB() {
        /*if (! line_data.empty()) {
            ali_function_db->all_function_executions.push_back(line_data);
            ali_function_db->flush_to_db();
        }
    }*/
}; //needs to know when return was called

struct StaticFunctionData* create_StaticFunctionData(const char* the_func_name, int the_line_number, const char* the_file_name);

extern bool ALI_GLOBAL_DEBUG;
extern int ALI_GLOBAL_MAX_EX;

#define false 0

#define NO_INSTRUMENT false
#define SEGFAULTHANDLE ali_clang_plugin_runtime::install_handlers();

#define LHS(line,beg,end,arg) (arg)
#define ARG_UNKNOWN(line,beg,end,arg) (arg)
#define LOGPARAMETER(line,beg,end,arg) (arg)
#define RHS(line,beg,end,arg) (arg)
#define CALL_ARG(arg) (arg)
#define CALL_LVALUE_ARG(line,beg,end,arg) (arg)
#define OPERATOR_RHS_ARG_CANONICAL(line,beg,end,arg) (arg)
#define MEMBER_CALL(arg) (arg)
#define MEMBER_EXPR(arg) (arg)
#define OPERATOR_RHS_ARG_NOTCANONICAL(line,beg,end,arg) (arg)
#define OPERATOR_LHS_ARG(line,beg,end,arg) (arg)
#define CALL(arg) (stdlogger,(arg),0)
//#define CALLR(line,beg,end,arg) (arg)
#define CALLR(line,beg,end,arg) (arg)
#define BOOLEXP(line,beg,end,arg) (arg)

#define RVALUE_PAREN(line,beg,end,arg) (arg)
#define LVALUE_PAREN(line,beg,end,arg) (arg)

#define NoOp(line,beg,end,arg) arg
#define IntegralCast(line,beg,end,arg) arg
#define IntegralToBoolean(line,beg,end,arg) (arg)
#define LValueToRValue(line,beg,end,arg) arg
#define PointerToBoolean(line,beg,end,arg) (arg)
#define FloatingCast(line,beg,end,arg) (arg)
#define IntegralToFloating(line,beg,end,arg) (arg)
#define DerivedToBase(line,beg,end,arg) (arg)
#define ConstructorConversion(line,beg,end,arg) (arg)
#define ArrayToPointerDecay(line,beg,end,arg) (arg)
#define NullToPointer(line,beg,end,arg) NULL
#define UserDefinedConversion(line,beg,end,arg) (arg)
#define FloatingToIntegral(line,beg,end,arg) (arg)

#define ExprWithCleanupsCall(line,beg,end,arg) arg
#define ExprWithCleanups(line,beg,end,arg) (arg)
#define ConditionalOperator(line,beg,end,arg) (arg)
#define CXXConstructExpr(line,beg,end,arg) (arg)
#define ArraySubscriptExpr(line,beg,end,arg) (arg)
#define CXXFunctionalCastExpr(line,beg,end,arg) (arg)
#define CXXBindTemporaryExpr(line,beg,end,arg) (arg)
#define MaterializeTemporaryExpr(line,beg,end,arg) (arg)
#define CStyleCastExpr(line,beg,end,arg) (arg)
#define CXXThisExpr(line,beg,end,arg) (arg)
#define RETURN_VAL(line,beg,end,arg) arg

#define Atomic(line,beg,end,arg) (arg)
#define Fundamental(line,beg,end,arg) (arg)
#define EnumLog(line,beg,end,arg) (arg)
#define IntegralOrEnumType(line,beg,end,arg) (alang_log_data(line,beg,end,(arg)))
#define StringType(line,beg,end,arg) (arg)
#define ClassWithOperator(line,beg,end,arg) (arg)

#define TemporaryObjectExpr(line,beg,end,arg) (arg)

#define stdlogger 0

#endif
