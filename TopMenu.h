#ifndef TOPMENU__H_
#define TOPMENU__H_

#include "Configuration.cpp"

class Menu {
public:
    Menu(){
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
};

#endif