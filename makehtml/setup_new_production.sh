#!/bin/bash

# takes arguments production triplet

if [ $# -eq 0 ]; then
    echo "Usage: sh setup_new_production.sh anaXXX_pcdbYYY_vZZZ"
    exit 1
fi

triplet=${1}
echo "setting up for "$triplet

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

newdir=/sphenix/WWW/subsystem/QAHtml_${triplet}
if [ ! -d ${newdir} ]; then
    mkdir ${newdir}
else
    echo "Directory exists"
fi

cp /sphenix/WWW/subsystem/QAHtmlProduction/*.* ${newdir}
cp -r /sphenix/WWW/subsystem/QAHtmlProduction/photos ${newdir}
cp -r /sphenix/WWW/subsystem/QAHtmlProduction/icons ${newdir}

if [ ! -d ${newdir}/physics ]; then
    mkdir ${newdir}/physics
fi

echo "Starting drawing"
sh $PWD/makehtmlcalo.sh calofitting ${newdir}
sh $PWD/makehtmlcalo.sh calo ${newdir}

echo "finished drawing new production "${triplet}
