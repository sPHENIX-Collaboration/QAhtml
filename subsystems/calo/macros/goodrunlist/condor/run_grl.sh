#! /bin/bash

source /opt/sphenix/core/bin/sphenix_setup.sh -n new
source /opt/sphenix/core/bin/setup_local.sh /gpfs/mnt/gpfs02/sphenix/user/gmattson/install

infile="joblists/job-$1.list"
outfile="joblists/out/emcal_runlist-$1.csv"

if [[ -f $outfile ]]
then
    rm $outfile
fi

# find $aggrdir -name 'HIST_CALO*' > aggruns.txt
time cat $infile | xargs -I{} root -q ../goodrunlist_calo.C'("'{}'", "'$outfile'")'
