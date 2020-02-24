================
Extending Gnomon
================

Existing plugins
====================


+---------------+------------------------+-----------------+----------------+-----------------+
|Plugins        |package                 |pipeline         |coverage        |conda            |
+===============+========================+=================+================+=================+
|Data           |*plugin_data*           ||PipeData|       ||CovData|       ||AnaData|        |
+---------------+------------------------+-----------------+----------------+-----------------+
|TissueImage    |*plugin_tisssueimage*   ||PipeImage|      ||CovImage|      ||AnaImage|       |
+---------------+------------------------+-----------------+----------------+-----------------+
|TissueFem      |*plugin_tisssuefem*     ||PipeFem|        ||CovFem|        ||AnaFem|         |
+---------------+------------------------+-----------------+----------------+-----------------+
|TissueLineage  |*plugin_tissuelineage*  ||Pipelineage|    ||Covlineage|    ||Analineage|     |
+---------------+------------------------+-----------------+----------------+-----------------+
|TissueMesh     |*plugin_tissuemesh*     ||PipeMesh|       ||CovMesh|       ||AnaMesh|        |
+---------------+------------------------+-----------------+----------------+-----------------+
|TissueImageMesh|*plugin_tissueimagemesh*||PipeImageMesh|  ||CovImageMesh|  ||AnaImageMesh|   |
+---------------+------------------------+-----------------+----------------+-----------------+
|Tree           |*plugin_tree*           ||PipeTree|       ||CovTree|       ||AnaTree|        |
+---------------+------------------------+-----------------+----------------+-----------------+
|TreeLSystem    |*plugin_treelsystem*    ||PipeTreeLSystem|||CovTreeLSystem|||AnaTreeLSystem| |
+---------------+------------------------+-----------------+----------------+-----------------+
|TreeGraph      |*plugin_treegraph*      ||PipeTreeGraph|  ||CovTreeGraph|  ||AnaTreeGraph|   |
+---------------+------------------------+-----------------+----------------+-----------------+

.. |PipeData| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-data/badges/master/pipeline.svg
.. |PipeImage| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimage/badges/master/pipeline.svg
.. |PipeFem| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuefem/badges/master/pipeline.svg
.. |PipeMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuemesh/badges/master/pipeline.svg
.. |Pipelineage| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuelineage/badges/master/pipeline.svg
.. |PipeImageMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimagemesh/badges/master/pipeline.svg
.. |PipeTree| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tree/badges/master/pipeline.svg
.. |PipeTreeLSystem| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-treelsystem/badges/master/pipeline.svg
.. |PipeTreeGraph| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-treegraph/badges/master/pipeline.svg
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

Creating new plugins
====================


.. toctree::
    :maxdepth: 2

    plugins/existing_abstraction_existing_data_plugin

Defining new workspaces
=======================
