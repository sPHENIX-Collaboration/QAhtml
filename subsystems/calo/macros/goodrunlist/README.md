Steps to produce good run list csv file:

1. Create the list of all aggregate histo files
```$ find /sphenix/data/data02/sphnxpro/QAhtml/aggregated -name 'HIST_CALO*' >
aggruns_all_07052024.txt```

2. `$ python3 ProcessAggFiles.py`

3. Run condor jobs

4. Concatenate job outputs
``` cat condor/joblists/out/emcal-* > emcal_runlist_07052024.csv ```
