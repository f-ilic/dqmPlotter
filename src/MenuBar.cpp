#include "../include/MenuBar.h"
#include "../include/StatusBar.h"

string MenuBar::OpenDialog(Int_t menu_id) const {
    TGFileInfo file_info_;
    const char *filetypes[] = {"Certificate", "*.pem", 0, 0};
    file_info_.fFileTypes = filetypes;
    TGFileDialog* loadDialog = new TGFileDialog(gClient->GetDefaultRoot(), nullptr, kFDOpen, &file_info_);
    
    return file_info_.fFilename;
}

void MenuBar::HandleMenu(Int_t menu_id) {
    switch (menu_id) {
    case M_USER_CERT: {
        SetCertificatePath(OpenDialog(menu_id));
        break;
    }
    case M_USER_KEY:{
        SetPublicKeyPath(OpenDialog(menu_id));
        break;
    }
    case M_UPDATE_INDEX:{
        this->UpdateIndex();
        break;
    }
    case M_WORK_WITH_LOCAL_COPIES:{
        this->TogglePopupEntry(M_WORK_WITH_LOCAL_COPIES);
        break;
    }
    case M_FILE_EXIT:{
        string cmd = "rm -rf " + Configuration::Instance().GetValue(Configuration::TMPDATADIRECTORY) + "*";
        if(system(cmd.c_str())) {
            cout << "system(cmd) returned nullptr" << endl;
            cout << " -- maybe bad" << endl;
        }
        
        gApplication->Terminate(0);
        break;
    }
    }
}

void MenuBar::DrawInFrame(TGMainFrame *main_frame) {
    menu_bar = new TGMenuBar(main_frame, 35, 50, kHorizontalFrame);
    popup_menu = new TGPopupMenu(gClient->GetRoot());
    popup_menu->AddEntry("Set User Certificate", M_USER_CERT, 0, gClient->GetPicture("bld_open.png"));
    popup_menu->AddEntry("Set User Key", M_USER_KEY, 0, gClient->GetPicture("bld_open.png"));
    popup_menu->AddEntry("Update Index", M_UPDATE_INDEX, 0, gClient->GetPicture("refresh.png"));
    
    popup_menu->AddEntry("Work with local copies", M_WORK_WITH_LOCAL_COPIES, 0, nullptr);
    
    popup_menu->AddEntry("Exit", M_FILE_EXIT, 0, gClient->GetPicture("bld_exit.png"));
    
    if (Configuration::Instance().GetValue(Configuration::LOCALCOPIES) == "ON"){
        popup_menu->CheckEntry(M_WORK_WITH_LOCAL_COPIES);
    }
    
    popup_menu->AddSeparator(popup_menu->GetEntry(M_WORK_WITH_LOCAL_COPIES));
    popup_menu->AddSeparator(popup_menu->GetEntry(M_FILE_EXIT));

    menu_bar->AddPopup("File Browser", popup_menu, new TGLayoutHints(kLHintsLeft, 0, 4, 0, 0));
    main_frame->AddFrame(menu_bar, new TGLayoutHints(kLHintsLeft ,2,2,2,2));
    
    popup_menu->Connect("Activated(Int_t)", "MenuBar", this, "HandleMenu(Int_t)");
}

void MenuBar::SetCertificatePath(const string& path) const{
    if (path != ""){
//        StatusBar::GetStatusBar().GetStatusBarControl()->SetText((string("Setting certificate path to: ") + path).c_str(), 0);
    }
}

void MenuBar::SetPublicKeyPath(const string& path) const{
    if (path != ""){       
//        StatusBar::GetStatusBar().GetStatusBarControl()->SetText((string("Setting public key path to: ") + path).c_str(), 0);
    }
}

void MenuBar::UpdateIndex(){
    if (Configuration::Instance().GetValue(Configuration::USERCERTPATH) == "" ||
        Configuration::Instance().GetValue(Configuration::USERPUBLICKEYPATH) == ""){
        cout << "Certificates not set: \n\t" << Configuration::Instance().GetValue(Configuration::USERCERTPATH) << "\n\t"
                                             << Configuration::Instance().GetValue(Configuration::USERPUBLICKEYPATH) << endl;
                                             
//        StatusBar::GetStatusBar().GetStatusBarControl()->SetText("Certificates not set", 0);

    } else {
        string cmd = "python " + app_path + Configuration::Instance().GetValue(Configuration::UPDATEDATABASESCRIPTPATH) + " " +
                app_path + Configuration::Instance().GetValue(Configuration::DATABASECREATION) +    " " +
                app_path + Configuration::Instance().GetValue(Configuration::DATABASEPATH) + " " +
                Configuration::Instance().GetValue(Configuration::USERCERTPATH) + " " +
                Configuration::Instance().GetValue(Configuration::USERPUBLICKEYPATH);

        cout << "Running: " << cmd << endl;
//        StatusBar::GetStatusBar().GetStatusBarControl()->SetText("Running database update. Please wait...", 0);

        int res = system(cmd.c_str());

        cout << "Exit code: " << res << endl;
        if (res == 0) {
//            StatusBar::GetStatusBar().GetStatusBarControl()->SetText("Database updated successfully.", 0);
            
            Emit("IndexUpdated()");
        } else {
//            StatusBar::GetStatusBar().GetStatusBarControl()->SetText("Error. Database update not successful.", 0);
        }
    }
}

void MenuBar::TogglePopupEntry(Int_t menu_id){
    if (this->popup_menu->IsEntryChecked(menu_id)) {
        this->popup_menu->UnCheckEntry(menu_id);
        if (menu_id == M_WORK_WITH_LOCAL_COPIES) {
            Configuration::Instance().UpdateKey(Configuration::LOCALCOPIES, "OFF");
        }
    } else {
        this->popup_menu->CheckEntry(menu_id);
        if (menu_id == M_WORK_WITH_LOCAL_COPIES) {
            Configuration::Instance().UpdateKey(Configuration::LOCALCOPIES, "ON");
        }
    }
}
