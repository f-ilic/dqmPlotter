#include "../include/FileViewer.h"

FileViewer::FileViewer() {
}

void FileViewer::OpenFileInTreeView(string remote_file_path, string displayname) {
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

bool FileViewer::IsFileOpen(string s) {
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

TFile* FileViewer::GetRemoteFile(string filepath) {
#define ROOTDavixIsAPieceOfShit true
    if(!ROOTDavixIsAPieceOfShit) {
        gEnv->SetValue("Davix.GSI.UserCert", "/afs/cern.ch/user/p/pjurgiel/.globus/copy/usercert.pem");
        gEnv->SetValue("Davix.GSI.UserKey", "/afs/cern.ch/user/p/pjurgiel/.globus/copy/userkey_nopass.pem");
        gEnv->SetValue("Davix.Debug", 1);
    }
    return TFile::Open(filepath.c_str());
}

void FileViewer::AddChildren(TGListTreeItem* parent) {
    // add only if not loaded previously
    if (parent->GetFirstChild() == nullptr) {
        TKey* currentKey = tree_items_map[parent];

        if (currentKey->IsFolder()) {
            cout << "This is a folder!" << endl;
            TDirectory* dir = (TDirectory*)(currentKey->ReadObj());

            TList* currentDirKeys = dir->GetListOfKeys();

            for (TObject* obj : *currentDirKeys) {
                TString name = obj->GetName();

                cout << "Adding: " << name << endl;
                TGListTreeItem* newItem;

                if ((TKey*)obj->IsFolder()) {
                    newItem = list_tree->AddItem(parent, name, popen, pclose, kFALSE);
                } else {
                    TObject* o = ((TKey*)obj)->ReadObj();

                    if (dynamic_cast<TProfile2D*>(o)) {    // FIND THE PROPER NAME FOR ICON
                        cout << "Adding TProfile2D" << endl;
                        newItem = list_tree->AddItem(parent, name, ph1, ph1);
                    } else if (dynamic_cast<TProfile*>(o)) {  // FIND THE PROPER NAME FOR ICON
                        cout << "Adding TProfile" << endl;
                        newItem = list_tree->AddItem(parent, name, ph1, ph1);
                    } else if (dynamic_cast<TH2*>(o)) {
                        cout << "Adding TH2" << endl;
                        newItem = list_tree->AddItem(parent, name, ph2, ph2);
                    } else{ //let's assume everythng else is th1
                        cout << "Adding TH1" << endl;
                        newItem = list_tree->AddItem(parent, name, ph1, ph1);
                    }
                }
                newItem->CheckItem(kFALSE);
                tree_items_map[newItem] = (TKey*)obj;
            }
        }
    }
}


void FileViewer::TreeItemDoubleClicked(TGListTreeItem* item, int id) {
    cout << "FileViewer::TreeItemDoubleClicked: " << id << endl;
    TObject* object = tree_items_map[item]->ReadObj();

    Double_t w = 300;
    Double_t h = 400;
    TCanvas * c1 = new TCanvas("c", "c", w, h);
    c1->cd(1);
    ((TH1*)object)->Draw();

    AddChildren(item);
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
