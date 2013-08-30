#!/bin/python

from flask import Flask,render_template,url_for,redirect,request
import subprocess,os,datetime, shutil, aes, json, ali_vis
from StringIO import StringIO
app = Flask(__name__)
app.debug = True
password = "userpassword"

@app.route('/')
def printindex():
  return printfiles('.')

def getJSON(fname,data_type):
    print "getJSON database_name:"+request.args.get('dbname', '')
    if request.args.get('dbname', '') != '':
      database_name = str(request.args.get('dbname', ''))
    else:
      database_name =  'alang_ios.sqlite'
    print "set database_name to:"+database_name
    if request.args.get('unique', '') == 'true':
        table_suffix="_unique"
    else:
      table_suffix="_all"
    if data_type == 'trace':
        ex_id = int(request.args.get('exid', ''))
        functionname = request.args.get('funcname', '')
        output_trace,changes_array = ali_vis.get_trace_data(fname,table_suffix,ex_id,functionname,database_name)
        ret = dict(code="", trace=output_trace, changes=changes_array)
    else:
        ret = ali_vis.get_timeline_data(fname,table_suffix,database_name)
    return json.dumps(ret, indent=None)

@app.route('/files/<filename>',methods=['GET', 'POST'])
def printfiles(filename):
    print "database_name:"+str(request.args.get('dbname', ''))
    if not os.path.dirname(__file__) == '':
      os.chdir(os.path.dirname(__file__))
    returnstring=""
    fname=request.args.get('fname', '').replace('..','').replace('////','//')
    if (request.args.get('getJSON', '') == 'true') :
      return getJSON(fname,'trace')
    elif (request.args.get('getTimeline', '') == 'true') :
      return getJSON(fname,'timeline')
    if (request.args.get('save', '') == 'true') :
       
       passcode=request.form['pass']
       encrypted=request.form['code']
       blocksize = 256
       
       if (aes.decrypt( passcode, password, blocksize )) != "secret": return "Failed"
       #decrypt the code now that we know its the valid password
       decrypted = aes.decrypt( encrypted, password, blocksize )
       
       now = datetime.datetime.now()
       print 'save fname:'+fname
       #first backup the original file
       backupname= '../../frontend/projects/backup/'+now.strftime("%Y-%m-%d")+'_'+str(now.hour)+'/'+fname
       if not os.path.exists(os.path.dirname(backupname)): os.makedirs(os.path.dirname(backupname))
       shutil.copyfile('../../frontend/projects/'+fname, backupname)
       
       f = open('../../frontend/projects/'+fname, 'w')
       f.write(decrypted)
       f.close()
       return "saved at: "+str(datetime.datetime.now())
    if fname == '':
     fname='.'
    if (not os.path.isdir('../../frontend/projects/'+fname)):
      extension = os.path.splitext(fname)[1]
      with open('../../frontend/projects/'+fname, 'r') as content_file:
         content = content_file.read()
      return render_template('visualize.html',extension=extension,content=content,fname=fname)
      #return render_template('syntaxEditor.html',extension=extension,content=content,fname=fname) #'<html><body><textarea>'+ content.replace('<','&lt;').replace('>','&gt;')+'</textarea></body></html>'
    for dirname, dirnames, filenames in os.walk('../../frontend/projects/'+fname):
     for subdirname in dirnames:
        returnstring+="<br><a href='/files/fname?fname="+ os.path.join(fname, subdirname)+"'>"+subdirname+"</a>"
     for filename in filenames:
        extension = os.path.splitext(filename)[1]
        if extension == '.c' or extension == '.cpp':
          returnstring+="<li><a href='/files/fname?fname="+ os.path.join(fname, filename)+"'>"+filename+"</a></li>"

     return render_template('visualize.html',extension='folder',content='folder',fname=fname,contents=returnstring,db_name=request.args.get('dbname', ''))#return '<html><body>'+returnstring+'</body></html>'

@app.route("/upload", methods=['POST'])
def upload():
            print "upload called"
            f = request.files['file']
            print 'in upload handler'
            print f
            # Note I've monkeypatched werkzeug.datastructures.FileStorage 
            # so it wont squash exceptions
            f.close()
            #f.stream.close()
            return 'ok'

@app.route("/test/")
def test_server():
    client = app.test_client()
    resp = client.post(
        '/upload',
        data = {
            'file': (StringIO('my file contents'), 'hello world.txt'),
        }
    )
    return "done"

if __name__ == '__main__':  
     app.run(host='0.0.0.0')
