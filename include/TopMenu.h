#ifndef TOPMENU__H_
#define TOPMENU__H_

// R__LOAD_LIBRARY(lib/Configuration_cpp.so)
#include "../include/Configuration.h"

#include "TApplication.h"
#include "TSystem.h"
#include "TGFrame.h"
#include "TGMenu.h"
#include "TGFileDialog.h"
#include <iostream>

// #pragma link C all globals
extern TSystem* gSystem;
extern TApplication* gApplication;

class TopMenu {
public:
    TopMenu(){
        app_path = gSystem->pwd();
        app_path += "/";
    }

    void HandleMenu(Int_t menu_id);
    void DrawInFrame(TGMainFrame* main_frame);
    void SetCertificatePath(string path);
    void SetPublicKeyPath(string path);
    void UpdateIndex();
    void OpenDialog(Int_t menu_id);

public:  
    string app_path{""};
    
private:
    enum EMyMessageTypes {
        M_USER_CERT,
        M_USER_KEY,
        M_UPDATE_INDEX,
        M_FILE_EXIT
    };
// ClassDef(Menu,1);
};

#endif
