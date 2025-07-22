#!/bin/bash
[[ -e htmlrunning ]] && exit 0
echo $$ > htmlrunning
source ./setup_all.sh
Xvfb :2 -nolisten tcp &
export DISPLAY=unix:2

for i in /sphenix/tg/tg01/jets/vbailey/run25_jet_hists/new_newcdbtag_v005/HIST_JETQA-*.root; do
  outfile="output/output_$(basename "$i")"
  root.exe -q "draw_calo_jet.C(\"${i}\", \"${outfile}\", false, true)" #very strange - do not run with root.exe -b 
done

kill $!
rm htmlrunning
