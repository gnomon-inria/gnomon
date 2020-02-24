================
Extending Gnomon
================

Existing plugins
====================


+---------------+------------------------+---------------+--------------+--------------+
|Plugins        |package                 |pipeline       |coverage      |conda         |
+===============+========================+===============+==============+==============+
|Data           |*plugin_data*           ||PipeData|     ||CovData|     ||AnaData|     |
+---------------+------------------------+---------------+--------------+--------------+
|TissueImage    |*plugin_tisssueimage*   ||PipeImage|    ||CovImage|    ||AnaImage|    |
+---------------+------------------------+---------------+--------------+--------------+
|TissueMesh     |*plugin_tissuemesh*     ||PipeMesh|     ||CovMesh|     ||AnaMesh|     |
+---------------+------------------------+---------------+--------------+--------------+
|TissueImageMesh|*plugin_tissueimagemesh*||PipeImageMesh|||CovImageMesh|||AnaImageMesh||
+---------------+------------------------+---------------+--------------+--------------+

.. |PipeData| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-data/badges/master/pipeline.svg
.. |PipeImage| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimage/badges/master/pipeline.svg
.. |PipeMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuemesh/badges/master/pipeline.svg
.. |PipeImageMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimagemesh/badges/master/pipeline.svg
.. |CovData| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-data/badges/master/coverage.svg
.. |CovImage| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimage/badges/master/coverage.svg
.. |CovMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuemesh/badges/master/coverage.svg
.. |CovImageMesh| image:: https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimagemesh/badges/master/coverage.svg
.. |AnaData| image:: https://anaconda.org/gnomon/plugin_data/badges/version.svg
.. |AnaImage| image:: https://anaconda.org/gnomon/plugin_data/badges/version.svg
.. |AnaMesh| image:: https://anaconda.org/gnomon/plugin_data/badges/version.svg
.. |AnaImageMesh| image:: https://anaconda.org/gnomon/plugin_data/badges/version.svg

Creating new plugins
====================


.. toctree::
    :maxdepth: 2

    plugins/existing_abstraction_existing_data_plugin

Defining new workspaces
=======================
