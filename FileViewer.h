#include "Riostream.h"
#include <map>
#include <vector>

class FileViewer {
public:
    FileViewer();
    void TreeItemDoubleClicked(TGListTreeItem* item, int id);
//    void StartBtnClicked();
    void OpenFileInTreeView(string remote_file_path);

    void DrawInFrame(TGMainFrame* main_frame);

private:
    TFile* GetRemoteFile(string filepath);
    void AddChildren(TGListTreeItem* parent);

    TGCanvas*       file_tree;
    TGViewPort*     view_port;
    TGListTree*     list_tree;
//    TGTextButton*   load_file_button;


    const TGPicture* popen = gClient->GetPicture("ofolder_t.xpm");
    const TGPicture* pclose = gClient->GetPicture("folder_t.xpm");
    const TGPicture* ph1 = gClient->GetPicture("h1_t.xpm");
    const TGPicture* ph2 = gClient->GetPicture("h2_t.xpm");

    map<TGListTreeItem*, TKey*> tree_items_map;
};
