#! /bin/bash

source /opt/sphenix/core/bin/sphenix_setup.sh -n new
source /opt/sphenix/core/bin/setup_local.sh /gpfs/mnt/gpfs02/sphenix/user/gmattson/install

infile="joblists/job-$1.list"
outfile_cemc="joblists/out/emcal_runlist-$1.csv"
outfile_ihcal="joblists/out/ihcal_runlist-$1.csv"
outfile_ohcal="joblists/out/ohcal_runlist-$1.csv"

if [[ -f $outfile_cemc ]]
then
    rm $outfile_cemc
fi
if [[ -f $outfile_ihcal ]]
then
    rm $outfile_ihcal
fi
if [[ -f $outfile_ohcal ]]
then
    rm $outfile_ohcal
fi

# find $aggrdir -name 'HIST_CALO*' > aggruns.txt
time cat $infile | xargs -I{} root -b -q ../goodrunlist_calo.C'("'{}'", "'$outfile_cemc'", "'$outfile_ihcal'", "'$outfile_ohcal'")'
