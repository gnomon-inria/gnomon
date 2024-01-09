import numpy as np
from typing import Any, Dict, List

from gnomon.utils import algorithmPlugin
from gnomon.utils.decorators import imageOutput
from gnomon.core import gnomonAbstractImageReader


@algorithmPlugin(version='0.1.0', coreversion='0.81.1', name="Dummy Image Reader")
@imageOutput(attr='img', data_plugin="dummyImageData")
class dummyImageReader(gnomonAbstractImageReader):
    def __init__(self):
        super().__init__()
        print("INIIIIT", self.__class__)
        self.img = {}
        self.path = None

    def __del__(self):
        print("DEL", self.__class__)
        self.img = {}
        self.path = None


    def setPath(self,path):
        self.path = path

    def run(self):
        self.img = {}
        img = np.ones((100,100))
        time = 0.
        self.img[time] = img

    def extensions(self):
        return ["inr", "inr.gz", "mha", ".mha.gz", "tif", "tiff", "czi", "lsm"]


