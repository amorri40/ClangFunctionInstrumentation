import sqlite3 as lite
import sys
import json
from datetime import datetime
from flask import Flask,render_template,url_for,redirect,request

def get_functions_for_file(fname, suffix, database_name='enigma_compiler.sqlite'):
    fname = fname.replace('/static/projects/CompilerSource/','').replace('./CompilerSource/','')
    fname = fname.replace('./','',1)
    try:
        print fname
        con = lite.connect(database_name)
        cur = con.cursor()
        cur.execute("SELECT * FROM sqlite_master WHERE type='table' AND name LIKE '%"+fname+"%"+suffix+"';")
        return cur.fetchall()
    except lite.Error, e:
        print "Error %s:" % e.args[0]
    finally:
        if con:
            con.close()

def connect_to_db(database_name='enigma_compiler.sqlite'):
    con = lite.connect(database_name)
    con.text_factory = lambda x: unicode(x, 'utf-8', 'ignore')
    return con.cursor()

def ms_to_string(miliseconds):
    hours, milliseconds = divmod(miliseconds, 3600000)
    minutes, milliseconds = divmod(miliseconds, 600000)
    seconds = (miliseconds) / 100000
    milliseconds = (miliseconds) / 1000000
    l = (miliseconds) / 1000000
    #s = "2013-%02i-%02i %02i:%02i:%02i" % (hours, minutes, seconds,(milliseconds),l)
    val = divmod(miliseconds,10000)
    val2 = divmod(val[1],1000)
    val3 = divmod(val2[1],100)
    val4 = divmod(val3[1],100)
    s = str(val[0])+"-"+str(val2[0])+"-"+str(val3[0])+" "+str(val3[0])+":"+str(val4[1])+":01"
    return s

def get_timeline_data(fname,table_suffix, database_name='enigma_compiler.sqlite'):
    all_events=[]
    print "fname:"+fname
    cpp_file_name = fname.split('.cpp',1)[0]

    all_function_in_this_file = get_functions_for_file(fname,'_executions_unique',database_name)
    cur = connect_to_db(database_name)
    last_time=0
    for funct in all_function_in_this_file:
        #print funct[1]
        cur.execute("SELECT * FROM \""+funct[1]+"\"")
        unique_executions = cur.fetchall()
        
        funcname = ""
        if len(funct[1].split('.cpp_',1)) > 1:
            funcname = funct[1].split('.cpp_',1)[1].replace('_executions_unique','')
            path_to_file = funct[1].split('.cpp_',1)[0]+'.cpp'
        elif len(funct[1].split('.c_',1)) > 1:
            funcname = funct[1].split('.c_',1)[1].replace('_executions_unique','')
            path_to_file = funct[1].split('.c_',1)[0]+'.c' #first remove function info
            #print 'split by fname:'+fname
            fname = fname.replace('./','/',1)
            path_to_file = fname + path_to_file.split(fname,1)[1] #remove the absolute path to the folder
        #print "funcname"+funcname
        #print "path_to_file:"+path_to_file
        execution_id = 0
        for execution in unique_executions:
            last_time = execution[2] # this is used for all_data to focus on the last date
            this_exec = {"id": funcname+str(execution_id), "funcname":funcname, "exid":execution_id, "title":funcname, "description":funct[1].replace(fname,''), "link":path_to_file, "startdate": ms_to_string(execution[1]), "enddate": ms_to_string(execution[2]), "modal_type":"full","importance": 50,"high_threshold":60,"date_display":"year","icon":"flag_yellow.png"}
            #print this_exec
            all_events.append(this_exec)
            execution_id+=1
    #now append screenshots
    cur.execute("SELECT * FROM \"alang_screenshots\"")
    all_screenshots = cur.fetchall()
    for screenshot in all_screenshots:
        #print screenshot
        screen_time = screenshot[0]
        screenshot_path = url_for('static', filename='projects'+screenshot[1])
        print screenshot_path
        this_screen = {"id": "sshot"+str(screen_time), "type_of_point" : "screenshot", "title":"screenshot", "description":"screenshot", "link":"#", "startdate": ms_to_string(screen_time), "enddate": ms_to_string(screen_time), "image" : screenshot_path, "modal_type":"full","importance": 50,"high_threshold":60,"date_display":"year"}
        all_events.append(this_screen)


    all_data = [{"id":"js_history", "title":"Execution data for "+fname, "description":"<p>All the execution data for the functions in this file is displayed in this timeline</p>","focus_date":ms_to_string(last_time),"initial_zoom":"50", "image_lane_height":50, "events":all_events}]
    return all_data

def get_trace_data(fname,table_suffix, ex_id, function_name, database_name='enigma_compiler.sqlite'):
    print "fname:"+fname+" exid:"+str(ex_id)
    #first get the names of the tables
    function_name_ex_all=get_functions_for_file(fname,function_name+'_executions_unique',database_name)[0][1]
    
    function_name_changes_unique=get_functions_for_file(fname,function_name+'_changes_unique',database_name)[0][1]
    con = None

    try:
        cur = connect_to_db(database_name)

        cur.execute("SELECT * FROM \""+function_name_ex_all+"\"")

        all_executions = cur.fetchall()

        if len(all_executions) < ex_id:
            return ["{trace: {exception_msg:'Invalid execution id'}}"]
        
        if table_suffix == "_unique":
            execution_json_string = '['+all_executions[ex_id][0]+'-1]'
        else:
            execution_json_string = all_executions[ex_id][0]
        print execution_json_string
        print "======================="

        execution = json.loads(execution_json_string)

        # Now get unique changes
        cur.execute("SELECT * FROM \""+function_name_changes_unique+"\"")
        
        changes_array = cur.fetchall()



        output_trace=[]

        for exe_change in execution:
            print exe_change
            if exe_change == -1: continue
            if table_suffix == "_unique":
                change_data_string = changes_array[exe_change-1][0]
            else:
                change_data_string = changes_array[exe_change[0]][0]
            change_data = json.loads(change_data_string,strict=False)
            print change_data
            line_num = change_data[0]
            line_start_col = change_data[1]
            line_end_col = change_data[2]
            val = change_data[3]
            #val_type = change_data[4]
            #print val_type
            output_trace.append({'exp_value':val, 'col_start' : line_start_col, 'col_end': line_end_col, 'func_name': fname, 'event': 'step_line', 'stack_to_render': [], 'stdout': '', 'heap': {}, 'line': line_num, 'column':(line_start_col-1), 'expr_width':(line_end_col-line_start_col)})             
        return output_trace, changes_array
    except lite.Error, e:
        
        print "Error %s:" % e.args[0]
        sys.exit(1)
        
    finally:
        
        if con:
            con.close()

#print get_functions_for_file('JDI','_executions_all')[0][1]

