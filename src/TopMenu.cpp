#include "../include/TopMenu.h"

// #ifndef __CINT__
// ClassImp(Menu);
// #endif

void Menu::OpenDialog(Int_t menu_id) {
    cout << "[ OK ] Open File Dialog" << endl;
    TGFileInfo file_info_;
    const char *filetypes[] = {"Certificate", "*.pem", 0, 0};
    file_info_.fFileTypes = filetypes;
    TGFileDialog* loadDialog = new TGFileDialog(gClient->GetDefaultRoot(), nullptr, kFDOpen, &file_info_);
    (menu_id == M_USER_CERT) ? this->SetCertificatePath(file_info_.fFilename) : this->SetPublicKeyPath(file_info_.fFilename);
}

void Menu::HandleMenu(Int_t menu_id) {
    switch (menu_id) {
    case M_USER_CERT: {
        OpenDialog(menu_id);
        break;
    }
    case M_USER_KEY:{
        OpenDialog(menu_id);
        break;
    }
    case M_UPDATE_INDEX:{
        this->UpdateIndex();
        break;
    }
    case M_FILE_EXIT:
        gApplication->Terminate(0);
        break;
    }
}

void Menu::DrawInFrame(TGMainFrame *main_frame) {
    TGMenuBar*    menu_bar;
    TGPopupMenu*  popup_menu;
    menu_bar = new TGMenuBar(main_frame, 35, 50, kHorizontalFrame);
    popup_menu = new TGPopupMenu(gClient->GetRoot());
    popup_menu->AddEntry("Set User Certificate", M_USER_CERT, 0, gClient->GetPicture("bld_open.png"));
    popup_menu->AddEntry("Set User Public Key", M_USER_KEY, 0, gClient->GetPicture("bld_open.png"));
    popup_menu->AddEntry("Update Index", M_UPDATE_INDEX, 0, gClient->GetPicture("refresh.png"));
    popup_menu->AddEntry("Exit", M_FILE_EXIT, 0, gClient->GetPicture("bld_exit.png"));

    menu_bar->AddPopup("File Browser", popup_menu, new TGLayoutHints(kLHintsLeft, 0, 4, 0, 0));
    main_frame->AddFrame(menu_bar, new TGLayoutHints(kLHintsLeft ,2,2,2,2));
    popup_menu->Connect("Activated(Int_t)", "Menu", this, "HandleMenu(Int_t)");
}

void Menu::SetCertificatePath(string path){
    if (path != ""){
        cout << "Setting certificate path to " << Configuration::GetConfiguration().UpdateKey(Configuration::USERCERTPATH, path) << endl;
    }
}

void Menu::SetPublicKeyPath(string path){
    if (path != ""){
        cout << "Setting public key path to " << Configuration::GetConfiguration().UpdateKey(Configuration::USERPUBLICKEYPATH, path) << endl;
    }
}

void Menu::UpdateIndex(){
    if (Configuration::GetConfiguration().GetValue(Configuration::USERCERTPATH) == "" || 
        Configuration::GetConfiguration().GetValue(Configuration::USERPUBLICKEYPATH) == ""){
        cout << "Certificates not set: \n\t" << Configuration::GetConfiguration().GetValue(Configuration::USERCERTPATH) << "\n\t" 
                                             << Configuration::GetConfiguration().GetValue(Configuration::USERPUBLICKEYPATH) << endl;
    } else {
        string cmd = "python " + app_path + Configuration::GetConfiguration().GetValue(Configuration::UPDATEDATABASESCRIPTPATH) + " " +
                app_path + Configuration::GetConfiguration().GetValue(Configuration::DATABASECREATION) +    " " +
                app_path + Configuration::GetConfiguration().GetValue(Configuration::DATABASEPATH) + " " +
                Configuration::GetConfiguration().GetValue(Configuration::USERCERTPATH) + " " +
                Configuration::GetConfiguration().GetValue(Configuration::USERPUBLICKEYPATH);

        cout << "Running: " << cmd << endl;
        // int res = system(cmd.c_str());

        // cout << "Exit code: " << res << endl;
        // if (res == 0) {
            // cout << "Database updated successfully" << endl;
            // // Reload the list of files now...
        // }
    }
}