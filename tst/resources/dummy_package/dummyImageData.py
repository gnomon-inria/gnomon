import numpy as np
from typing import Any, Dict, List

from gnomon.utils import formDataPlugin
from gnomon.core import gnomonAbstractImageData, gnomonImage


@formDataPlugin(version="0.5.0", coreversion="0.81.0", data_setter='set_image', data_getter='get_image', name="DummyImageData")
class dummyImageData(gnomonAbstractImageData):
    def __init__(self, image=None):
        super().__init__()
        self._image = None
        self._vtk_image = {}

    def __del__(self):
        self._image = None
        self._vtk_image = {}

    def set_image(self, img):
        print("SET IMAGE")
        self._image = img

    def get_image(self) -> Any:
        print("GET IMAGE")
        return self._image

    def clone(self):
        return None

    def fromGnomonForm(self, form: gnomonImage) -> None:
        pass

    def metadata(self) -> Dict[str, str]:
        metadata = {"name": "I am dummy"}
        return metadata

    def dataName(self) -> str:
        return "DummyImageData"

    def setImage(self, image, channel: str):
        print("SET IMAGE 2")
        pass

    def image(self, channel: str):
        print("GET IMAGE 2")
        return None
    
    def setChannelName(self, channel: str, name: str):
        pass

    def channels(self) -> List[str]:
        return []
    
    def minValue(self , channel: str) -> int:
        return 0

    def maxValue(self , channel: str) -> int:
        return 1
