#!/bin/bash

source ~/miniconda3/etc/profile.d/conda.sh

conda activate gnomon-dev
current_dir=`pwd`
export PYTHONPATH=${CONDA_PREFIX}/lib/cmake/dtkImaging/lib/python3.7/site-packages:${current_dir}/lib/python3.7/site-packages

# clone repos
cd ..
if [ ! -d "gnomon-packages"]
then
    mkdir gnomon-packages
    cd gnomon-packages
    git clone git@gitlab.inria.fr:gnomon/gnomon-packages/gnomon-package-data
    git clone git@gitlab.inria.fr:gnomon/gnomon-packages/gnomon-package-tissueimage
    git clone git@gitlab.inria.fr:gnomon/gnomon-packages/gnomon-package-tissuelineage
    git clone git@gitlab.inria.fr:gnomon/gnomon-packages/gnomon-package-tissuefem
    git clone git@gitlab.inria.fr:gnomon/gnomon-packages/gnomon-package-tissueimagemesh
    git clone git@gitlab.inria.fr:gnomon/gnomon-packages/gnomon-package-tissuemesh
fi

# install plugins
for d in */; do
    cd $d
    git checkout develop
    git pull
    cp conda/env.yaml conda/env.yaml.bak
    sed -i "/gnomon/ c#gnomon" conda/env.yaml
    mamba env update -n gnomon-dev -f conda/env.yaml
    rm conda/env.yaml
    mv conda/env.yaml.bak conda/env.yaml
    python setup.py install
    cd ..
done
