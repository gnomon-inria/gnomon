# Installing Plugin Packages


Available plugin packages can be found {bdg-link-primary-line}`here <../plugins/index.html>`
or by running the following command in your environment:

```shell script
gnomon-utils package search
```

## Installation

To install a package use the command:


```shell script
gnomon-utils package install <name-of-the-package>
```

::::{admonition} Recommended Packages
:class: tip
:name: recommended-packages

For instance, for a basic usage, we recommend that you install the following plugin package:

```shell script
gnomon-utils package install gnomon_package_tissueimagemesh
```

:::{dropdown} Alternative command
```shell script
mamba install -c gnomon -c dtk-forge6 -c conda-forge -c mosaic -c morpheme gnomon_package_tissueimagemesh
```
:::
::::

## Updating a package

### **Step 1:** Activate your `(gnomon)` environment
```shell script
conda activate gnomon
```

### **Step 2:**  Update the package
```shell script
gnomon-utils package update <name-of-the-package>
```

:::{dropdown} Alternative command
```shell script
mamba update -c dtk-forge6 -c gnomon -c mosaic -c morpheme -c conda-forge <name-of-the-package>
```
:::