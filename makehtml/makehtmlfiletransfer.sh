#!/usr/bin/bash
[[ -e htmlrunningfiletransfer ]] && exit 0
echo $$ > htmlrunningfiletransfer
source $(pwd)/setup_all.sh
ROOT_INCLUDE_PATH=${OPT_SPHENIX}/include:${ROOT_INCLUDE_PATH}
Xvfb :9 -nolisten tcp &
export DISPLAY=unix:9
python3 makehtmlfiletransfer.py >& /sphenix/user/sphnxpro/htmllogs/makehtmlfiletransfer.log
kill $!
rm htmlrunningfiletransfer
# no echos - this will force an email from cron
#echo "Finished"
