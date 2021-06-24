import getpass
import json
import os
import subprocess
import weakref

import gnomoncore
from datetime import date
from gnomoncore import gnomonAbstractDataDriver, gnomonAbstractDataDriverPlugin
from PyQt5.QtCore import QSettings
from pymongo import MongoClient


from .gnomonPlugin import gnomonPlugin

def get_username():
    return getpass.getuser()

@gnomonPlugin(namespace=gnomoncore, base_class=gnomonAbstractDataDriver)
class gnomonDataDriverMongo(gnomonAbstractDataDriver):
    def __init__(self):
        super().__init__()

        # 1 launch and connect to the db
        settings = QSettings(QSettings.IniFormat,QSettings.UserScope,"inria","gnomon-core")
        settings.beginGroup("mongo")
        uri = settings.value("uri")
        port = settings.value("port")
        user = settings.value("logging")
        pwd = settings.value("passwd")
        is_test = os.environ["IS_TEST"] == "1"
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
        return "mongo"

    def insert(self, doc):
        # TODO latter
        # depending of the contents of the document, insert into pipeline or runs collection
        doc['user'] = get_username()
        doc['date'] = date.today().isoformat()
        if doc['type'] == 'pipeline':
            self._db.pipelines.insert_one(doc)
        elif doc['type'] == 'run':
            self._db.runs.insert_one(doc)
        else:
            print(f"wrong type of document for: {doc}")
            return False

        return True

    def delete_one(self, key):
        if "type" in key and key["type"] == "run":
            res = self._db.runs.delete_one(key)
        else:
            res = self._db.pipelines.delete_one(key)
        return res.deleted_count == 1

    def protect(self, key):
        to_protect = self.find_one(key)
        assert(to_protect)

        to_protect['expiration_date'] = date.today().replace(year=date.today().year+1).isoformat()
        if 'pipelines' in to_protect:
            # protect the pipelines as well
            p_ids = [p["id"] for p in to_protect["pipelines"] ]
            for p_id in  p_ids:
                # TODO use id or something like name ?
                p = self.find_one({"id" : p_id})
                assert(p)
                self.protect(p)

        self._db.protected.insert_one(to_protect)
        return True

    def find_one(self, query):
        if "type" in query and query["type"] == "run":
            res = self._db.runs.find_one(query)
        else:
            res = self._db.pipelines.find_one(query)
        return res
