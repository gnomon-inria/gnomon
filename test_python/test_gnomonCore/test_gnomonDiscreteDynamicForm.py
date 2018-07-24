# Version: $Id$
#
#

# Commentary:
#
#

# Change Log:
#
#

# Code:

import sys
from PyQt5.QtCore import QSettings

settings = QSettings(QSettings.IniFormat,QSettings.UserScope,"inria","dtk-script")
settings.beginGroup("modules");
paths = settings.value("path")
settings.endGroup()

for path in paths.split(":"):
    sys.path.append(path)
    
from gnomoncore import gnomonDiscreteDynamicForm


def test_gnomonDiscreteDynamicForm():
    '''Tests the gnomonDiscreteDynamicForm class.
    '''

    dyf = gnomonDiscreteDynamicForm()

#
# test_gnomonDiscreteDynamicForm.py ends here.