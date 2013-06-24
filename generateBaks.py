#searches in the ../Molecules/ directory for all .json files and back them up (saves them to the same filename...
# ...with additional .bak extension)
#DON'T RUN THIS UNLESS THE CURRENT JSON FILES ARE THE ORIGINALS!

import os

molDir = '../Molecules/'

for curdir in os.listdir('../Molecules/'):
    mydir = molDir + curdir
    print "looking in " + mydir 
    os.chdir(mydir)
    for somefile in os.listdir('./'):
        print "...looking at file" + somefile
        if '.json' in somefile:
            os.system("cp " + somefile + " " + somefile + ".bak")
    os.chdir("../../filter")
    print "cwd: " + os.getcwd()
