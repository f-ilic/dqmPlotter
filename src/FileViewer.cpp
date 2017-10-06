#include "../include/FileViewer.h"
#include "../include/Configuration.h"
#include "../include/StatusBar.h"

FileViewer::FileViewer() {
}

void FileViewer::OpenFileInTreeView(const string& remote_file_path, const string& displayname) { 
    if (list_tree->FindChildByName(nullptr, displayname.c_str()))
    {        
        StatusBar::GetStatusBar().GetStatusBarControl()->SetText("This dataset has been already loaded. Skipping...", 0);
        return;
    }
  
    TFile* remote_file = GetRemoteFile(remote_file_path);
    if (remote_file && !IsFileOpen(displayname)) {
        for (auto i: *(remote_file->GetListOfKeys())) {
            TGListTreeItem* item = list_tree->AddItem(nullptr, displayname.c_str());
            item->SetPictures(popen, pclose);
            tree_items_map[item] = ((TKey*)i);
            open_files[displayname] = item;
        }
    }
}

bool FileViewer::IsFileOpen(const string& s) {
    return (open_files.find(s) != open_files.end());
}

void FileViewer::DrawInFrame(TGCompositeFrame* main_frame){
    TGVerticalFrame* top_frame      = new TGVerticalFrame(main_frame);
    TGHorizontalFrame* button_frame = new TGHorizontalFrame(top_frame);

    // canvas widget
    file_tree = new TGCanvas(top_frame,100,100);

    // canvas viewport
    view_port = file_tree->GetViewPort();
    list_tree = new TGListTree(file_tree, kHorizontalFrame);
    list_tree->Connect("DoubleClicked(TGListTreeItem*, Int_t)", "FileViewer", this, "TreeItemDoubleClicked(TGListTreeItem*, Int_t)");
    view_port->AddFrame(list_tree);
    list_tree->SetLayoutManager(new TGHorizontalLayout(list_tree));
    file_tree->SetContainer(list_tree);

    remove_item_button = new TGTextButton(button_frame, "Remove Selected");
    remove_all_button = new TGTextButton(button_frame, "Remove All");

    button_frame->AddFrame(remove_item_button, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    button_frame->AddFrame(remove_all_button, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));

    top_frame->AddFrame(file_tree, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    top_frame->AddFrame(button_frame, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));

    remove_item_button->Connect("Clicked()", "FileViewer", this, "RemoveSelectedItem()");
    remove_all_button->Connect("Clicked()", "FileViewer", this, "RemoveAll()");

    main_frame->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
}

void FileViewer::PrintSelectedItem() {
    TGListTreeItem* item = list_tree->GetSelected();
    cout << tree_items_map[item]->ReadObj()->GetName() << endl;
}

void FileViewer::RemoveSelectedItem() {
    TGListTreeItem* item = list_tree->GetSelected();

    if(!item)
        return;

    tree_items_map.erase(item);
    list_tree->DeleteItem(item);

    // if the item is a root dir of a file
    // remove the directory from open_files;
    for(auto& e : open_files) {
        if(e.second==item) {
            open_files.erase(e.first);
        }
    }

}

void FileViewer::RemoveAll() {
    for(auto& e : open_files) {
        list_tree->DeleteItem(e.second);
    }
    open_files.clear();
    tree_items_map.clear();
}

//TODO: change signature to (const string& filepath, const string& displayname)
//      so that you dont have to parse the filepath again.
TFile* FileViewer::GetRemoteFile(const string& filepath) {
    if(!DEVMODE) {
        gEnv->SetValue("Davix.GSI.UserCert", "/afs/cern.ch/user/p/pjurgiel/.globus/copy/usercert.pem");
        gEnv->SetValue("Davix.GSI.UserKey", "/afs/cern.ch/user/p/pjurgiel/.globus/copy/userkey_nopass.pem");
        gEnv->SetValue("Davix.Debug", 1);
    }
    
    string file_path_to_open = filepath;

    if (Configuration::GetConfiguration().GetValue(Configuration::LOCALCOPIES) == "ON") {
        TGStatusBar* statusBar = StatusBar::GetStatusBar().GetStatusBarControl();
        statusBar->SetText("Downloading file to disk. This can take a while...", 0);
      
        string local_copy_name = filepath.substr(filepath.rfind("/") + 1);
        string local_copy_path = Configuration::GetConfiguration().GetValue(Configuration::TMPDATADIRECTORY) + local_copy_name;
        
        TFile* remote_file = TFile::Open(filepath.c_str());
        remote_file->Cp(local_copy_path.c_str());
        remote_file->Close();
        
        file_path_to_open = local_copy_path;
        
        statusBar->SetText((string("File ") + filepath + " downloaded.").c_str(), 0);
    }
    
    return TFile::Open(file_path_to_open.c_str());
}

void FileViewer::AddChildren(TGListTreeItem* parent) {
    // add only if not loaded previously
    if (parent->GetFirstChild() == nullptr) {
        TKey* currentKey = tree_items_map[parent];

        if (currentKey->IsFolder()) {
            TDirectory* dir = (TDirectory*)(currentKey->ReadObj());
            TList* current_dir_keys = dir->GetListOfKeys();

            for (TObject* obj : *current_dir_keys) {
                TString name = obj->GetName();
                TGListTreeItem* new_item;

                if ((TKey*)obj->IsFolder()) {
                    new_item = list_tree->AddItem(parent, name, popen, pclose, kFALSE);
                } else {
                    TObject* o = ((TKey*)obj)->ReadObj();

                    if (dynamic_cast<TProfile2D*>(o)) {    // FIND THE PROPER NAME FOR ICON
                        new_item = list_tree->AddItem(parent, name, ph1, ph1);
                    } else if (dynamic_cast<TProfile*>(o)) {  // FIND THE PROPER NAME FOR ICON
                        new_item = list_tree->AddItem(parent, name, ph1, ph1);
                    } else if (dynamic_cast<TH2*>(o)) {
                        new_item = list_tree->AddItem(parent, name, ph2, ph2);
                    } else{ //let's assume everythng else is th1
                        new_item = list_tree->AddItem(parent, name, ph1, ph1);
                    }
                }
                new_item->CheckItem(kFALSE);
                tree_items_map[new_item] = (TKey*)obj;
            }
        }
    }
}


void FileViewer::TreeItemDoubleClicked(TGListTreeItem* item, int id) {
    TObject* object = tree_items_map[item]->ReadObj();
    AddChildren(item);

    //FIXME: this needs to emit signals based on object
    // with the most general being ItemDoubleClicked
    // but for exampole:
    // -FolderDoubleClicked
    // -FileDoubleClicked (maybe even th1/2/profile doubleclicked)
    if(dynamic_cast<TH1*>(object))
        Emit("ItemDoubleClicked(TH1*)", (TH1*)object);
}

void FileViewer::DisplayInTreeView(map<string*, string*> *t) {
    for(auto& e : *t) {
        cout << *(e.first) << endl;
        cout << *(e.second) << endl;
        OpenFileInTreeView(*(e.first), *(e.second));
        delete (e.first);
        delete (e.second);
    }
    delete t;
}
