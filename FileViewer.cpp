#include "FileViewer.h"

FileViewer::FileViewer(TGMainFrame* main_frame) {
    // canvas widget
    file_tree = new TGCanvas(main_frame,100,100);
    file_tree->SetName("m_fileTree");

    // canvas viewport
    view_port = file_tree->GetViewPort();

    load_file_button = new TGTextButton(main_frame,"Load File From Filesystem");
    load_file_button->Connect("Clicked()", "FileViewer", this, "StartBtnClicked()");


    list_tree = new TGListTree(file_tree,kHorizontalFrame);
    list_tree->Connect("DoubleClicked(TGListTreeItem*, Int_t)", "FileViewer", this, "TreeItemDoubleClicked(TGListTreeItem*, Int_t)");

    view_port->AddFrame(list_tree);
    list_tree->SetLayoutManager(new TGHorizontalLayout(list_tree));

    file_tree->SetContainer(list_tree);

    main_frame->AddFrame(file_tree, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    main_frame->AddFrame(load_file_button ,new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
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

TFile* FileViewer::GetRemoteFile(string filepath) {
#define ROOTDavixIsAPieceOfShit true
    if(!ROOTDavixIsAPieceOfShit) {
        gEnv->SetValue("Davix.GSI.UserCert", "/home/fil/Documents/usercert.pem");
        gEnv->SetValue("Davix.GSI.UserKey", "/home/fil/Documents/userkey.pem");
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

//    Double_t w = 600;
//    Double_t h = 600;
//    TCanvas * c1 = new TCanvas("c", "c", w, h);
//    c1->cd(1);
//    ((TH1*)object)->Draw();

    AddChildren(item);
}

void FileViewer::StartBtnClicked() {
    cout << "FileViewer::StartBtnClicked()" << endl;
    TGFileInfo file_info_;
    const char *filetypes[] = {"ROOT files", "*.root", 0, 0};
    file_info_.fFileTypes = filetypes;
    TGFileDialog* loadDialog = new TGFileDialog(gClient->GetDefaultRoot(), 0, kFDOpen, &file_info_);
    OpenFileInTreeView(file_info_.fFilename);
}
