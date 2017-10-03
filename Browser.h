#include "FileTable.cpp"

class Browser {

public:
    Browser();
    void ApplyFilter();        // @SLOT
    void SelectFiles();        // @SLOT
    void SetCertificatePath(string certpath);
    void DrawInFrame(TGCompositeFrame* mf);

private:
    TGVerticalFrame*   top_frame;
    TGHorizontalFrame* search_frame;
    TGTextEntry*       search_box;
    TGListBox*         available_files_box;
    TGComboBox*        datamode_dropdown;
    TGComboBox*        module_dropdown;
    TGTextButton*      applyfilter_button;
    TGTextButton*      selectfiles_button;
    TList*             selection_in_box;

    TGListBox*         selected_files_box;

    FileTable          table;
    string             certificate_path;

    FileViewer         file_view;
};
