#!/usr/bin/python3

import argparse
import glob
import os
import os.path
import subprocess
import pyodbc
import time
from datetime import datetime

parser = argparse.ArgumentParser(description="Create the HTML files with QA histograms files for all events in a run.")
parser.add_argument("-v","--verbose",help="add additional printing", action="store_true")
parser.add_argument("-t","--test",help="run a verbose test without actually creating the HTML", action="store_true")
args = parser.parse_args()
if args.test and not args.verbose:
    args.verbose = True

subsys = {"filetransfer" : ["FILETRANSFERQA","draw_filetransfer.C"]}

runtypes = ["_run3auau"]

def get_unique_run_dataset_pairs(cursor):
    query = "SELECT DISTINCT runnumber FROM datasets WHERE runnumber > 70000 and runnumber < 100000 and dataset='physics' order by runnumber;"
    if args.verbose:
        print(query)
    cursor.execute(query)
    runnumbers = [(row.runnumber) for row in cursor.fetchall()]
    
    return runnumbers

def get_times(cursor, runnumber):
    query = "SELECT full_file_path,time from files where lfn in (select filename from datasets files where runnumber='{}')".format(runnumber)
    if args.verbose:
        print(query)
    cursor.execute(query)
    return {(row.full_file_path, row.time) for row in cursor.fetchall()}

def main():
    runsToProcess = []
    conn = pyodbc.connect("Driver={PostgreSQL};Server=sphnxhpssdbmaster;Database=RawdataCatalog;READONLY=True")
    cursor = conn.cursor()
    for key, value in subsys.items():
        for runtype in runtypes:
            runs = get_unique_run_dataset_pairs(cursor)
            if args.verbose:
                print(runs)
            for run in runs:
                qafilepath = "/sphenix/WWW/subsystem/QAHtml/physics/run_{:010d}_{:010d}/{}/FileTransferQA_0_{}.png".format(round(run, -3)-1000, round(run,-3), run, run)
                if args.verbose:
                    print("qa file path " + qafilepath)
                fileqatime = 0
                if os.path.exists(qafilepath):
                    fileqatime = os.path.getmtime(qafilepath)
                
                filepath_times = get_times(cursor, run)
                for path, time in filepath_times:
                    dt = datetime.strptime(str(time),"%Y-%m-%d %H:%M:%S")
                    if args.verbose:
                        print("path " + path + " has time " + str(time))
                        print("   corresponding to " + str(dt.timestamp()))
                        print("       with qafile time " + str(fileqatime))
                    if dt.timestamp() > fileqatime:
                        if args.verbose:
                            print("reprocessing " + str(run))
                        runsToProcess.append(run)
                        break
            # now we got all the runs to be reprocessed, so run the macro on them
            for run in runsToProcess:
                macro = "/sphenix/u/sphnxpro/qahtml/QAhtml/subsystems/"+key+"/macros/"+value[1]+"("+str(run)+")"
                cmd = ["root.exe","-q",macro]
                if args.verbose:
                    print(cmd)
                if not args.test:
                    subprocess.run(cmd)
    conn.close()
    print("Finished script")


    
if __name__ == "__main__":
    main()
