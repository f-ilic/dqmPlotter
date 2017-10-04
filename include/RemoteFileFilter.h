#ifndef REMOTEFILEFILTER__H_
#define REMOTEFILEFILTER__H_

#include "TGTextEntry.h"
#include "TGComboBox.h"
#include "RQ_OBJECT.h"


#include "../include/FileTable.h"
#include <iostream>
#include <map>

class RemoteFileFilter {
    RQ_OBJECT("RemoteFileFilter")

public:
    RemoteFileFilter();
    void DrawInFrame(TGCompositeFrame* mf);
    void ApplyFilter();

    void FillFromFile(string path_to_file_to_load);
    set<string> GetUniqueModulesFromFile(string filepath);

//signal: INTERFACE
    void FilesSelected(map<string*, string*>*); //*SIGNAL*

//signal: INTERNAL
    void SelectFiles(); //*SIGNAL*

private:
    void DisplayInListBox(FileTable ftable);

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
};
#endif
