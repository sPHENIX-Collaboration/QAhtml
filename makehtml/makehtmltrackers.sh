#!/bin/bash
[[ -e htmlrunningtrk ]] && exit 0
histtype=$1
echo $$ > htmlrunningtrk$histtype
source ./setup_all.sh
if [ $histtype = "hit" ]; then
    Xvfb :2 -nolisten tcp &
    export DISPLAY=unix:2
elif [ $histtype = "cluster" ]; then
    Xvfb :3 -nolisten tcp &
    export DISPLAY=unix:3
elif [ $histtype = "seed" ]; then
    Xvfb :5 -nolisten tcp &
    export DISPLAY=unix:4
else
    kill $!
    rm htmlrunningtrk$histtype
fi
python3 makehtmltrackers.py -ht $histtype >& /sphenix/u/sphnxpro/qahtml/QAhtml/makehtml/makehtmltrack$histtype.log
kill $!
rm htmlrunningtrk$histtype
# no echos - this will force an email from cron
#echo "Finished"
