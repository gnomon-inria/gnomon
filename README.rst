The official documentation can be found here: [Gnomon](https://gnomon.gitlabpages.inria.fr/gnomon).

Existing plugins
====================


+---------------+------------------------+------------------+----------------+-----------------+
|Plugins        |package                 |pipeline          |coverage        |conda            |
+===============+========================+==================+================+=================+
|Data           |*plugin_data*           ||PipeData|_       ||CovData|       ||AnaData|_       |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueImage    |*plugin_tisssueimage*   ||PipeImage|_      ||CovImage|      ||AnaImage|_      |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueFem      |*plugin_tisssuefem*     ||PipeFem|_        ||CovFem|        ||AnaFem|_        |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueLineage  |*plugin_tissuelineage*  ||Pipelineage|_    ||Covlineage|    ||Analineage|_    |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueMesh     |*plugin_tissuemesh*     ||PipeMesh|_       ||CovMesh|       ||AnaMesh|_       |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueImageMesh|*plugin_tissueimagemesh*||PipeImageMesh|_  ||CovImageMesh|  ||AnaImageMesh|_  |
+---------------+------------------------+------------------+----------------+-----------------+
|Tree           |*plugin_tree*           ||PipeTree|_       ||CovTree|       ||AnaTree|_       |
+---------------+------------------------+------------------+----------------+-----------------+
|TreeLSystem    |*plugin_treelsystem*    ||PipeTreeLSystem|_||CovTreeLSystem|||AnaTreeLSystem|_|
+---------------+------------------------+------------------+----------------+-----------------+
|TreeGraph      |*plugin_treegraph*      ||PipeTreeGraph|_  ||CovTreeGraph|  ||AnaTreeGraph|_  |
+---------------+------------------------+------------------+----------------+-----------------+

.. |PipeData| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-data/badges/master/pipeline.svg
.. _PipeData : https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-data/pipelines/latest
.. |PipeImage| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimage/badges/master/pipeline.svg
.. _PipeImage : https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimage/pipelines/latest
.. |PipeFem| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuefem/badges/master/pipeline.svg
.. _PipeFem : https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuefem/pipelines/latest
.. |PipeMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuemesh/badges/master/pipeline.svg
.. _PipeMesh : https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuemesh/pipelines/latest
.. |Pipelineage| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuelineage/badges/master/pipeline.svg
.. _Pipelineage : https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuelineage/pipelines/latest
.. |PipeImageMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimagemesh/badges/master/pipeline.svg
.. _PipeImageMesh : https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimagemesh/pipelines/latest
.. |PipeTree| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tree/badges/master/pipeline.svg
.. _PipeTree : https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tree/pipelines/latest
.. |PipeTreeLSystem| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-treelsystem/badges/master/pipeline.svg
.. _PipeTreeLSystem : https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-treelsystem/pipelines/latest
.. |PipeTreeGraph| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-treegraph/badges/master/pipeline.svg
.. _PipeTreeGraph : https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-treegraph/pipelines/latest
.. |CovData| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-data/badges/master/coverage.svg
.. |CovImage| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimage/badges/master/coverage.svg
.. |CovFem| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuefem/badges/master/coverage.svg
.. |Covlineage| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuelineage/badges/master/coverage.svg
.. |CovMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuemesh/badges/master/coverage.svg
.. |CovImageMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimagemesh/badges/master/coverage.svg
.. |CovTree| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tree/badges/master/coverage.svg
.. |CovTreeLSystem| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-treelsystem/badges/master/coverage.svg
.. |CovTreeGraph| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-treegraph/badges/master/coverage.svg
.. |AnaData| image:: https://anaconda.org/gnomon/plugin_data/badges/version.svg
.. _AnaData : https://anaconda.org/gnomon/plugin_data
.. |AnaImage| image:: https://anaconda.org/gnomon/plugin_tissueimage/badges/version.svg
.. _AnaImage : https://anaconda.org/gnomon/plugin_tissueimage
.. |AnaFem| image:: https://anaconda.org/gnomon/plugin_tissuefem/badges/version.svg
.. _AnaFem : https://anaconda.org/gnomon/plugin_tissuefem
.. |Analineage| image:: https://anaconda.org/gnomon/plugin_tissuelineage/badges/version.svg
.. _Analineage : https://anaconda.org/gnomon/plugin_tissuelineage
.. |AnaMesh| image:: https://anaconda.org/gnomon/plugin_tissuemesh/badges/version.svg
.. _AnaMesh : https://anaconda.org/gnomon/plugin_tissuemesh
.. |AnaImageMesh| image:: https://anaconda.org/gnomon/plugin_tissueimagemesh/badges/version.svg
.. _AnaImageMesh : https://anaconda.org/gnomon/plugin_tissueimagemesh
.. |AnaTree| image:: https://anaconda.org/gnomon/plugin_tree/badges/version.svg
.. _AnaTree : https://anaconda.org/gnomon/plugin_tree
.. |AnaTreeLSystem| image:: https://anaconda.org/gnomon/plugin_treelsystem/badges/version.svg
.. _AnaTreeLSystem : https://anaconda.org/gnomon/plugin_treelsystem
.. |AnaTreeGraph| image:: https://anaconda.org/gnomon/plugin_treegraph/badges/version.svg
.. _AnaTreeGraph : https://anaconda.org/gnomon/plugin_treegraph



To install a plugin : ``conda install -c gnomon -c mosaic -c dtk-forge -c fredboudon -c conda-forge <package>``


# Mongo

a mongo instance will be automatically launched on gnomon start if there is none configured. The local instance can be tuned with
 the qsettings parameters.

A connection to an existing mongo db will try to be established if the following variables are set in the qsettings file:
```
[mongo]
uri
port
login
passwd
```

If a connection to an existing db is not possible, a local one will be created in the directory defined by the qsetting `mongo/dbpath`
or in the directory `~/gnomondb` if the qsetting is not set.

