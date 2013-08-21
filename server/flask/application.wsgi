import os, sys, logging
logging.basicConfig(stream=sys.stderr)

PROJECT_DIR = '/ClangFunctionInstrumentation/server/flask/'

sys.path.append(PROJECT_DIR)

from runserver import app as application