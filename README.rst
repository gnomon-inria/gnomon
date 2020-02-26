The official documentation can be found here: [Gnomon](https://gnomon.gitlabpages.inria.fr/gnomon).

Existing plugins
====================


+---------------+------------------------+------------------+----------------+-----------------+
|Plugins        |package                 |pipeline          |coverage        |conda            |
+===============+========================+==================+================+=================+
|Data           |*plugin_data*           ||PipeData|_       ||CovData|       ||AnaData|        |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueImage    |*plugin_tisssueimage*   ||PipeImage|_      ||CovImage|      ||AnaImage|       |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueFem      |*plugin_tisssuefem*     ||PipeFem|_        ||CovFem|        ||AnaFem|         |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueLineage  |*plugin_tissuelineage*  ||Pipelineage|_    ||Covlineage|    ||Analineage|     |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueMesh     |*plugin_tissuemesh*     ||PipeMesh|_       ||CovMesh|       ||AnaMesh|        |
+---------------+------------------------+------------------+----------------+-----------------+
|TissueImageMesh|*plugin_tissueimagemesh*||PipeImageMesh|_  ||CovImageMesh|  ||AnaImageMesh|   |
+---------------+------------------------+------------------+----------------+-----------------+
|Tree           |*plugin_tree*           ||PipeTree|_       ||CovTree|       ||AnaTree|        |
+---------------+------------------------+------------------+----------------+-----------------+
|TreeLSystem    |*plugin_treelsystem*    ||PipeTreeLSystem|_||CovTreeLSystem|||AnaTreeLSystem| |
+---------------+------------------------+------------------+----------------+-----------------+
|TreeGraph      |*plugin_treegraph*      ||PipeTreeGraph|_  ||CovTreeGraph|  ||AnaTreeGraph|   |
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
.. |AnaImage| image:: https://anaconda.org/gnomon/plugin_tissueimage/badges/version.svg
.. |AnaFem| image:: https://anaconda.org/gnomon/plugin_tissuefem/badges/version.svg
.. |Analineage| image:: https://anaconda.org/gnomon/plugin_tissuelineage/badges/version.svg
.. |AnaMesh| image:: https://anaconda.org/gnomon/plugin_data/badges/version.svg
.. |AnaImageMesh| image:: https://anaconda.org/gnomon/plugin_data/badges/version.svg
.. |AnaTree| image:: https://anaconda.org/gnomon/plugin_tree/badges/version.svg
.. |AnaTreeLSystem| image:: https://anaconda.org/gnomon/plugin_treelsystem/badges/version.svg
.. |AnaTreeGraph| image:: https://anaconda.org/gnomon/plugin_treegraph/badges/version.svg



To install a plugin : ``conda install <plugin_name> -c gnomon -c mosaic -c dtk-forge -c fredboudon -c conda-forge``
