#include "FileViewer.h"

FileViewer::FileViewer() {
}

void FileViewer::OpenFileInTreeView(string remote_file_path) {
    TFile* remote_file = GetRemoteFile(remote_file_path);
    if (remote_file) {
        cout << "Remote file successfully open!" << endl;

        for (auto i: *(remote_file->GetListOfKeys())) {
            cout << i->GetName() << endl;
            TGListTreeItem* item = list_tree->AddItem(nullptr, i->GetName());
            item->SetPictures(popen, pclose);
            tree_items_map[item] = ((TKey*)i);
        }
    }
}

void FileViewer::DrawInFrame(TGMainFrame* main_frame){
    // canvas widget
    file_tree = new TGCanvas(main_frame,100,100);

    // canvas viewport
    view_port = file_tree->GetViewPort();
    list_tree = new TGListTree(file_tree,kHorizontalFrame);
    list_tree->Connect("DoubleClicked(TGListTreeItem*, Int_t)", "FileViewer", this, "TreeItemDoubleClicked(TGListTreeItem*, Int_t)");
    view_port->AddFrame(list_tree);
    list_tree->SetLayoutManager(new TGHorizontalLayout(list_tree));
    file_tree->SetContainer(list_tree);

    remove_item_button = new TGTextButton(main_frame, "Remove Selected Item (from view)");

    main_frame->AddFrame(file_tree, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    main_frame->AddFrame(remove_item_button, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));

    remove_item_button->Connect("Clicked()", "FileViewer", this, "DeleteSelectedItem()");
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

    Double_t w = 600;
    Double_t h = 600;
    TCanvas * c1 = new TCanvas("c", "c", w, h);
    c1->cd(1);
    ((TH1*)object)->Draw();

    AddChildren(item);
}
