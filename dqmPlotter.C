#include "Browser.cpp"


// ==================== START MENU ====================

enum EMyMessageTypes {
   M_USER_CERT,
   M_USER_KEY,
   M_UPDATE_INDEX,
   M_FILE_EXIT
};

class Menu {
public:
    Menu(TGMainFrame* main_frame){
      
        app_path = gSystem->pwd();
        app_path += "/";
      
        mf = main_frame;
        TGMenuBar*    menu_bar;
        TGPopupMenu*  popup_menu;
        menu_bar = new TGMenuBar(main_frame, 35, 50, kHorizontalFrame);
        popup_menu = new TGPopupMenu(gClient->GetRoot());
        popup_menu->AddEntry("Set User Certificate", M_USER_CERT, 0, gClient->GetPicture("bld_open.png"));
        popup_menu->AddEntry("Set User Public Key", M_USER_KEY, 0, gClient->GetPicture("bld_open.png"));
        
        popup_menu->AddEntry("Update Index", M_UPDATE_INDEX, 0, gClient->GetPicture("refresh.png"));
        
        
//        popup_menu->AddEntry("Exit", M_FILE_EXIT, 0, gClient->GetPicture("bld_exit.png"));
        menu_bar->AddPopup("File Browser", popup_menu, new TGLayoutHints(kLHintsLeft, 0, 4, 0, 0));
        main_frame->AddFrame(menu_bar, new TGLayoutHints(kLHintsLeft ,2,2,2,2));
        popup_menu->Connect("Activated(Int_t)", "Menu", this, "HandleMenu(Int_t)");
    }

    void HandleMenu(Int_t menu_id);
    TGMainFrame* mf;
    
    TString updatescript_path{"remoteROOT/readRemoteDirectories.py"};
    TString recreation_str{"remoteROOT/toUpdate.txt"};
    TString db_path{"remoteROOT/rootFiles.db"};
    TString certificate_path{""};
    TString publickey_path{""};
    TString app_path{""};

    void SetCertificatePath(TString path){
        if (path != "")
        {
            certificate_path = path;
            cout << "Setting certificate path to " << certificate_path << endl;
        }
    }
    
    void SetPublicKeyPath(TString path){
        if (path != "")
        {
            publickey_path = path;
            cout << "Setting public key path to " << publickey_path << endl;
        }
    }
    
    void UpdateIndex(){
        if (certificate_path == "" || publickey_path == ""){
            cout << "Certificates not set: \n\t" << certificate_path << "\n\t" << publickey_path << endl;
        }
        else{
            TString cmd = "python " + app_path + updatescript_path + " " + 
                                      app_path + recreation_str +    " " + 
                                      app_path + db_path +           " " + 
                                      certificate_path +             "x " + 
                                      publickey_path; 
            
            cout << "Running: " << cmd << endl;
            int res = system(cmd);
            
            cout << "Exit code: " << res << endl;
            if (res == 0)
            {
                cout << "Database updates successfully" << endl;
                // Reload the list of files now...
            }
        }
    } 

    TString GetCertificatePath() {
        return certificate_path;
    }
    
    TString GetPublicKeyPath() {
        return publickey_path;
    }
};

void Menu::HandleMenu(Int_t menu_id) {
    switch (menu_id) {
        case M_USER_CERT:
        case M_USER_KEY:{
            cout << "[ OK ] Open File Dialog" << endl;
            TGFileInfo file_info_;
            const char *filetypes[] = {"Certificate", "*.pem", 0, 0};
            file_info_.fFileTypes = filetypes;
            TGFileDialog* loadDialog = new TGFileDialog(gClient->GetDefaultRoot(), mf, kFDOpen, &file_info_);
            (menu_id == M_USER_CERT) ? this->SetCertificatePath(file_info_.fFilename) : this->SetPublicKeyPath(file_info_.fFilename);
            break;
        
        }
        case M_UPDATE_INDEX:
            this->UpdateIndex();
            break;
        
        case M_FILE_EXIT: 
            gApplication->Terminate(0);
            break;
        
    }
}



// ==================== END MENU ====================

#include <iostream>
void dqmPlotter() {
//    system("./test.sh"); // this is how to call external script
    int width = 600;
    int height = 1000;
    TGMainFrame* main_frame = new TGMainFrame(gClient->GetRoot(), width, height);

    main_frame->SetWindowName("Browser");
    Menu*  menu = new Menu(main_frame);
    Browser* fs = new Browser(main_frame);

    main_frame->MapSubwindows();
    main_frame->MapWindow();

    main_frame->MoveResize(100, 100, width, height);
    
    system("ls");
}


