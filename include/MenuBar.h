#ifndef TOPMENU__H_
#define TOPMENU__H_

#include "../include/Configuration.h"

#include "TApplication.h"
#include "TSystem.h"
#include "TGFrame.h"
#include "TGMenu.h"
#include "TGFileDialog.h"
#include "RQ_OBJECT.h"

#include <iostream>

// #pragma link C all globals
extern TSystem* gSystem;
extern TApplication* gApplication;


class MenuBar {
    RQ_OBJECT("MenuBar")
public:
    MenuBar() : app_path(string(gSystem->pwd()) + "/") {}

    void HandleMenu(Int_t menu_id);
    void DrawInFrame(TGMainFrame* main_frame);
    void SetCertificatePath(const string& path);
    void SetPublicKeyPath(const string& path);
    void UpdateIndex();                             //*SIGNAL*
    string OpenDialog(Int_t menu_id) const;
    
    void TogglePopupEntry(Int_t menu_id);
    void IndexUpdated();      //*SIGNAL*
    void SignalStatus(std::string* status);      //*SIGNAL*

public:
    const string app_path;

private:
    TGMenuBar*    menu_bar;
    TGPopupMenu*  popup_menu;
    TGPopupMenu*  plugin_menu;


    enum EMyMessageTypes {
        M_USER_CERT,
        M_USER_KEY,
        M_UPDATE_INDEX,
        M_WORK_WITH_LOCAL_COPIES,
        M_FILE_EXIT,
        M_PREVIEW_PLUGIN
    };
};

#endif
