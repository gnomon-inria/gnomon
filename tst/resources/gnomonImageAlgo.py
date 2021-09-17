import numpy as np
import scipy.ndimage as nd

from dtkcore import d_bool, d_int, d_real, d_inliststring, d_inliststringlist

import gnomoncore

from gnomon_utils import load_plugin_group, gnomonPlugin, gnomonParametric
from gnomon_utils.gnomonDecorator import gnomonImageInput, gnomonImageOutput

from timagetk.components import SpatialImage

@gnomonPlugin(version='0.1.0', coreversion='0.16.0', namespace=gnomoncore)
@gnomonParametric
@gnomonImageInput(attr='in_img', method='inputImage', setter_method='setInputImage')
@gnomonImageOutput(attr='out_img', method='outputImage')
class imageAlgorithm(gnomoncore.gnomonAbstractFormAlgorithm):

    def __init__(self):
        super().__init__()

        self._parameters = {}
        self._parameters['sigma'] = d_real("The sigma parameter", 1., 0., 5., 2)

        self.in_img = {}
        self.out_img = {}

    def run(self):
        self.out_img = {}

        for time in self.in_img.keys():
            self.out_img[time] = {}

            for channel in self.in_img[time].keys():
                img = self.in_img[time][channel]
                filtered_img = nd.gaussian_filter(img.get_array(),sigma=self['sigma']/np.array(img.voxelsize))
                self.out_img[time][channel] = SpatialImage(filtered_img,voxelsize=img.voxelsize)

