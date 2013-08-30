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
#include <sys/time.h>

extern "C" {
    bool ALI_GLOBAL_DEBUG = true;
    sqlite3 *ali__log__db;
    int ALI_GLOBAL_MAX_EX = 1;
    int ALI_GLOBAL_MIN_EX = 1;
    int ALI_GLOBAL_MAX_CHANGES = 20; //per function changes (useful for big loops)
    int ALI_EXE_PER_FRAME = 50; //starting executions per sec
    int ALI_MAX_PER_FRAME = 50; //40 is good
    int WAIT_FOR_FRAMES = 0;
    int TOTAL_CHANGE_COUNT = 0;
    const char* database_name=0;
    bool ALI_STARTED_THREAD=false;
    
    void execute_query(const char* query) {
        std::cout << "Query:" << query;
        char * sErrMsg = 0;
        sqlite3_exec(ali__log__db, query, NULL, NULL, &sErrMsg);
    }
    
    namespace ali_clang_plugin_runtime {
    extern ali_clang_plugin_runtime::AllExecutionData alang_all_ex_data;
    }
    void alang_add_static_function_data(ali_clang_plugin_runtime::StaticFunctionData* sfd) {
        ali_clang_plugin_runtime::alang_all_ex_data.allfunctionex.push_back(sfd);
    }
    
    void alang_log_one_func_to_db(int change_limit) {
        ali_clang_plugin_runtime::alang_all_ex_data.write_database(change_limit);
    }
    
    int alang_start_clock = clock();
    extern void start_logging_thread();
    extern void getImage (bool isMainLoop);
    
    void alang_alter_logging() {
        if (!ALI_GLOBAL_DEBUG) return;
        if (!ALI_STARTED_THREAD) {start_logging_thread();ALI_STARTED_THREAD=true;}
        static int timeDemoFrames = 0;
        //static long long time_last_sec = clock();
        static struct timeval start, end;
        
        
        if (WAIT_FOR_FRAMES > 0) {WAIT_FOR_FRAMES--; gettimeofday(&start, NULL); return;}
        timeDemoFrames++;
        
        if (timeDemoFrames%1 == 0) { //every X frames
            long mtime, seconds, useconds;
            gettimeofday(&end, NULL);
            seconds  = end.tv_sec  - start.tv_sec;
            useconds = end.tv_usec - start.tv_usec;
            mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
            
            if (mtime > 40) {
                WAIT_FOR_FRAMES=1;
                //ALI_GLOBAL_MAX_EX--;
                if (ALI_MAX_PER_FRAME>1)
                ALI_MAX_PER_FRAME--;
            }
            else if (mtime < 33) {
                ALI_MAX_PER_FRAME++; //going fast so lets log more executions per frame
                //ALI_GLOBAL_MAX_CHANGES++; //going fast so lets increase number of changes logged
            } else if (mtime >= 34 && ALI_MAX_PER_FRAME>1) {
                ALI_MAX_PER_FRAME--;
                //ALI_GLOBAL_MAX_CHANGES--;
            }
            
            //ALI_GLOBAL_MAX_EX++;
            if (ALI_EXE_PER_FRAME > 0) {
                ALI_GLOBAL_MAX_EX++;
            }
            else //if (ALI_EXE_PER_FRAME <= 0)
            {ALI_GLOBAL_MAX_EX--;
                if (ALI_GLOBAL_MAX_EX <=0) ALI_GLOBAL_MAX_EX=1;
            }
            /*else  {
                ALI_EXE_PER_FRAME = ALI_MAX_PER_FRAME; //needs to be reset
            }*/
            
            if (timeDemoFrames%90 == 0) {
                //log info
                //std::cout << "exe_per_frame_leftover:" << ALI_EXE_PER_FRAME << " max_ex:" << ALI_GLOBAL_MAX_EX << "ALI_MAX_PER_FRAME:" << ALI_MAX_PER_FRAME << " Total_change:"<< TOTAL_CHANGE_COUNT << " time:" << mtime << "\n";
                getImage(true);
            }
            
            
            ALI_EXE_PER_FRAME = ALI_MAX_PER_FRAME;
            
            gettimeofday(&start, NULL);
            
            /*if (timeDemoFrames%130 == 0) { //only log every second
                //exections logged in this frame is neither all or none so lets log
                //alang_log_one_func_to_db(ALI_MAX_PER_FRAME);
                gettimeofday(&end, NULL);
                seconds  = end.tv_sec  - start.tv_sec;
                useconds = end.tv_usec - start.tv_usec;
                mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
                //std::cout << "It took:" << (mtime/33)<<"\n";
                //ALI_EXE_PER_FRAME =0;
                //ALI_GLOBAL_MAX_EX--;
                WAIT_FOR_FRAMES+=(mtime/33)+0.5;
            }*/
            if (TOTAL_CHANGE_COUNT > 100000)  {
                ALI_EXE_PER_FRAME =0;
                ALI_MAX_PER_FRAME=1;
                //sqlite3_close(ali__log__db);
                //sqlite3_open(database_name, &ali__log__db);
            }
            
        }
        //float	fps = timeDemoFrames * 1000.0f / ( clock() - alang_start_clock );
        //std::cout << fps;
        //ALI_GLOBAL_MAX_EX++;
    }
    
    struct alang_StaticFunctionData {
        int execution_number;
        const char* the_func_name;
        int the_line_number;
        const char* the_file_name;
    };
    
    extern const char* get_dbpath_path();
    
    
    void* new_CStaticFunctionData(const char* the_func_name, int the_line_number, const char* the_file_name) {
        std::cout << the_func_name;
         //WAIT_FOR_FRAMES=10;
        return new ali_clang_plugin_runtime::StaticFunctionData(the_func_name, the_line_number, the_file_name);
    }
    
    
    
    struct alang_StaticFunctionData* g_sfd;
    int g_current_change_number = 0;
    int g_prev_change_number = 0;
    using namespace ali_clang_plugin_runtime;
    
    void* alang_push_ex(void* sfd) {
        ALI_EXE_PER_FRAME--; //the main log of how many executions, don't remove
        return new InstrumentFunctionDB((StaticFunctionData*)sfd);
    }
    
#define alang_log_macro InstrumentFunctionDB* inst_func_db = (InstrumentFunctionDB*)inst;\
    if (inst_func_db->change_count > ALI_GLOBAL_MAX_CHANGES) return val;\
    inst_func_db->change_count++;\
    inst_func_db->log_builtin(line_num,start_loc,end_loc,val); \
    return val;
    
    long long alang_log_data(void* inst, int line_num, int start_loc, int end_loc, long long val) {
        alang_log_macro
    }
    
    char alang_log_char(void* inst, int line_num, int start_loc, int end_loc, char val) {
        alang_log_macro
    }
    long long alang_log_int(void* inst, int line_num, int start_loc, int end_loc, long long val) {alang_log_macro}
    bool alang_log_bool(void* inst, int line_num, int start_loc, int end_loc, bool val) {alang_log_macro}
    
    void alang_pop_ex(void* inst) {
        if (inst == NULL) return;
        InstrumentFunctionDB* inst_func_db = (InstrumentFunctionDB*)inst;
        delete inst_func_db;
        //g_current_change_number = g_prev_change_number;
    }

}

