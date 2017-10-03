class FileViewer {
public:
    FileViewer();
    void TreeItemDoubleClicked(TGListTreeItem* item, int id);
    void OpenFileInTreeView(string remote_file_path, string displayname="");
    void DrawInFrame(TGCompositeFrame* main_frame);

    void PrintSelectedItem();
    void RemoveSelectedItem();
    void RemoveAll();
    bool IsOpened(string s);


private:
    TFile* GetRemoteFile(string filepath);
    void AddChildren(TGListTreeItem* parent);

    TGCanvas*       file_tree;
    TGViewPort*     view_port;
    TGListTree*     list_tree;
    TGTextButton*   remove_item_button;
    TGTextButton*   remove_all_button;

    map<TGListTreeItem*, TKey*>  tree_items_map;
    map<string, TGListTreeItem*> open_files;

    const TGPicture* popen  = gClient->GetPicture("ofolder_t.xpm");
    const TGPicture* pclose = gClient->GetPicture("folder_t.xpm");
    const TGPicture* ph1    = gClient->GetPicture("h1_t.xpm");
    const TGPicture* ph2    = gClient->GetPicture("h2_t.xpm");
};
