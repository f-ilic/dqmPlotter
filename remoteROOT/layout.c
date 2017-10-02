#include "layout.h"
#include "Riostream.h"
#include <map>
#include <vector>


class RemoteFileBrowser {
public:
    RemoteFileBrowser(const TString& theRemoteMapPath);
    ~RemoteFileBrowser();
    void TreeItemDblClicked(TGListTreeItem* item, Int_t id);
    void ClearBtnClick();
    void GetBtnClick();
    
private:
    TFile* GetRemoteFile(const TString& filepath);
    void AddChildren(TGListTreeItem* parent);

    TGMainFrame *m_mainFrame;
    TGCanvas *m_fileTree;
    TGViewPort *m_viewPort;
    TGListTree *m_listTree;
    TGTextButton *m_clearBtn;
    TGTextButton *m_getBtn;

    const TGPicture *popen;       //used for list tree items
    const TGPicture *pclose;      //used for list tree items

    const TGPicture *ph1;
    const TGPicture *ph2;
    const TGPicture *pp;          // FIND THE PROPER NAME FOR ICON
    const TGPicture *pp2;         // FIND THE PROPER NAME FOR ICON

    map<TGListTreeItem*, TKey*> m_treeItemMap;
    TGListTreeItem* m_root;

    TFile* remoteFile;

    Int_t m_width{400};
    Int_t m_height{600};

    TString m_path;
};

void layout() {
    RemoteFileBrowser* browser = new RemoteFileBrowser("https://cmsweb.cern.ch/dqm/online/data/browse/Original/00030xxxx/0003038xx/DQM_V0001_PixelPhase1_R000303823.root");
}  


