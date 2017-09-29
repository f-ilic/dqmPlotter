// ==================== START FILETABLE  ====================

class FileTable {
public:
    FileTable() {}
    void FillFromFile(string path_to_file_to_load);
    string GetPathFromName(string name);
    string GetNameFromPath(string path);
    FileTable FilterNamesBy(vector<string> filters);
    void DisplayInListBox(TGListBox* listbox);
    void PrintDebug();
    void AddEntry(string k, string v);

private:
    map<string, string> table;          // key=name, value=path
};


string FileTable::GetPathFromName(string name) {
    return table[name];
}

void FileTable::FillFromFile(string path_to_file_to_load) {
    cout << "Trying to load " << endl;

    std::ifstream in(path_to_file_to_load);

    char str[255];

    string filenamepath, displayname;
    while(in) {
        in.getline(str, 255);  // delim defaults to '\n'
        filenamepath=string(str);

        int beginIdx = filenamepath.rfind('/');
        displayname = filenamepath.substr(beginIdx + 1);
        this->AddEntry(displayname, filenamepath);
    }
}

void FileTable::AddEntry(string k, string v) {
    table[k] = v;
}

void FileTable::PrintDebug() {
    for(auto& e : table)
        cout << "key: " << e.first << "  value: " << e.second << endl;
}

void FileTable::DisplayInListBox(TGListBox* listbox) {
    listbox->RemoveAll();
    int i = 0;
    for(auto& elem : table) {
        listbox->AddEntry(elem.first.c_str(), i++);
    }
    listbox->Layout();
}

FileTable FileTable::FilterNamesBy(vector<string> filters) {
    FileTable filtered;

    for(auto& entry : table) {
        bool is_ok = true;
        for(auto& f : filters){

            if(entry.first.find(f) != string::npos) {
                is_ok &= true;
            } else {
                is_ok &= false;
                break;
            }
        }
        if(is_ok)
            filtered.AddEntry(entry.first, entry.second);
    }
    return filtered;
}

// ==================== END FILETABLE  ====================





// ==================== START BROWSER  ====================

class Browser {

public:
    Browser(TGMainFrame*);
    void GetItems() { cout << "FileSearch::GetItems" << endl; }


    void ApplyFilter();        // @SLOT
    void SelectFiles();        // @SLOT
    void ClearSelectedFiles(); // @SLOT

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
};


Browser::Browser(TGMainFrame* main_frame) {

    table.FillFromFile("./rootfiles.txt");

    cout << "FileSearch::FileSearch" << endl;

    top_frame  = new TGVerticalFrame(main_frame);
    bot_frame  = new TGVerticalFrame(main_frame);

    search_frame = new TGHorizontalFrame(top_frame);
    datamode_dropdown = new TGComboBox(search_frame, 100);

    module_dropdown = new TGComboBox(search_frame, 100);
    search_box = new TGTextEntry(search_frame);
    applyfilter_button = new TGTextButton(search_frame, "Apply Filter");

    search_frame->AddFrame(datamode_dropdown);
    search_frame->AddFrame(module_dropdown);
    search_frame->AddFrame(search_box, new TGLayoutHints(kLHintsExpandX));
    search_frame->AddFrame(applyfilter_button);

    available_files_box = new TGListBox(top_frame);
    selectfiles_button = new TGTextButton(top_frame, "Add selected File(s)");

    selected_files_box = new TGListBox(bot_frame);
    clear_selected_button = new TGTextButton(bot_frame, "Clear Selection");

    top_frame->AddFrame(search_frame, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    top_frame->AddFrame(available_files_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    top_frame->AddFrame(selectfiles_button, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));

    bot_frame->AddFrame(selected_files_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    bot_frame->AddFrame(clear_selected_button, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    main_frame->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    main_frame->AddFrame(bot_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));

    datamode_dropdown->AddEntry("NOT IMPLEMENTED YET", 0);
    datamode_dropdown->AddEntry("Online", 1);
    datamode_dropdown->AddEntry("Offline", 2);
    datamode_dropdown->AddEntry("Relval", 3);

    module_dropdown->AddEntry("All", 0);
    module_dropdown->AddEntry("Ecal", 1);
    module_dropdown->AddEntry("Pixel", 2);
    module_dropdown->AddEntry("PixelPhase1", 3);
    module_dropdown->AddEntry("SiStrip", 4);

    datamode_dropdown->Resize(150, 20);
    module_dropdown->Resize(150, 20);

    datamode_dropdown->Select(0);
    module_dropdown->Select(0);

    available_files_box->SetMultipleSelections(true);

    applyfilter_button->SetToolTipText("Click here to print the selection you made");
    applyfilter_button->Connect("Clicked()", "Browser", this, "ApplyFilter()");

    selectfiles_button->Connect("Clicked()", "Browser", this, "SelectFiles()");
    clear_selected_button->Connect("Clicked()", "Browser", this, "ClearSelectedFiles()");

    table.DisplayInListBox(available_files_box);

    selection_in_box = new TList;
}

void Browser::ApplyFilter() {
    available_files_box->RemoveAll();
    string module_str = module_dropdown->GetSelectedEntry()->GetTitle();
    string query_str = search_box->GetText();

    vector<string> filters;
    filters.push_back("_" + module_str + "_");
    filters.push_back(query_str);

    FileTable result = table.FilterNamesBy(filters);
    result.DisplayInListBox(available_files_box);
}

void Browser::ClearSelectedFiles() {
    selected_files_box->RemoveAll();
}

void Browser::SelectFiles() {
    selection_in_box->Clear();
    available_files_box->GetSelectedEntries(selection_in_box);
    selection_in_box->ls();

    FileTable selected_files;

    for(const auto&& obj: *selection_in_box) {
        string obj_name = obj->GetTitle();
        string val = table.GetPathFromName(obj_name);
        cout << "found value " << val << endl;
        selected_files.AddEntry(obj_name, val);
    }

    selected_files.DisplayInListBox(selected_files_box);
    selected_files.PrintDebug();
}

// ==================== END BROWSER  ====================


void dqmPlotter() {
    int width = 600;
    int height = 1000;
    TGMainFrame* main_frame = new TGMainFrame(gClient->GetRoot(), width, height);

    main_frame->SetWindowName("Browser");
    Browser* fs = new Browser(main_frame);

    main_frame->MapSubwindows();
    main_frame->MapWindow();

    main_frame->MoveResize(100, 100, width, height);

}
