# Binary Image

## Description
>A gnomon Binary Image is a data structure which could hold time series binary (black or white) image.  
The corresponding class in gnomon is `binaryImageDataSpatialImage`.  
It gives possibility to access some information about image like:
- Dimensions
- Voxel Size

## Default reader plugin
>The default reader of binary image form  is **binaryImageReader**. Which reads a 3D microscopy intensity image file.  
Extensions supported by this reader are : `inr, inr.gz, mha, tif`.  

## Default writer plugin
>The default writer of binary image form is **binaryImageWriter**

## Gnomon Binary Image Example

![Gnomon Bimary Image illustration](../_static/binary_image.png)

## Plugins which took Binary Image as input

>Here is a non exhaustive list of some algorithms which took this form as input.  
- edgeEnhancementBinaryImage
- lsmContour

## Plugins which produce Binary Image as output
> Here is a non exhaustive list of some algorithms which produce this form as output.  
- lsmContour
- binarization

## A Use Case, Resampling an image
```python
import numpy as np

from dtkcore import d_int, d_inliststring

import gnomon.core as gnomoncore
from gnomon.core import gnomonAbstractBinaryImageFromImage

from gnomon.utils import algorithmPlugin
from gnomon.utils.decorators import imageInput, binaryImageInput, binaryImageOutput
from gnomon_package_tissueimage.form.imageData.gnomonImageDataMultiChannelImage import gnomonImageDataMultiChannelImage
from gnomon_package_imageenhancement.form.binaryImageDataSpatialImage import binaryImageDataSpatialImage


@algorithmPlugin(version="0.3.1", coreversion="0.80.0")
@imageInput('img_dict', gnomonImageDataMultiChannelImage)
@binaryImageInput('init', binaryImageDataSpatialImage)
@binaryImageOutput('b_img', binaryImageDataSpatialImage)
class binarization(gnomonAbstractBinaryImageFromImage):
    """
    Image Binarization Plugin
    """
    def __init__(self):
        super().__init__()

        self._parameters = {
            'channel': d_inliststring("Channel", "", [""], "Channel on which to apply the algorithm"),
            'greater_or_lower': d_inliststring("> or <", "greater >", ["greater >", "lower <"], "Channel on which to apply the algorithm"),
            "threshold": d_int("Threshold", 127, 0, 255, "Image is true where intensity > threshold"),
        }

        self.img_dict = {}
        self.init = {}
        self.b_img = {}
    
    def run(self):
        self.b_img = {}
        for time in self.img_dict.keys():
            in_img = self.img_dict[time]

            for channel in in_img.keys():
                if 'channel' not in self._parameters.keys() or channel == self['channel']:
                    if self['greater_or_lower'] == 'greater >':
                        binary_img = in_img[channel] > self['threshold']
                    else:
                        binary_img = in_img[channel] < self['threshold']
                    self.b_img[time] = binary_img.astype('bool')
```
