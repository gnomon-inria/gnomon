How to make a package

install mamba and boa and anaconda-client:
conda install -c conda-forge mamba boa anaconda-client

Run:
mamba mambabuild conda -c dtk-forge6 -c conda-forge -c morpheme -c mosaic -c gnomon --output-folder conda-bld

Copy the output package path

Create a .conda package:
cph transmute <package path> .conda

The new package should be at the same location as the original way. It's path should be the same but with .conda at
the end instead of .tar.bz2

Upload this new file:
anaconda upload -u gnomon <.conda package path>