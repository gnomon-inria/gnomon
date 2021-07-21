import json
import os
import unittest
from gnomon_utils import gnomonDataDriverMongo

os.environ["IS_TEST"]="1"
driver = gnomonDataDriverMongo()


class TestMongo(unittest.TestCase):
    '''
    Tests the mongo connection class.
    '''

    def test_connection(self):
        # test if db is up and connection is working
        assert driver._client

    def test_add(self):
        # add a new document
        driver.insert('{"input":"miaou", "type":"pipeline", "id":"id_1"}')
        driver.insert('{"input":"mia", "type":"pipeline", "id":"id_2"}')
        driver.insert('{"name":"wouf", "pipelines" : [{"id": "id_1", "input": "first_file"}, {"id" : "id_2"}], "type":"run"}')

        res = driver._db.pipelines.find_one()
        assert res["input"] == "miaou"

        res = driver._db.runs.find_one()
        assert res["name"] == "wouf"

        assert driver._db.list_collection_names()[0] == "pipelines"

    def test_find(self):
        driver.insert('{"find": "iwillbefound", "type":"pipeline"}')
        res = driver.find('{"find": "iwillbefound"}')
        assert type(res) is list
        first_res = json.loads(res[0])
        assert first_res["find"] == "iwillbefound"


    def test_delete_one(self):
        driver.insert('{"name":"iwilldisappear", "type":"pipeline"}')
        assert driver.delete_one('{"name":"iwilldisappear"}')


    def test_protect(self):
        driver.protect('{"name":"wouf", "type" : "run"}')
        pass
        # TODO

        # insert a document
        # protect it

        # find it OK

        # try to remove it NOK



#
# test_gnomonTime.py ends here.