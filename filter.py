import os
import sys
import filterFile

if len(sys.argv) < 2:
    print "WARNING: No input distance threshold value. Setting threshold to 1" 
else:
    threshold = sys.argv[1]

#directory to Molecules folder:
molDir = '../Molecules/'

#for each json file in each molecules folder, generate a new filtered json....
for curdir in os.listdir('../Molecules/'):
    mydir = molDir + curdir
    os.chdir(mydir)
    for somefile in os.listdir('./'):
        #find json files by checking the extension (remember there are also .json.bak files...)
        if somefile[-5:] == '.json':
            print "looking at " + somefile
            filterFile.main(somefile, threshold)
    os.chdir("../../filter")

