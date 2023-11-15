import unittest
import numpy as np
from typing import List, Dict

from gnomon.core import initialize, gnomonImage, imageData_pluginFactory, gnomonAbstractImageData
from gnomon.utils import formDataPlugin

instance_count = 0

@formDataPlugin(version="0.5.0", coreversion="0.81.0", data_setter='set_image', data_getter='get_image', name="DummyImagePlugin")
class gnomonImageDataDummyImage(gnomonAbstractImageData):

    def __init__(self, image=None):
        super().__init__()
        global instance_count
        print("iniit  of dummyData")
        instance_count += 1
        self._image = None

    def __del__(self):
        global instance_count
        print("del  of dummyData")
        instance_count -= 1

    def set_image(self, img):
        pass

    def get_image(self):
        pass

    def clone(self):
        return self

    def fromGnomonForm(self, form: gnomonImage) -> None:
        pass

    # Metadata
    def metadata(self) -> Dict[str, str]:
        metadata = {
            "Miaou": "wouf",
        }
        return metadata

    def dataName(self) -> str:
        return "DummyImage"

    def setImage(self, image, channel: str):
        print("set image")

    def image(self, channel: str):
        print("get img")
        return None

    def setChannelName(self, channel: str, name: str):
        pass

    def channels(self) -> List[str]:
        return []

    def minValue(self , channel: str) -> int:
        return 0

    def maxValue(self , channel: str) -> int:
        return 1

    def serialize(self) -> str:
        return ""

    def deserialize(self, serialization: str):
        pass


class TestPythonPlugin(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        initialize("",True)

    def setUp(self):
        self.img_d_factory = imageData_pluginFactory()

    def tearDown(self):
        pass

    def test_create_data(self):
        img_data = self.img_d_factory.create("gnomonImageDataDummyImage")
        assert img_data
        assert img_data.dataName() == "DummyImage"
        img_data.__swig_destroy__(img_data) # creation by factory, explicit destruction needed!

    def test_create_img(self):
        img_data = self.img_d_factory.create("gnomonImageDataDummyImage")
        img = gnomonImage(img_data)
        assert img.data().dataName() == "DummyImage"
        assert img.data().pluginName() == "gnomonImageDataDummyImage"
        # notice that there is no __swig_destroy__  . It will be done by the form (gnomonImage)

    # name with z to be executed at the end
    def test_z_instance_numbers(self):
        global instance_count
        print(instance_count)
        assert instance_count == 0