namespace ali_clang_plugin_runtime {
    
    std::ostream & operator<< ( std::ostream & os , const VAROBJECT & ty) {
        switch (ty.objectType) {
            case VAROBJECT::Int:
                os << ty.intValue;
                break;
            case VAROBJECT::String:
                os << ty.strValue;
                break;
            case VAROBJECT::Double:
                os << ty.dblValue;
                break;
            case VAROBJECT::Bool:
                os << ty.intValue;
                break;
            case VAROBJECT::CharT:
                os << ty.intValue;
                break;
        }
        return os;
    }
    
    void AllExecutionData::write_database(int exe_limit) {
        int current_logs=0;
        int number_logged = 0;
        int size = allfunctionex.size();
        while (current_logs < exe_limit && number_logged < size) {
            
            
            if (iiterator < size)
            {
                //current_logs += allfunctionex[iiterator]->all_function_executions.size();
                current_logs += allfunctionex[iiterator]->flush_to_db(exe_limit);
            }
            else {
                iiterator=0;
                continue;
            }
            number_logged++;
            iiterator++;
        }
        
    }
    
    
    bool ALI_GLOBAL_DEBUG = true;

    int ALI_GLOBAL_MAX_EX = 3;
    static bool created_database=false;

    void open_sqlite(std::string db_name) {
#ifdef IPHONE
        db_name = get_dbpath_path();
#endif
        database_name = db_name.c_str();
        /* Open database */
        int rc = sqlite3_open(db_name.c_str(), &ali__log__db);
        if( rc ){
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(ali__log__db));
            //exit(0);
        } else {
        created_database=true;
            create_table("alang_screenshots", "", " (Time INTEGER PRIMARY KEY, Path TEXT) ");
        }
    }
    
    void create_table(std::string table_name, std::string table_name_suffix, std::string schema) {
        
        std::ostringstream oss;
        oss << "CREATE TABLE IF NOT EXISTS \"" << table_name << table_name_suffix  << "\" "<< schema;
        char * sErrMsg = 0;
        sqlite3_exec(ali__log__db, oss.str().c_str(), NULL, NULL, &sErrMsg);
    }
    
    
    
    
    sqlite3_stmt * start_insert(std::string table_name, std::string table_name_suffix, std::string schema) {
        sqlite3_stmt * stmt;
        const char * tail = 0;
        
        std::ostringstream oss;
        oss << "INSERT INTO \"" << table_name << table_name_suffix  << "\" " << schema;
        sqlite3_prepare_v2(ali__log__db,  oss.str().c_str(), oss.str().length(), &stmt, &tail);
        return stmt;
    }
    
    int step_stmt(sqlite3_stmt * stmt) {
        int result = sqlite3_step(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
        return result;
    }
    
    int bind_change_sql(sqlite3_stmt * stmt, std::string unique_id, int col_start, int col_end, int line_num, int tim ) {
        sqlite3_bind_text(stmt, 1, unique_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, col_start);
        sqlite3_bind_int(stmt, 3, col_end);
        sqlite3_bind_int(stmt, 4, line_num);
        sqlite3_bind_int(stmt, 5, tim);
        
        return step_stmt(stmt);
    }
    
    int bind_execution_sql(sqlite3_stmt * stmt, std::string unique_id, int start_time, int end_time) {
        sqlite3_bind_text(stmt, 1, unique_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, start_time);
        sqlite3_bind_int(stmt, 3, end_time);
        return step_stmt(stmt);
    }

    void StaticFunctionData::create_tables() {
        if (created_tables) return;
        if (!created_database) open_sqlite("./enigma_compiler.sqlite");
        create_table(func_name, "_changes_unique", " (Special_id TEXT PRIMARY KEY, Col_Start INTEGER, Col_End INTEGER, Line_Number INTEGER, Time INTEGER) ");
        //create_table(func_name, "_changes_unique", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Line_Number INTEGER, Time INTEGER PRIMARY KEY)");
        create_table(func_name, "_executions_unique", " (Special_id TEXT PRIMARY KEY, Start_Time INTEGER, End_Time INTEGER) ");
        //create_table(func_name, "_executions_all", " (Special_id TEXT, Type_Of_Var TEXT, Name_Of_Var TEXT, Value_Of_Var TEXT, Start_Time INTEGER, End_Time INTEGER PRIMARY KEY)");
        created_tables=true;
        
        
        //WAIT_FOR_FRAMES=1;
    }
    
    bool created_segfault_handler=false;
    
    int StaticFunctionData::flush_to_db(int exe_limit) {
        
        if (all_function_executions.empty()) return 0;
        
        if (!created_tables) {create_tables();}//return 2;}
        
        
        stmt_unique = start_insert(func_name,"_changes_unique"," VALUES (@SP, @CS, @CE, @LN, @TI)");
        //stmt_ex_all = start_insert(func_name,"_executions_all"," VALUES (@SP, @TY, @NA, @VA, @LI, @TI)");
        stmt_ex_unique = start_insert(func_name,"_executions_unique"," VALUES (@SP, @ST, @ET)");
        char * sErrMsg = 0;
        sqlite3_exec(ali__log__db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
        
        this->execution_number++;
        //std::cout << "\n\n" << func_name << "\n";
        
        int executions_size=all_function_executions.size();
        int executions_logged=0;
        
        for (; executions_logged < executions_size; executions_logged++) {//for(std::vector<vector_of_change>::iterator execution = all_function_executions.begin(); execution != all_function_executions.end(); ++execution) {
            vector_of_change line_data = all_function_executions[executions_logged];
            if (line_data.empty()) continue;
            //vector_of_change line_data = *execution;
            
            std::ostringstream execution_id;
            
            unsigned long start_time=0,end_time=0;
            int size_of_changes = line_data.size(); //debug
            for (int no_of_change = 0; no_of_change< size_of_changes; no_of_change++) {//for(vector_of_change::iterator it2 = execution->begin(); it2 != execution->end(); ++it2) { //
                Change c = line_data[no_of_change];//*it2;
                //if (c== NULL) {std::cout << "\n===C is null==="<<"\n\n\n"; continue;}
                if (ali__log__db == NULL) { std::cout << "database is null"; /*all_function_executions.erase(execution);*/ return 0;}
                
                if (no_of_change == 0)
                    start_time=c.time_of_change;
                
                std::ostringstream unique_special_id;
                    unique_special_id << "[" << c.line_num << "," << c.start_loc << "," << c.end_loc << ",\"" << c.value << "\"]";//, \""<< c.value.objectType << "\"]";
                    end_time = c.time_of_change;
                long long insert_row_id=-1;
                
                if ( map_of_sqlrows.find(unique_special_id.str()) == map_of_sqlrows.end() ) {
                    // not found so add to db
                
                
                int change_result = bind_change_sql(stmt_unique,unique_special_id.str(), c.start_loc, c.end_loc, c.line_num, end_time);
                long long last_row=-1;
                if (change_result == SQLITE_DONE)
                last_row = sqlite3_last_insert_rowid(ali__log__db);
                insert_row_id = last_row;
                
                
                        if (last_row !=-1) {
                            map_of_sqlrows[unique_special_id.str()] = last_row; insert_row_id = last_row;}
                        else
                        {
                            std::ostringstream oss;
                            oss << "SELECT rowid FROM " << func_name << "_changes_unique WHERE Special_id = " << unique_special_id.str();
                            execute_query(oss.str().c_str());
                            map_of_sqlrows[unique_special_id.str()] = -1; insert_row_id = -1;
                        }
                    } else {
                        insert_row_id = map_of_sqlrows[unique_special_id.str()];
                    }
                execution_id << insert_row_id << ",";
                //delete c;
                
                
            }
            //execution->clear();
            
            //all_function_executions.erase
            //line_data.clear();
            //if (ali__log__db == NULL) {continue;}
                int unique_ex_result = bind_execution_sql(stmt_ex_unique,execution_id.str(), start_time, end_time);
                //unsigned long unique_that_was_just_inserted = sqlite3_last_insert_rowid(ali__log__db);
                //bind_change_sql(stmt_ex_all,execution_id.str(), "", "", "", start_time, end_time);
            //all_function_executions.erase(execution);
            //executions_logged++;
            //if (executions_logged > exe_limit) break;
            }
        
        sqlite3_exec(ali__log__db, "END TRANSACTION", NULL, NULL, &sErrMsg);
        sqlite3_finalize(stmt_unique);
        //sqlite3_finalize(stmt_ex_all);
        sqlite3_finalize(stmt_ex_unique);
        sqlite3_free(sErrMsg);
        all_function_executions.clear(); //now that they have been writtern destroy them
        return executions_logged;
    }
    

template <class T> std::string TToStr(T& t)
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

template <class T> std::string TToStr(T* t)
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

unsigned long report_memory(void) {
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
#include <unistd.h>

void segfault_handler(int sig) {
    void *array[10];
    size_t size;
    
    // get void*'s for all entries on the stack
    size = backtrace(array, 10);
    
    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
    void install_handlers() {
        signal(SIGSEGV, segfault_handler);
    }
}
