#!/bin/bash
[[ -e htmlrunningtrk ]] && exit 0
histtype=$1
echo $$ > htmlrunningtrk$histtype
source ./setup_all.sh
Xvfb :2 -nolisten tcp &
export DISPLAY=unix:2
python3 makehtmltrackers.py -ht $histtype >& /sphenix/u/sphnxpro/qahtml/QAhtml/makehtml/makehtmltrack$histtype.log
kill $!
rm htmlrunningtrk$histtype
# no echos - this will force an email from cron
#echo "Finished"
