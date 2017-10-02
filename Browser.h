#include "FileTable.cpp"
#include "FileViewer.cpp"

class Browser {

public:
    Browser();
    void GetItems() { cout << "FileSearch::GetItems" << endl; }


    void ApplyFilter();        // @SLOT
    void SelectFiles();        // @SLOT
    void ClearSelectedFiles(); // @SLOT
    void SetCertificatePath(string certpath);

    void DrawInFrame(TGMainFrame* mf);


private:

    TGVerticalFrame*   top_frame;
    TGVerticalFrame*   bot_frame;
    TGHorizontalFrame* search_frame;
    TGTextEntry*       search_box;
    TGListBox*         available_files_box;
    TGComboBox*        datamode_dropdown;
    TGComboBox*        module_dropdown;
    TGTextButton*      applyfilter_button;
    TGTextButton*      selectfiles_button;
    TList*             selection_in_box;

    TGListBox*         selected_files_box;
    TGTextButton*      clear_selected_button;

    FileTable          table;
    string             certificate_path;

    FileViewer         file_view;
};