RemoteFileBrowser::RemoteFileBrowser(const TString &theRemoteMapPath) {
    // main frame
    m_mainFrame = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
    m_mainFrame->SetName("m_mainFrame");
    m_mainFrame->SetLayoutBroken(kTRUE);

    ULong_t ucolor;        // will reflect user color changes
    gClient->GetColorByName("#ffffff",ucolor);

    // canvas widget
    m_fileTree = new TGCanvas(m_mainFrame,100,100,kSunkenFrame,ucolor);
    m_fileTree->SetName("m_fileTree");

    // canvas viewport
    m_viewPort = m_fileTree->GetViewPort();

    m_clearBtn = new TGTextButton(m_mainFrame,"CLEAR",-1,TGTextButton::GetDefaultGC()(),TGTextButton::GetDefaultFontStruct(),kRaisedFrame);
    m_clearBtn->SetTextJustify(36);
    m_clearBtn->SetMargins(0,0,0,0);
    m_clearBtn->SetWrapLength(-1);
    m_mainFrame->AddFrame(m_clearBtn, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    m_clearBtn->MoveResize(32,m_height - 50,88,24);
    m_clearBtn->Connect("Clicked()", "RemoteFileBrowser", this, "ClearBtnClick()");

    m_getBtn = new TGTextButton(m_mainFrame,"GET",-1,TGTextButton::GetDefaultGC()(),TGTextButton::GetDefaultFontStruct(),kRaisedFrame);
    m_getBtn->SetTextJustify(36);
    m_getBtn->SetMargins(0,0,0,0);
    m_getBtn->SetWrapLength(-1);
    m_mainFrame->AddFrame(m_getBtn, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    m_getBtn->MoveResize(132,m_height - 50,88,24);
    m_getBtn->Connect("Clicked()", "RemoteFileBrowser", this, "GetBtnClick()");

    m_mainFrame->SetMWMHints(kMWMDecorAll,
                             kMWMFuncAll,
                             kMWMInputModeless);
    m_mainFrame->MapSubwindows();

    m_mainFrame->Resize(m_mainFrame->GetDefaultSize());
    m_mainFrame->MapWindow();
    m_mainFrame->Resize(m_width,m_height);

    ////////////////////////////////////////////////
    m_path = theRemoteMapPath;
    remoteFile = GetRemoteFile(m_path);
    if (remoteFile) {
        cout << "Remote file successfully open!" << endl;

        // list tree
        m_listTree = new TGListTree(m_fileTree,kHorizontalFrame);
        m_listTree->SetName("m_listTree");

        m_listTree->Connect("DoubleClicked(TGListTreeItem*, Int_t)", "RemoteFileBrowser", this, "TreeItemDblClicked(TGListTreeItem*, Int_t)");

        popen = gClient->GetPicture("ofolder_t.xpm");
        pclose = gClient->GetPicture("folder_t.xpm");
        ph1 = gClient->GetPicture("h1_t.xpm");
        ph2 = gClient->GetPicture("h2_t.xpm");

        // TString rootName = *(remoteFile->GetListOfKeys())[0].GetName();
        for (auto i: *(remoteFile->GetListOfKeys())) {
            cout << i->GetName() << endl;

            TGListTreeItem* item = m_listTree->AddItem(nullptr, i->GetName());
            item->SetPictures(popen, pclose);

            m_treeItemMap[item] = ((TKey*)i);

            m_root = item;
        }

        m_viewPort->AddFrame(m_listTree);
        m_listTree->SetLayoutManager(new TGHorizontalLayout(m_listTree));
        m_listTree->MapSubwindows();
        m_fileTree->SetContainer(m_listTree);
        m_fileTree->MapSubwindows();
        m_mainFrame->AddFrame(m_fileTree, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
        m_fileTree->MoveResize(0,0,m_width,m_height - 100);
    }
}

RemoteFileBrowser::~RemoteFileBrowser() {
    if (remoteFile) remoteFile->Close();
}

void RemoteFileBrowser::TreeItemDblClicked(TGListTreeItem *item, Int_t id) {
    cout << "DblClicked: " << id << endl;
    AddChildren(item);
}

void RemoteFileBrowser::ClearBtnClick() {
    cout << "Clear..." << endl;
    m_listTree->CheckAllChildren(m_root, kFALSE);
    m_listTree->ClearHighlighted();
}

void RemoteFileBrowser::GetBtnClick() {
    cout << "GET..." << endl;
    TList* listOfCheckedItems = new TList();

    for (auto pair: m_treeItemMap) {
        if(pair.first->IsChecked() == 1 && pair.first->GetParent() != nullptr)
            listOfCheckedItems->Add((TObject*)pair.first);
        cout << "N" << endl;

        cout << pair.first->IsChecked() << "\t" << pair.second->ReadObj()->GetName() << endl;
    }
}

TFile *RemoteFileBrowser::GetRemoteFile(const TString &filepath) {
    gEnv->SetValue("Davix.GSI.UserCert", "/home/fil/Documents/usercert.pem");
    gEnv->SetValue("Davix.GSI.UserKey", "/home/fil/Documents/userkey.pem");
    gEnv->SetValue("Davix.Debug", 1);
    return TFile::Open(filepath);
}

void RemoteFileBrowser::AddChildren(TGListTreeItem *parent) {
    // add only if not loaded previously
    if (parent->GetFirstChild() == nullptr) {
        TKey* currentKey = m_treeItemMap[parent];

        if (currentKey->IsFolder()) {
            cout << "This is a folder!" << endl;
            TDirectory* dir = (TDirectory*)(currentKey->ReadObj());

            TList* currentDirKeys = dir->GetListOfKeys();

            for (TObject* obj : *currentDirKeys) {
                TString name = obj->GetName();

                cout << "Adding: " << name << endl;
                TGListTreeItem* newItem;

                if ((TKey*)obj->IsFolder()) {
                    newItem = m_listTree->AddItem(parent, name, popen, pclose, kFALSE);
                } else {
                    TObject* o = ((TKey*)obj)->ReadObj();

                    if (dynamic_cast<TProfile2D*>(o)) {    // FIND THE PROPER NAME FOR ICON
                        cout << "Adding TProfile2D" << endl;
                        newItem = m_listTree->AddItem(parent, name, ph1, ph1);
                    } else if (dynamic_cast<TProfile*>(o)) {  // FIND THE PROPER NAME FOR ICON
                        cout << "Adding TProfile" << endl;
                        newItem = m_listTree->AddItem(parent, name, ph1, ph1);
                    } else if (dynamic_cast<TH2*>(o)) {
                        cout << "Adding TH2" << endl;
                        newItem = m_listTree->AddItem(parent, name, ph2, ph2);
                    } else{ //let's assume everythng else is th1
                        cout << "Adding TH1" << endl;
                        newItem = m_listTree->AddItem(parent, name, ph1, ph1);
                    }
                }
                newItem->CheckItem(kFALSE);
                m_treeItemMap[newItem] = (TKey*)obj;
            }
        }
    }
}
