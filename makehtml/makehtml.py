#!/usr/bin/python3

import glob
import os.path
import subprocess

def runFromPath(fullpathfilename):
    #filename is the last string after spliting by "/"
    #run number will be the first thing after the "-" in the filename
    return int(fullpathfilename.split("/")[-1].split("-")[1])

subsys = ["calo"]
basedir = "/sphenix/data/data02/sphnxpro"
aggpath = basedir+"/QAhtml/aggregated"

for s in subsys:

    #first grab all of the aggregated runs for the subsystem
    #and determine their modification times
    aggfiles = glob.glob(aggpath+"/*"+s.upper()+"*")
    subsysAggRuns = {}
    for f in aggfiles:
        #extract the run number and determine the modification timestamp
        runnumber = runFromPath(f)
        #keep a dictionary of the modification times of the aggregated files
        subsysAggRuns[runnumber] = os.path.getmtime(f)
    #print(subsysAggRuns)

    #now check all current hist files for a subsystem and get the latest modify time for each run
    histFilesModified = {}
    subsyspath = basedir+"/"+s+"hist"
    for rundir in glob.glob(subsyspath+"/*"):
        #find the runs in this directory
        #and the last time it was modified
        for f in glob.glob(rundir+"/*"):
            runnumber = runFromPath(f)
            modTime = os.path.getmtime(f)
            if runnumber in histFilesModified:
                if modTime > histFilesModified[runnumber] :
                    histFilesModified[runnumber] = modTime
            else:
                histFilesModified[runnumber] = modTime
    #print(histFilesModified)

    #now go through mark what needs (re)aggregated
    runsToAgg = []
    for run in histFilesModified:
        if not run in subsysAggRuns:
            runsToAgg.append(run)
        else:
            if histFilesModified[run] > subsysAggRuns[run]:
                runsToAgg.append(run)
    #print(runsToAgg)
    #these are the runs not to aggregate
    #print(set(histFilesModified.keys())^set(runsToAgg))

    #now aggregate and create the HTML
    for run in runsToAgg:
        #make the filelist for aggregations
        lowRun = int(run/100)
        highRun = lowRun+1
        runDir = basedir+"/"+s+"hist/run_000"+str(lowRun)+"00_000"+str(highRun)+"00"
        histFiles = glob.glob(runDir+"/*"+str(run)+"*")
        #print(histFiles)
        #make the output aggregation filename
        pieces = histFiles[0].split("/")[-1].split("-")
        aggFile = aggpath+"/"+pieces[0]+"-"+pieces[1]+"-9000.root"
        #and build the hadd command
        cmd = ["hadd","-ff",aggFile]
        cmd.extend(histFiles)
        print(cmd)
        #and run the hadd command
        #subprocess.run(cmd)

        #now run the HTML piece
        macro = "/sphenix/u/sphnxpro/qahtml/QAhtml/subsystems/"+s+"/macros/draw_"+s+".C(\""+aggFile+"\")"
        print(macro)
        #subprocess.run(["root.exe","-q",])
