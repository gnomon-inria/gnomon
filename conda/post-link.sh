#!/bin/bash

if [ "$(HAS_GUI)" == "FALSE" ]; then
    # This post-link script is only for the GUI app
    exit 0;
fi

${PREFIX}/bin/pip install morphonet
${PREFIX}/bin/pip install 'python-lsp-server[websockets]'

envname=$(basename ${PREFIX})

if [ `uname` == Linux ]
then
    # launch script
    cat > ${PREFIX}/bin/launch_gnomon.sh <<EOF
#!/bin/sh
. "${PREFIX}/../../etc/profile.d/conda.sh" && conda activate ${envname} && gnomon

EOF

    chmod +x ${PREFIX}/bin/launch_gnomon.sh

    # desktop file
    cat > /home/$USER/.local/share/applications/gnomon.desktop <<EOF
#!/usr/bin/env xdg-open
[Desktop Entry]
Version=1.0
Terminal=true
Type=Application
Name=Gnomon
Exec=sh ${PREFIX}/bin/launch_gnomon.sh
Icon=${PREFIX}/resources/gnomon_logo.png

EOF
fi

if [ `uname` == Darwin ]
then
    ln -s ${PREFIX}/bin/Gnomon.app /Applications/
    mv ${PREFIX}/bin/Gnomon.app/Contents/MacOS/gnomon ${PREFIX}/bin/Gnomon.app/Contents/Resources/gnomon_app
    cat > ${PREFIX}/bin/Gnomon.app/Contents/MacOS/gnomon <<EOF
#!/bin/sh
. "${PREFIX}/../../etc/profile.d/conda.sh" && conda activate ${envname} && ${PREFIX}/bin/Gnomon.app/Contents/Resources/gnomon_app
EOF
    chmod +x ${PREFIX}/bin/Gnomon.app/Contents/MacOs/gnomon
fi
