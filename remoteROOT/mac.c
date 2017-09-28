int mac()
{
  gEnv->SetValue("Davix.GSI.UserCert", "/afs/cern.ch/user/p/pjurgiel/.globus/copy/usercert.pem");
  gEnv->SetValue("Davix.GSI.UserKey", "/afs/cern.ch/user/p/pjurgiel/.globus/copy/userkey_nopass.pem");

  gEnv->SetValue("Davix.Debug", 1);

  cout << gEnv->GetValue("Davix.Debug", (int)-1);

  TFile* g  = TFile::Open("https://cmsweb.cern.ch/dqm/online/data/browse/Original/00030xxxx/0003038xx/DQM_V0001_PixelPhase1_R000303823.root");
  
  cout << endl;
  if (g)
  {
    cout << "File open" << endl;
  }
  else{
    cout << "Failed to open the file" << endl;
  }
  
  delete g;
  
  return 0;
}
