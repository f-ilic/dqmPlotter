from os import listdir
from os.path import isfile, join

srcPath = "."
cppFiles = [f for f in listdir(srcPath) if isfile(join(srcPath, f)) and f.split(".")]
print(cppFiles)

for file in cppFiles:
  print(file)

  os.system("root -l " + file + "++")
  
  newfilePrefix = file.replace(".", "_")
  
  os.system("mv " + newfilePrefix + "*" + " ../lib/.")