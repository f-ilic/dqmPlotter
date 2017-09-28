class TempClass {
public:
    TempClass(TGMainFrame*);

    void PushResult(string t) {
        file_box->AddEntry(t.c_str(), 0);
    }


private:
   TGVerticalFrame* top_frame;
   TGListBox*       file_box;

};

TempClass::TempClass(TGMainFrame* main_frame) {
    top_frame = new TGVerticalFrame(main_frame);
    file_box  = new TGListBox(top_frame);

    top_frame->AddFrame(file_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    main_frame->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
}


class FileSearch {

public:
    FileSearch(TGMainFrame*, TempClass *resclass);
    void GetItems() { cout << "FileSearch::GetItems" << endl; }


    void ApplyFilter(); // @SLOT
    void SelectFiles(); // @SLOT

private:

    void LoadFilenames(string path);
    void DisplayTable(vector<string>);

    TGVerticalFrame*   top_frame;
    TGHorizontalFrame* search_frame;
    TGTextEntry*       search_box;
    TGListBox*         result_box;
    TGComboBox*        datamode_dropdown;
    TGComboBox*        module_dropdown;
    TGTextButton*      applyfilter_button;
    TGTextButton*      selectfiles_button;
    TList*             selection_in_box;
    TempClass* tempclass;

    vector<string>     table; // this contains all the paths to the remote files
};


FileSearch::FileSearch(TGMainFrame* main_frame, TempClass* resclass) {
    cout << "FileSearch::FileSearch" << endl;

    tempclass = resclass;
    top_frame  = new TGVerticalFrame(main_frame);

    search_frame = new TGHorizontalFrame(top_frame);
    datamode_dropdown = new TGComboBox(search_frame, 100);

    module_dropdown = new TGComboBox(search_frame, 100);
    search_box = new TGTextEntry(search_frame);
    applyfilter_button = new TGTextButton(search_frame, "Apply Filter");

    search_frame->AddFrame(datamode_dropdown);
    search_frame->AddFrame(module_dropdown);
    search_frame->AddFrame(search_box, new TGLayoutHints(kLHintsExpandX));
    search_frame->AddFrame(applyfilter_button);

    result_box = new TGListBox(top_frame);
    selectfiles_button = new TGTextButton(top_frame, "Add selected File(s)");

    top_frame->AddFrame(search_frame, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    top_frame->AddFrame(result_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    top_frame->AddFrame(selectfiles_button, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    main_frame->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));

    datamode_dropdown->AddEntry("NOT IMPLEMENTED YET", 0);
    datamode_dropdown->AddEntry("Online", 1);
    datamode_dropdown->AddEntry("Offline", 2);
    datamode_dropdown->AddEntry("Relval", 3);

    module_dropdown->AddEntry(".", 0);
    module_dropdown->AddEntry("Ecal", 1);
    module_dropdown->AddEntry("Pixel", 2);
    module_dropdown->AddEntry("PixelPhase1", 2);
    module_dropdown->AddEntry("Strip", 3);

    datamode_dropdown->Resize(150, 20);
    module_dropdown->Resize(150, 20);

    datamode_dropdown->Select(0);
    module_dropdown->Select(0);

    result_box->SetMultipleSelections(true);

    applyfilter_button->SetToolTipText("Click here to print the selection you made");
    applyfilter_button->Connect("Clicked()", "FileSearch", this, "ApplyFilter()");

    selectfiles_button->Connect("Clicked()", "FileSearch", this, "SelectFiles()");

    LoadFilenames("./rootfiles.txt");
    DisplayTable(table);


    selection_in_box = new TList;

}

void FileSearch::ApplyFilter() {
    cout << "applying filter" << endl;

    result_box->RemoveAll();

    string module_str = module_dropdown->GetSelectedEntry()->GetTitle();
    string query_str = search_box->GetText();

    vector<string> query_result;

    for(auto& elem : this->table) {
        if ((elem.find("_" + module_str + "_") != string::npos) &&   // module filter
            (elem.find(query_str) != string::npos)) {                // textbox filter
            query_result.push_back(elem);
        }
    }

    DisplayTable(query_result);
}

void FileSearch::SelectFiles() {
    cout << "selecting files" << endl;

    selection_in_box->Clear();
    result_box->GetSelectedEntries(selection_in_box);
    selection_in_box->ls();

    tempclass->PushResult("something");
}

void FileSearch::LoadFilenames(string path) {
    cout << "FileSearch::LoadFilenames" << endl;
    std::ifstream in(path);

    char str[255];

    int i = 0;
    string tmp, displayname;
    while(in) {
        in.getline(str, 255);  // delim defaults to '\n'
        tmp=string(str);

        int beginIdx = tmp.rfind('/');
        displayname = tmp.substr(beginIdx + 1);
        table.push_back(displayname);
    }
}

void FileSearch::DisplayTable(vector<string> t) {
    result_box->RemoveAll();
    int i = 0;
    for(auto& elem : t) {
        result_box->AddEntry(elem.c_str(), i++);
    }
    result_box->Layout();
}




void dqmPlotter() {
    int width = 600;
    int height = 1000;
    TGMainFrame* main_frame = new TGMainFrame(gClient->GetRoot(), width, height);
    TGMainFrame* sub_frame = new TGMainFrame(gClient->GetRoot(), width, height/2);


    main_frame->SetWindowName("Search window");
    sub_frame->SetWindowName("Selected files");


    TempClass* tc = new TempClass(sub_frame);
    FileSearch* fs = new FileSearch(main_frame, tc);

    main_frame->MapSubwindows();
    main_frame->MapWindow();

    main_frame->MoveResize(100, 100, width, height);

    sub_frame->MapSubwindows();
    sub_frame->MapWindow();

    sub_frame->MoveResize(100, 100, width, height/2);
}
