FROM continuumio/miniconda3
# https://hub.docker.com/r/continuumio/miniconda3
LABEL maintainer="Jonathan LEGRAND <jonathan.legrand@ens-lyon.fr>"
# Refers to the 'docker' section of Gnomon documentation to use this docker image.

ENV SETUSER=gnomon
# Conda install location (see documentation from dockher-hub):
ENV CONDA_DIR=/opt/conda
ENV PATH /opt/conda/envs/env/bin:$PATH

USER root
# Change shell to 'bash', default is 'sh'
SHELL ["/bin/bash", "-c"]
# Update package manager & install requirements:

RUN apt-get update \
    && apt-get install -yq --no-install-recommends \
#    libfontconfig1 \
#    libfreetype6 \
    libgl1-mesa-glx \
#    libglib2.0-0 \
#    libx11-6 \
#    libx11-xcb1 \
#    libxcb-cursor0 \
    libxt6 \
    libxi6 && \
    apt-get clean && \
    apt-get autoremove && \
    rm -rf /var/lib/apt/lists/* && \
    useradd -m ${SETUSER} && \
    cd /home/${SETUSER} && \
    chown -R ${SETUSER}: /home/${SETUSER} && \
    chown -R $SETUSER: /opt/conda

# Change user
USER ${SETUSER}
WORKDIR /home/${SETUSER}

# RUN conda update -n base -c defaults conda && \
RUN conda init bash && \
    conda create -n gnomon -c gnomon -c dtk-forge -c conda-forge -c mosaic gnomon plugin_tissueimagemesh plugin_tree nomkl && \
    echo "source activate gnomon" > ~/.bashrc && \
    source /home/${SETUSER}/.bashrc && \
    conda clean --all -f -y && \
    find /opt/conda/ -follow -type f -name '*.a' -delete && \
    find /opt/conda/ -follow -type f -name '*.pyc' -delete && \
    find /opt/conda/ -follow -type f -name '*.js.map' -delete

ENV QT_DEBUG_PLUGINS=1
#ENV QT_GRAPHICSSYSTEM="native"

CMD ["/bin/bash", "-c", "source ~/.bashrc && gnomon"]