class FileViewer {
public:
    FileViewer();
    void TreeItemDoubleClicked(TGListTreeItem* item, int id);
    void OpenFileInTreeView(string remote_file_path);
    void DrawInFrame(TGMainFrame* main_frame);

// TODO: WIP
    void PrintSelectedItem() {
        TGListTreeItem* item = list_tree->GetSelected();
        cout << tree_items_map[item]->ReadObj()->GetName() << endl;
    }
// TODO: WIP
    void DeleteSelectedItem() {
        TGListTreeItem* item = list_tree->GetSelected();
        tree_items_map.erase(item);
        list_tree->DeleteItem(item);
    }

private:
    TFile* GetRemoteFile(string filepath);
    void AddChildren(TGListTreeItem* parent);

    TGCanvas*       file_tree;
    TGViewPort*     view_port;
    TGListTree*     list_tree;
    TGTextButton*   remove_item_button;


    const TGPicture* popen = gClient->GetPicture("ofolder_t.xpm");
    const TGPicture* pclose = gClient->GetPicture("folder_t.xpm");
    const TGPicture* ph1 = gClient->GetPicture("h1_t.xpm");
    const TGPicture* ph2 = gClient->GetPicture("h2_t.xpm");

    map<TGListTreeItem*, TKey*> tree_items_map;
};
