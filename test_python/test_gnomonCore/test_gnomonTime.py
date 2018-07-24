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
    
from gnomoncore import gnomonTime

class TestGnomonTime:
    '''
    Tests the gnomonTime class.
    '''

    def setUp(self):    
        self.t0 = gnomonTime(0)
        self.t1 = gnomonTime(1)
        self.t2 = gnomonTime(0, gnomonTime.ArbitraryTime)
        self.t3 = gnomonTime(0, gnomonTime.DateTime)

    def tearDown(self):
        pass

    def test_gnomonTime_mode(self):
        assert self.t0.getMode() == gnomonTime.ArbitraryTime
        assert self.t0.getMode() == self.t2.getMode()
        assert self.t2.getMode() != self.t3.getMode()

    def test_gnomonTime_timestamp(self):
        assert self.t0.getTimeStamp() == 0
        assert self.t0.getTimeStamp() < self.t1.getTimeStamp()

#
# test_gnomonTime.py ends here.