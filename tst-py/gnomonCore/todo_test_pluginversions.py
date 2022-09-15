import unittest

import gnomon.core
from gnomon.utils.gnomonPlugin import corePlugin
from gnomon.core import gnomonAbstractDataDriver

class dummyPlugin(gnomonAbstractDataDriver):
    def __init__(self):
        super().__init__()
        self.myname = "dummy"

    def name(self):
        return self.myname

    def insert(self, doc):
        return ""

    def delete_one(self, key):
        return False

    def protect(self, key):
        return False

    def find(self, query):
        return [""]

version_exact = "0.71.0"

split = version_exact.split('.')
split[1] = str(int(split[1]) + 1)
version_future = '.'.join(split)
split[1] = str(int(split[1]) - 2)
version_old_bad = '.'.join(split)
split[1] = str(int(split[1]) + 1)
split[2] = str(int(split[2]) - 1)
version_old_good = '.'.join(split)


@corePlugin(version="0.1.0", coreversion=version_old_good, base_class=gnomonAbstractDataDriver)
class dummyOldPlugin(dummyPlugin):
    def __init__(self):
        super().__init__()
        self.doc = "I am a good old plugin"
        self.myname = "ooooold"


@corePlugin(version="0.1.0", coreversion=version_old_bad, base_class=gnomonAbstractDataDriver)
class dummyOldBadPlugin(dummyPlugin):
    def __init__(self):
        super().__init__()
        self.doc = "I am a bad old plugin"
        self.myname = "ooooold"


@corePlugin(version="0.1.0", coreversion=version_exact, base_class=gnomonAbstractDataDriver)
class dummyExactPlugin(dummyPlugin):
    def __init__(self):
        super().__init__()
        self.doc = "I am an good exact plugin"
        self.myname = "exact"


@corePlugin(version="0.1.0", coreversion=version_future, base_class=gnomonAbstractDataDriver)
class dummyFuturePlugin(dummyPlugin):
    def __init__(self):
        super().__init__()
        self.doc = "I am a bad future plugin"
        self.myname = "future"


class TestPluginsVerrsion(unittest.TestCase):
    '''
    Tests the plugins version check.
    '''

    def test_good_old_plugin(self):
        plugin = gnomon.core.dataDriver_pluginFactory().create("dummyOldPlugin")
        assert plugin

    def test_bad_old_plugin(self):
        plugin = gnomon.core.dataDriver_pluginFactory().create("dummyOldBadPlugin")
        assert not plugin

    def test_good_exact_plugin(self):
        plugin = gnomon.core.dataDriver_pluginFactory().create("dummyExactPlugin")
        assert plugin

    def test_bad_future_plugin(self):
        plugin = gnomon.core.dataDriver_pluginFactory().create("dummyFuturePlugin")
        assert not plugin

