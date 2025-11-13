#!/usr/bin/bash
histtype=$1
[[ -e htmlrunningtrk${histtype} ]] && exit 0
echo $$ > htmlrunningtrk${histtype}
source ./setup_all.sh
if [ $histtype = "mvtxhit" ]; then
    Xvfb :2 -nolisten tcp &
    export DISPLAY=unix:2
elif [ $histtype = "intthit" ]; then
    Xvfb :12 -nolisten tcp &
    export DISPLAY=unix:12
elif [ $histtype = "tpchit" ]; then
    Xvfb :13 -nolisten tcp &
    export DISPLAY=unix:13
elif [ $histtype = "tpccluster" ]; then
    Xvfb :7 -nolisten tcp &
    export DISPLAY=unix:7
elif [ $histtype = "tpclaser" ]; then
    Xvfb :14 -nolisten tcp &
elif [ $histtype = "mvtxcluster" ]; then
    Xvfb :10 -nolisten tcp &
    export DISPLAY=unix:10
elif [ $histtype = "inttcluster" ]; then
    Xvfb :11 -nolisten tcp &
    export DISPLAY=unix:11
elif [ $histtype = "tpotcluster" ]; then
    Xvfb :14 -nolisten tcp &
    export DISPLAY=unix:14
elif [ $histtype = "silseed" ]; then
    Xvfb :5 -nolisten tcp &
    export DISPLAY=unix:5
elif [ $histtype = "tpcseed" ]; then
    Xvfb :15 =nolisten tcp &
    export DISPLAY=unix:15
elif [ $histtype = "bco" ]; then
    Xvfb :6 -nolisten tcp &
    export DISPLAY=unix:6
else
    echo "makehtmltrackers.sh called with bad argument: ${histtype}"
    rm htmlrunningtrk$histtype
    exit 1
fi
python3 makehtmltrackers.py -ht $histtype >& /sphenix/user/sphnxpro/htmllogs/makehtmltrack$histtype.log
kill $!
rm htmlrunningtrk$histtype
# no echos - this will force an email from cron
#echo "Finished"
