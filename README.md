# Gnomon

<p align="center">
  <img src="https://github.com/gnomon-inria/gnomon/raw/master/doc/_static/gnomon_logo.png" alt="Gnomon" width="200" /> 
</p>

> A computational platform to analyze and simulate the development of living forms in 3D.

<p align="center">
  <img src="https://github.com/gnomon-inria/gnomon/raw/master/doc/_static/index/gnomon_screenshot_quantification.png)" alt="Gnomon Workspace" width="600" /> 
</p>

**Gnomon** is an open-source platform that allows to intuitively design reproducible computational pipelines for the analysis and simulation of morphogenesis. It manipulates **3D+t representations of developing living forms**, and lets the user seamlessly interoperate plugins to visualize and process them in dedicated workspaces.

## Why Gnomon?

Studying morphogenesis requires combining computational tools that use different data structures and rely on independent software libraries. Handling such complex processing pipelines and turing them into reproducible tools is a major challenge.

Gnomon addresses this through three complementary principles:
- **Interoperate & extend** (implementation-agnostic plugin architecture)
- **Reproduce computations** (implicit replayable pipeline construction)
- **Design intuitively** (workspace-based GUI and interactive visualization)

## Installation

Gnomon is natively supported on **Linux and macOS**. Experimental use on Windows is possible through **WSL (Windows Subsystem for Linux)**.

The recommended installation method is through [conda](https://docs.conda.io/en/latest/miniconda.html). The installation requires approximately 4 GB of disk space, including the environment.

```bash
conda create -n gnomon python=3.9
conda activate gnomon
```

Then install Gnomon and its dependencies, together with recommended [plugin packages](https://gnomon.gitlabpages.inria.fr/gnomon/plugins/packages/):

```bash
conda install -c dtk-forge6 -c gnomon -c mosaic -c morpheme -c conda-forge gnomon gnomon_package_tisueimagemesh
```

Launch the application with:

```bash
gnomon
```

For the complete installation instructions, including macOS ARM, Windows/WSL, and plugin package installation, see the [installation guide](https://gnomon.gitlabpages.inria.fr/gnomon/quickstart/install.html).

## Documentation

- [Getting started](https://gnomon.gitlabpages.inria.fr/gnomon/quickstart/index.html)
- [Installation](https://gnomon.gitlabpages.inria.fr/gnomon/quickstart/install.html)
- [User guide](https://gnomon.gitlabpages.inria.fr/gnomon/user_guide/index.html)
- [Examples gallery](https://gnomon.gitlabpages.inria.fr/gnomon/gallery/index.html)
- [Why Gnomon?](https://gnomon.gitlabpages.inria.fr/gnomon/quickstart/about.html)

## Project and research context

Gnomon is intended as an integrated environment for studying **morphogenesis in biology**. The project has been led since 2019 by the **Mosaic Inria project-team**, directed by Christophe Godin, with support from **Inria** and **INRAE**. The project is hosted by the RDP Laboratory at ENS de Lyon.

- [Mosaic Inria project-team](https://team.inria.fr/mosaic/en/)
- [RDP Laboratory](https://www.ens-lyon.fr/RDP/?lang=en)

## Citation

If you use Gnomon in your research, please refer to [the project's documentation website](https://gnomon.gitlabpages.inria.fr/gnomon)
