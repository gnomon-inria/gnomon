===================
Docker images
===================

These steps require a proper setup of ``docker``.
Follows the official instructions here: `<https://docs.docker.com/install/>`_


Building the 'gnomon' docker image
==================================

To build the image, clone the **gnomon** repository, then go to the ``gnomon/`` folder and run:

.. code-block::

   docker build -t gnomon docker/


Using Docker image with Linux
=============================

Using the image from Docker Hub:

.. code-block::

   $ xhost +local:root;
   $ docker run -it \
   -e DISPLAY=unix$DISPLAY \
   --workdir=$(pwd) \
   --volume="/home/$USER:/home/$USER" \
   --volume="/tmp/.X11-unix:/tmp/.X11-unix" \
   inriamosaic/gnomon

After successfully building your own image:

.. code-block::

   $ xhost +local:root;
   $ docker run -it \
   -e DISPLAY=unix$DISPLAY \
   --workdir=$(pwd) \
   --volume="/home/$USER:/home/$USER" \
   --volume="/tmp/.X11-unix:/tmp/.X11-unix" \
   gnomon


With Mac OS
===========

Source: `<https://medium.com/@mr.sahputra/running-qt-application-using-docker-on-macos-x-ad2e9d34532a>`_

Steps should be:

   1. Make sure ``XQuartz`` running on MacOS X.

   2. Start ``socat``:

   .. code-block::

      $ socat TCP-LISTEN:6000,reuseaddr,fork UNIX-CLIENT:\"$DISPLAY\"

   3. Run docker container, for example:

   .. code-block::

      $ IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
      $ xhost + $IP; \
      $ docker run -it \
      -e DISPLAY=$IP:0 \
      --volume="/Users/$USER:/home/$USER" \
      --workdir=/home/$USER/ \
      -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
      gnomon