#!/bin/bash
[[ -e htmlrunning ]] && exit 0
echo $$ > htmlrunning
source ./setup_all.sh
Xvfb :49 -nolisten tcp &
export DISPLAY=unix:49

#for i in /sphenix/tg/tg01/jets/jamesj3j3/run25_jet_hists/ana502_2025p004_v001/run_66600_66700/HIST_JETQA-00066623-99999.root; do
for i in /sphenix/tg/tg01/jets/jamesj3j3/run25_jet_hists/new_newcdbtag_v001/golden_run_test/AddedSegments/HIST_JETQA-00072*-99999.root ; do
  outfile="output/output_$(basename "$i")"
  root.exe -q "draw_calo_jet.C(\"${i}\", \"${outfile}\", false, true)" #very strange - do not run with root.exe -b 
done

kill $!
rm htmlrunning
