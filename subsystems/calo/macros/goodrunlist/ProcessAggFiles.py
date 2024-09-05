import re
import os.path

if __name__ == "__main__":
    infile = 'aggruns_all_08072024.txt'
    outfile = 'aggruns_good_08072024.txt'
    runinfo = {} # runnum: [runnum, tag, aggfile (absolute path)] # see next comment
    joblistdir = 'condor/joblists/'

    # get the list of aggregated histo files with a find command
    # $ find /sphenix/data/data02/sphnxpro/QAhtml/aggregated -name 'HIST_CALO*' > aggruns_all_07052024.txt

    # Read in the list of all aggregate files and write out a list with only
    # the most recent production tag for each run
    with open(infile, 'r') as f:
        for line in f:
            basename = os.path.basename(line) # strip the path from the file name
            m = re.search(r'000\d{5}', basename) # runnum
            runnum = 0
            if m:
                runnum = int(m.group())
                # print(f'Found runnum {runnum} in {basename}')
            m2 = re.search(r'2024p00(\d)', basename) # production tag
            tag = 0
            if m2:
                tag = int(m2.group(1))
                # print(f'Found production tag 2024p00{tag} in {basename}')
            if not runnum in runinfo:
                runinfo[runnum] = [runnum, tag, line]
            else: # if runnum already exists, check the tag
                oldtag = runinfo[runnum][1]
                if tag > oldtag: # use only the most recent tag
                    runinfo[runnum][1] = tag
                    runinfo[runnum][2] = line

    # write aggregate files to outfile
    with open(outfile, 'w') as f:
        for key, value in runinfo.items():
            f.write(value[2])

    # break up the list of aggregate files into smaller lists for condor jobs
    # write multiple per-job file lists with 10 aggregate files per job
    filesperjob = 10 # number of aggregate files to process in one job
    jobnum = 0
    filesthisjob = 0
    with open(outfile, 'r') as f:
        lines = f.readlines()
        maxjobs = len(lines)//filesperjob + 1
        while jobnum < maxjobs:
            jobfile = joblistdir + 'job-' + str(jobnum) + '.list'
            with open(jobfile, 'w') as h:
                while filesthisjob < filesperjob:
                    index = jobnum*filesperjob + filesthisjob
                    if index < len(lines):
                        h.write(lines[index])
                    filesthisjob += 1
            filesthisjob = 0
            jobnum += 1


