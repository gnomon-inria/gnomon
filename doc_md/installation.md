# How to install gnomon 

## Install gnomon using conda
**Prerequisite**
- Make sure you have conda installed on your pc. If not please 
<a href="https://conda.io/projects/conda/en/latest/user-guide/install/index.html" target="_blank"> install</a> it first and come back.

- You need around *6 GB* of free memory

**Linux user**
- **Step 1:**  Create conda environment with right python version. Then activate this environment
```shell script
conda create -n gnomon python=3.9

conda activate gnomon
```
- **Step 2:** Install Mamba package in your environment
```shell script
conda install -c conda-forge mamba
```
- **Step 3:** Install **gnomon** and its **dependancies**
```shell script
mamba install -c dtk-forge6 -c gnomon -c mosaic -c morpheme -c conda-forge gnomon=0.60
```
- **Step 4:** Congrats you can launch your application
```shell script
gnomon
```