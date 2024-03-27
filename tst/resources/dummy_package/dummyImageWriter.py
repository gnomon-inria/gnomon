
import numpy as np
from typing import Any, Dict, List

from gnomon.utils import algorithmPlugin
from gnomon.utils.decorators import imageInput
from gnomon.core import gnomonAbstractImageWriter

@algorithmPlugin(version='0.1.0', coreversion='1.0.0', name="Dummy Image Writer")
@imageInput(attr='img', data_plugin="dummyImageData")
class dummyImageWriter(gnomonAbstractImageWriter):
    def __init__(self):
        super().__init__()
        self.filepath = None
        self.img = {}

    def setPath(self, filepath):
        self.filepath = filepath

    def run(self):
        is_dict = isinstance(self.img, dict)
        if not is_dict:
            print("ERROR: image is not a dictionary")
            return

        has_correct_key = 0. in self.img.keys()
        if not has_correct_key:
            print("ERROR: image does not have the correct key")
            return

        img = self.img[0.]
        is_array = img.__class__.__name__ == "ndarray"
        if not is_array:
            print("ERROR: image is not an array")
            return

        img_shape = img.shape
        if len(img_shape) != 2:
            print("ERROR: image is not 2D")
            return
        
        is_one = img[0][0] == 1.
        if not is_one:
            print("ERROR: image is not 1")
            return 

        # write 1 if the check is correct
        with open(self.filepath, "w") as f:
            f.write("1")
        
    def extensions(self):
        return ["inr", "inr.gz", "mha", ".mha.gz", "tif", "tiff", "czi", "lsm"]
