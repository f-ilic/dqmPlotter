#!/usr/bin/env python

from os import system, sys, chdir

filesToRebuild = ""
if len(sys.argv) > 1:
  filesToRebuild = ' '.join(sys.argv[1 :])

chdir("src/")
system("python BuildLibs.py " + filesToRebuild)
chdir("../")
system("root -l dqmPlotter.C")

