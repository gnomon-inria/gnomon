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

from gnomoncore import gnomonTime

def test_gnomonTime():
    '''Tests the gnomonTime type.
    '''
    t0 = gnomonTime(0)
    t1 = gnomonTime(1)
    t2 = gnomonTime(0, gnomonTime.ArbitraryTime)
    t3 = gnomonTime(0, gnomonTime.DateTime)


    assert t0.getMode() == gnomonTime.ArbitraryTime
    assert t0.getMode() == t2.getMode()
    assert t2.getMode() != t3.getMode()

    assert t0.getTimeStamp() == 0
    
    assert t0 < t1

#
# test_gnomonTime.py ends here.