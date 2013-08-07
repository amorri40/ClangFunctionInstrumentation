import sqlite3 as lite
import sys
import json

def get_functions_for_file(fname, suffix):
    fname = fname.replace('/static/projects/CompilerSource/','')
    try:
        con = lite.connect('enigma_compiler.sqlite')
        cur = con.cursor()
        cur.execute("SELECT * FROM sqlite_master WHERE type='table' AND name LIKE '"+fname+"%"+suffix+"';")
        return cur.fetchall()
    except lite.Error, e:
        
        print "Error %s:" % e.args[0]
        #sys.exit(1)
        
    finally:
        if con:
            con.close()

def get_trace_data(fname,table_suffix, ex_id):
    print fname
    #first get the names of the tables
    function_name_ex_all=get_functions_for_file(fname,'_executions'+table_suffix)[0][1]
    function_name_changes_unique=get_functions_for_file(fname,'_changes_unique')[0][1]
    con = None

    try:
        con = lite.connect('enigma_compiler.sqlite')
        
        cur = con.cursor()

        cur.execute("SELECT * FROM \""+function_name_ex_all+"\"")

        all_executions = cur.fetchall()

        if len(all_executions) < ex_id:
            return "{trace: {exception_msg:'Invalid execution id'}}"
        
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
            change_data = json.loads(change_data_string)
            print change_data
            line_num = change_data[0]
            line_start_col = change_data[1]
            line_end_col = change_data[2]
            val = change_data[3]
            val_type = change_data[4]
            print val_type
            output_trace.append({'func_name': fname, 'event': 'step_line', 'stack_to_render': [], 'stdout': '', 'heap': {1: ['Expression', 'happy()', None]}, 'line': line_num, 'column':(line_start_col-1), 'expr_width':(line_end_col-line_start_col), 'ordered_globals': [val], 'globals': {val:val_type}})             
        return output_trace
    except lite.Error, e:
        
        print "Error %s:" % e.args[0]
        sys.exit(1)
        
    finally:
        
        if con:
            con.close()

#print get_functions_for_file('JDI','_executions_all')[0][1]

