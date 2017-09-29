from ROOT import *

def TraverseDirTree(d, lvl):
  # print(type(d))
  # print(dir(d))
  for obj in d.GetListOfKeys():
    # print(dir(obj))
    print(" "*lvl + obj.GetName())
    if obj.IsFolder():
      TraverseDirTree(obj.ReadObj(), lvl + 1)
  
pathToGlobus = "/afs/cern.ch/user/p/pjurgiel/.globus/copy/"

gEnv.SetValue("Davix.GSI.UserCert", pathToGlobus + "usercert.pem")
gEnv.SetValue("Davix.GSI.UserKey", pathToGlobus + "userkey_nopass.pem")

gEnv.SetValue("Davix.Debug", "1")

# gEnv.Print()

httpsMainDir = "https://cmsweb.cern.ch/dqm/online/data/browse/Original/"
hostURL = "https://cmsweb.cern.ch"

g = TFile.Open(httpsMainDir + "00030xxxx/0003038xx/DQM_V0001_PixelPhase1_R000303823.root")

if g and g.IsOpen():
  print("File successfully open!")
  # CUSTOM TREE PRINT
  # for obj in g.GetListOfKeys():
    # print(obj.GetName())
    # if obj.IsFolder():
      # TraverseDirTree(obj.ReadObj(), 1)
  
  pMyMainFrame = TGMainFrame(gClient.GetDefaultRoot(), 300, 600);
  pMyMainFrame.MapWindow();
  pMyMainFrame.SetEditable();
  pBrowser = TGFileBrowser(pMyMainFrame);
  pMyMainFrame.SetEditable(kFALSE);
  pBrowser.AddFSDirectory("/", "/");
  pBrowser.GotoDir(gSystem.pwd());
  pMyMainFrame.MapSubwindows();
  pMyMainFrame.Layout();
    
  
  g.Close()
else:
  print("Failed to load the file!")

