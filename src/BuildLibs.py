from os import listdir, system
from os.path import isfile, join, exists
import sys

srcPath = "."
# UPDATE EVERYTHING OR ONLY SELECTED MODULE
if len(sys.argv) > 1:
  cppFiles = sys.argv[1 : ]
else:
  cppFiles = [f for f in listdir(srcPath) if isfile(join(srcPath, f)) and f.split(".")[1] == "cpp" ]
  
print(cppFiles)

libDir = "../lib/"
stringToPaste = ""

if not exists(libDir):
  print("Creating library directory...")
  system("mkdir " + libDir)

for file in cppFiles:
  
  print("#" * 20)
  print("Processing: %s" % (file))
  command = "root -l <<< \".L " + file + "++\""
  print("Running: %s" % (command))
  res = system(command)  
  print("Exit code: %d" % (res))
  
  if res == 0:
    newfilePrefix = file.replace(".", "_")
    
    print("Moving created libraries...")
    system("mv " + newfilePrefix + "*" + " " + libDir + ".")
    
    stringToPaste += "R__LOAD_LIBRARY(" + '/'.join(libDir.split("/")[1:]) + newfilePrefix + ".so)\n\r"
  else:
    print("Error: building current library failed...")
    
print("\n---YOUR LIBRARY COPY-PASTE CODE:\n\n")

print(stringToPaste)
