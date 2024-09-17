# Getting Started

This section is here to help you make your first steps with the Gnomon platform. It provides guidelines on how to install it on your system, details of the general principles of the platform and basic step-by-step tutorials to get you started! 

## Installation

The preferred way to install Gnomon is through its **conda** package. If you don't have conda set up on your system, we recommend you to get the latest [Miniconda Distribution](https://docs.conda.io/en/latest/miniconda.html).

::::{tab-set}

:::{tab-item} Conda

To install Gnomon in a new conda environment, you can use the following command:

```bash
conda create -n gnomon -c dtk-forge6 -c gnomon -c mosaic -c morpheme -c conda-forge gnomon
```
::: 
::::

More complete guidelines can be found on the [**Installation**](install) page.

### Installing plugin packages

Gnomon relies of **Plugins**[{fas}`book-open;sd-text-primary fa-2xs`](plugin-definition) for all its processing functionalities, and once you have installed the core platform, you will have you [**Install Plugin Packages**](install_packages) to actually be able to use it.

## General Presentation

Gnomon is a software whose goal is to facilitate the creation of complex processing pipelines through an intuitive user interface. If you want to learn more about the principles guiding its conception, please read [this brief introduction](about).

## Tutorials

For detailed use cases giving a more complete view of what you can do with Gnomon, have a look at the [Tutorials](../user_guide/tutorials) section, or at the [Examples Gallery](../gallery/index).

:::{toctree}
:maxdepth: 1
:hidden:

about
install
install_packages
:::
