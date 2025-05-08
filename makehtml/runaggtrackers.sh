#!/usr/bin/bash
histtype=$1
[[ -e aggrunningtrack${histtype} ]] && exit 0
echo $$ > aggrunningtrack${histtype}
source ./setup_all.sh
python3 TrackerDBAgg.py -ht $histtype >& /sphenix/u/sphnxpro/qahtml/QAhtml/makehtml/runaggtrack${histtype}.log

rm aggrunningtrack${histtype}
# no echos - this will force an email from cron
#echo "Finished"
