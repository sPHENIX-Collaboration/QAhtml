#!/bin/bash
histtype=$1
[[ -e htmlrunning${histtype} ]] && exit 0
echo $$ > htmlrunning${histtype}
source ./setup_all.sh
if [ $histtype = "calofitting" ]; then
    Xvfb :16 -nolisten tcp &
    export DISPLAY=unix:16
elif [ $histtype = "calo" ]; then
    Xvfb :17 -nolisten tcp &
    export DISPLAY=unix:17
elif [ $histtype = "global" ]; then
    Xvfb :18 -nolisten tcp &
    export DISPLAY=unix:18
else
    echo "makehtmlcalo.sh called with bad argument: ${histtype}"
    rm htmlrunning$histtype
    exit 1
fi

python3 makehtmlcalo.py -ht $histtype >& /sphenix/user/sphnxpro/htmllogs/makehtml$histtype.log
kill $!
rm htmlrunning$histtype
rm -f deadHotTowers*HIST_CALO*.root # -f: no error if no files exist
# no echos - this will force an email from cron
#echo "Finished"
