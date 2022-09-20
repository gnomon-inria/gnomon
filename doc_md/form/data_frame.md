# Data Frame

## Description
>Data Frame is a two dimensional tabular data. This data structure contains labeled axes (rows and columns).  
The corresponding class in gnomon is `gnomonDataFrameDataPandas`.

## Default reader plugin
> The reader of data frame form in gnomon is **gnomonDataFrameReaderPandas**. It loads a CSV file with named columns.  
This reader supports one extension `csv`.  

## Default writer plugin
>The default writer of point cloud form is **gnomonDataFrameWriterPandas**

## Gnomon Data Frame Example

![Gnomon Data Frame illustration Image](../_static/data_frame__image.png)

## Plugins which produce Image as output
> Here is a non exhaustive list of some algorithms which produce data frame as output.  
- cellFeaturesTissueImage
- signalQuantificationImageSignal
- surfaceCellCurvature

## A Use Case, Signal Quantification

```python
from copy import deepcopy

import numpy as np
import pandas as pd

from dtkcore import d_inliststringlist
from dtkcore import d_real

from gnomon.utils import algorithmPlugin
from gnomon.utils.decorators import dataFrameOutput
from gnomon.utils.decorators import imageInput
from gnomon.utils.decorators import pointCloudInput
from gnomon.utils.decorators import pointCloudOutput

from gnomon.core import gnomonAbstractPointCloudQuantification

from timagetk.algorithms.signal_quantification import quantify_nuclei_signal_intensity


@algorithmPlugin(version="0.3.1", coreversion="0.71.0")
@imageInput('img', data_plugin='gnomonImageDataMultiChannelImage')
@pointCloudInput("df", data_plugin="gnomonPointCloudDataPandas")
@pointCloudOutput('out_df', data_plugin="gnomonPointCloudDataPandas")
@dataFrameOutput('data_df', data_plugin="gnomonDataFrameDataPandas")
class nucleiSignalQuantificationTimagetk(gnomonAbstractPointCloudQuantification):
    """Quantify image signal intensities on a 3D nuclei point cloud.
   """

    def __init__(self):
        super().__init__()

        self.img = {}
        self.df = {}
        self.out_df = {}
        self.data_df = {}

        self._parameters = {}
        self._parameters['signal_channels'] = d_inliststringlist("Channels", [""], [""], "Channels on which to compute the signal intensity")
        self._parameters['gaussian_sigma'] = d_real("Sigma", 0.5, 0., 50., 2, "Standard deviation of the Gaussian kernel used to smooth signal")


    def run(self):
        self.data_df = {}
        self.out_df = {}

        for time in self.df.keys():
            df = self.df[time]
            out_df = deepcopy(df)

            nuclei_points = df[['center_'+dim for dim in 'xyz']].values

            assert (time in self.img) and (self.img[time] is not None)

            img = self.img[time]

            for signal_name in self['signal_channels']:
                signal_img = img[signal_name]
                out_df[signal_name] = quantify_nuclei_signal_intensity(signal_img, nuclei_points, nuclei_sigma=self['gaussian_sigma'])

            self.out_df[time] = out_df
            
            self.data_df[time] = deepcopy(out_df)
            self.data_df[time]['time'] = time

        if len(self.data_df)>0:
            self.data_df = {0:pd.concat([self.data_df[time] for time in self.data_df.keys()])}

```

