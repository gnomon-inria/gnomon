# Running a pipeline

A pipeline is a graph of tasks made by different plugins. 
Input and output is done through source and sinks nodes,
respectively readers and constructors for sources and writers for sinks.

Pipelines can be saved in pipeline files which are json files. A pipeline can be created
in gnomon.

We will use the following pipeline as an example.

```{graphviz}
digraph pipeline {
    imageReader1 -> imageFilter1
    imageFilter1 -> imageFilter2
    imageFilter2 -> binarization
    binarization -> binaryImageWriter1
}
```
In this pipeline there is one source node and one sink, respectively 
`imageReader1` and `binaryImageWriter1`. A pipeline requires at least
one of each to serve as input output. Both take a path as argument which 
is configurable.

Another class of source available is the `Constructor` class which generates
artificial forms.


The corresponding pipeline file is available at the bottom of this page.

## Run pipeline with command line

The command to run a pipeline file is:
```
gnomon-utils run ./my_pipeline.json
```

To run a pipeline with new input and output path use:
```
gnomon-utils run ./my_pipeline.json --ios imageReader1 ./new/image1.inr binaryImageWriter1 ./new/out.inr
```

## Python API
A pipeline can also be run through the python API in the subpackage
`gnomon.utils.pipelines`.

### Examples

 - Loading and executing a pipeline.
```python
from gnomon.utils.pipelines import load_pipeline

pr = load_pipeline("./my_pipeline.json")
pr.run()
```

 - Loading changing input path and then executing a pipeline.

```python
from gnomon.utils.pipelines import load_pipeline

pr = load_pipeline("./my_pipeline.json")
pr.path_dict["imageReader1"] = "./new/image1.inr"
pr.run()
```

### API documentation
```{eval-rst}
.. automodule:: gnomon.utils.pipelines
    :members:
```
 
## Pipeline file example
```json
{
    "binarization": {
        "description": "",
        "inputs": {
            "initialization": null,
            "input": "imageFilter2 -> output"
        },
        "name": "binarization",
        "outputs": [
            "output"
        ],
        "parameters": {
            "greater_or_lower": {
                "doc": "Channel on which to apply the algorithm",
                "index": 1,
                "label": "> or <",
                "type": "dtkCoreParameterInList<QString>",
                "values": [
                    "greater >",
                    "lower <"
                ]
            },
            "threshold": {
                "decimals": 0,
                "doc": "Image is true where intensity > threshold",
                "label": "Threshold",
                "max": 255,
                "min": 0,
                "type": "dtkCoreParameterNumeric<qlonglong,void>",
                "value": 11
            }
        },
        "plugin_group": "binaryImageFromImage",
        "plugin_name": "binarization",
        "plugin_version": "0.1.0"
    },
    "binaryImageWriter1": {
        "description": "",
        "inputs": {
            "binaryImage": "binarization -> output"
        },
        "name": "binaryImageWriter1",
        "path": "/Users/username/data/out.inr",
        "plugin_group": "binaryImageWriter",
        "plugin_name": "binaryImageWriter",
        "plugin_version": "0.1.0"
    },
    "description": "",
    "fileFormatVersion": "0.0.1",
    "gnomonVersion": "0.20.0",
    "imageFilter1": {
        "description": "",
        "inputs": {
            "input": "imageReader1 -> image",
            "mask": null
        },
        "name": "imageFilter1",
        "outputs": [
            "output"
        ],
        "parameters": {
            "K": {
                "decimals": 1,
                "doc": "Proportional to the cell walls width",
                "label": "Const K",
                "max": 0.3,
                "min": 0.1,
                "type": "dtkCoreParameterNumeric<double,void>",
                "value": 0.2
            },
            "gamma": {
                "decimals": 1,
                "doc": "Amount of gamma correction on the image signal (add contrast in low intensties)",
                "label": "Gamma",
                "max": 15,
                "min": 0,
                "type": "dtkCoreParameterNumeric<double,void>",
                "value": 1
            },
            "nbIter": {
                "decimals": 0,
                "doc": "Number of iterations",
                "label": "Iteration",
                "max": 255,
                "min": 0,
                "type": "dtkCoreParameterNumeric<qlonglong,void>",
                "value": 15
            },
            "sigma": {
                "decimals": 2,
                "doc": "Standard deviation (in µm) of the Gaussian kernel used to smooth signal",
                "label": "Gaussian Sigma",
                "max": 10,
                "min": 0,
                "type": "dtkCoreParameterNumeric<double,void>",
                "value": 0.5
            },
            "upDir": {
                "decimals": 0,
                "doc": "Diffusion tensor is updated every upDiter iteration",
                "label": "Tensor",
                "max": 255,
                "min": 0,
                "type": "dtkCoreParameterNumeric<qlonglong,void>",
                "value": 10
            }
        },
        "plugin_group": "imageFilter",
        "plugin_name": "anisotropic3dImageEnhancement",
        "plugin_version": "0.1.0"
    },
    "imageFilter2": {
        "description": "",
        "inputs": {
            "input": "imageFilter1 -> output",
            "mask": null
        },
        "name": "imageFilter2",
        "outputs": [
            "output"
        ],
        "parameters": {
            "pc_max": {
                "decimals": 2,
                "doc": "Upper percentile use to define the lower range of the input image for contrast stretching",
                "label": "Max percentile",
                "max": 255,
                "min": 0,
                "type": "dtkCoreParameterNumeric<double,void>",
                "value": 99
            },
            "pc_min": {
                "decimals": 2,
                "doc": "Lower percentile use to define the lower range of the input image for contrast stretching",
                "label": "Min percentile",
                "max": 255,
                "min": 0,
                "type": "dtkCoreParameterNumeric<double,void>",
                "value": 2
            }
        },
        "plugin_group": "imageFilter",
        "plugin_name": "globalContrastStretch",
        "plugin_version": "0.1.0"
    },
    "imageReader1": {
        "description": "",
        "inputs": {
        },
        "name": "imageReader1",
        "outputs": [
            "image"
        ],
        "path": "/Users/username/data/0hrs_plant1_trim-acylYFP_small.inr",
        "plugin_group": "imageReader",
        "plugin_name": "imageReaderTimagetk",
        "plugin_version": "0.1.0"
    },
    "input": [
        {
            "imageReader1_path": "imageReader1 -> path"
        }
    ],
    "name": "test_pipeline1",
    "output": [
        {
            "binaryImageWriter1_path": "binaryImageWriter1 -> path"
        }
    ],
    "type": "pipeline"
}
```