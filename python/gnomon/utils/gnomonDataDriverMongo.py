import getpass
import json
import os
import subprocess
import traceback
import weakref

from datetime import date
from gnomon.core import gnomonAbstractDataDriver, gnomonAbstractDataDriverPlugin
from pymongo import MongoClient
from bson.objectid import ObjectId

from .gnomonPlugin import corePlugin

def get_username():
    return getpass.getuser()

@corePlugin(version="0.1.0", coreversion="0.19.0", base_class=gnomonAbstractDataDriver)
class gnomonDataDriverMongo(gnomonAbstractDataDriver):
    def __init__(self):
        super().__init__()

        from PySide2.QtCore import QSettings

        # 1 launch and connect to the db
        settings = QSettings(QSettings.IniFormat,QSettings.UserScope,"inria","gnomon-core")
        settings.beginGroup("mongo")
        uri = settings.value("uri")
        port = settings.value("port")
        user = settings.value("logging")
        pwd = settings.value("passwd")

        is_test = "IS_TEST" in os.environ
        if is_test:
            print("MONGO TEST ENVIRONMENT")

        if uri and port and user and pwd and not is_test:
            # try to establish a connection
            self._client = MongoClient(f'{uri}',
                                      port=port,
                                      username=user,
                                      password=pwd,
                                      authSource='gnomon')
            pass
        else:
            dbpath = settings.value("dbpath")
            if not dbpath:
                from pathlib import Path
                dbpath = Path.home() / 'gnomondb'
                Path.mkdir(dbpath, exist_ok=True)
                dbpath = str(dbpath)
                settings.setValue("dbpath", dbpath)
                settings.sync()

            self.process = subprocess.Popen(["mongod",
                                           "--logpath", f"{dbpath}/mongolog.txt",
                                           "--logappend", "--noauth",
                                           "--dbpath", f"{dbpath}",
                                           "--wiredTigerCacheSizeGB", "1"],
                                          env=dict(PATH=os.environ['PATH']))
            self._client = MongoClient('localhost:27017')

        settings.endGroup()

        # 2 set up current db with write restrictions (no update only create and delete)
        if is_test:
            self._db = self._client.test_db
            self._client.drop_database("test_db")

        else:
            self._db = self._client.gnomon

        # register a finalize to close the process
        def closeProcess(p):
            if p:
                print("closing local mongod process.")
                p.terminate()

        self._finalizer = weakref.finalize(self, closeProcess, self.process)

    def name(self):
        return "gnomonDataDriverMongo"

    def _toJson(self, query):
        """internal method to transform a string query to a json document
        If a _id key is present, it will also transform it from a string to an ObjectID

        Args:
            doc (str): the query as a string
        """
        if type(query) is str:
            query = json.loads(query)

        if "_id" in query and type(query["_id"]) is str:
            query["_id"] = ObjectId(query["_id"])

        return query


    def insert(self, doc):
        # if type(doc) is list:
        #     res = True
        #     for single_doc in doc:
        #         res = res and self.insert(single_doc)
        #         return res

        doc = self._toJson(doc)

        doc['user'] = get_username()
        doc['date'] = date.today().isoformat()
        if doc['type'] == 'pipeline':
            res = self._db.pipelines.insert_one(doc)
        elif doc['type'] == 'run':
            res = self._db.runs.insert_one(doc)
        else:
            print(f"wrong type of document for: {doc}")
            return None

        return str(res.inserted_id)

    def delete_one(self, key):
        doc = self.find_one(key)
        if not doc:
            print("cannot delete a unexisting doc")
            return False
        elif "expiration_date" in doc and date.today() < date.fromisoformat(doc["expiration_date"]):
            print(f"cannot delete a protected doc (protected until {doc['expiration_date']})")
            return False

        key = self._toJson(key)

        if "type" in key and key["type"] == "run":
            res = self._db.runs.delete_one(key)
        else:
            res = self._db.pipelines.delete_one(key)

        return res.deleted_count == 1

    def protect(self, key):
        try:
            to_protect = self.find_one(key)

            if not to_protect:
                print(f" cannot found object with key {key} to protect it")
                return False

            exp_date = date.today().replace(year=date.today().year+1).isoformat()
            if 'pipelines' in to_protect:
                self._db.runs.update_one({'_id': to_protect['_id']}, {'$set': {'expiration_date': exp_date} } )

                # protect the pipelines as well
                p_ids = [p["id"] for p in to_protect["pipelines"] ]
                for p_id in  p_ids:
                    self._db.pipelines.update_one({'_id': p_id}, {'$set': {'expiration_date': exp_date} } )
            else:
                self._db.pipelines.update_one({'_id': to_protect['_id']}, {'$set': {'expiration_date': exp_date} } )

            return True
        except Exception:
            traceback.print_exc()
            return False

    def find_one(self, query):
        query = self._toJson(query)
        if "type" in query and query["type"] == "run":
            res = self._db.runs.find_one(query)
        else:
            res = self._db.pipelines.find_one(query)
        return res

    def find(self, query):
        query = self._toJson(query)
        if "type" in query and query["type"] == "run":
            res = [doc for doc in self._db.runs.find(query)]
        elif "type" in query and query["type"] == "pipeline":
            res = [doc for doc in self._db.pipelines.find(query)]
        else:
            res = [doc for doc in self._db.runs.find(query)]
            res += [doc for doc in self._db.pipelines.find(query)]

        for doc in res:
            doc["_id"] = str(doc["_id"])

        return [json.dumps(doc) for doc in res]
