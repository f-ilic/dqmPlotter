#include "Browser.h"

Browser::Browser() {
    table.FillFromFile("./rootfiles.txt");
    table.GetUniqueModulesFromFile("./rootfiles.txt");
}

void Browser::DrawInFrame(TGCompositeFrame *mf) {
    top_frame  = new TGVerticalFrame(mf);
    search_frame = new TGHorizontalFrame(top_frame);

    module_dropdown = new TGComboBox(search_frame, 100);
    search_box = new TGTextEntry(search_frame);
    applyfilter_button = new TGTextButton(search_frame, "Apply Filter");

    search_frame->AddFrame(module_dropdown);
    search_frame->AddFrame(search_box, new TGLayoutHints(kLHintsExpandX));
    search_frame->AddFrame(applyfilter_button);

    available_files_box = new TGListBox(top_frame);
    selectfiles_button = new TGTextButton(top_frame, "Choose selected file(s)");

    top_frame->AddFrame(search_frame, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    top_frame->AddFrame(available_files_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    top_frame->AddFrame(selectfiles_button, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));

    set<string> modules = table.GetUniqueModulesFromFile("./rootfiles.txt");

    int j=1;
    for(auto& e : modules) {
        module_dropdown->AddEntry(e.c_str(), j++);
    }

    module_dropdown->Resize(200, 20);
    module_dropdown->Select(0);

    available_files_box->SetMultipleSelections(true);
    applyfilter_button->Connect("Clicked()", "Browser", this, "ApplyFilter()");

    selectfiles_button->Connect("Clicked()", "Browser", this, "SelectFiles()");
    table.DisplayInListBox(available_files_box);

    selection_in_box = new TList;
    mf->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    file_view.DrawInFrame(mf);
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


void Browser::SelectFiles() {
    selection_in_box->Clear();
    available_files_box->GetSelectedEntries(selection_in_box);
    selection_in_box->ls();

    FileTable selected_files;

    for(const auto&& obj: *selection_in_box) {
        string obj_name = obj->GetTitle();
        string val = table.GetPathFromName(obj_name);
        cout << "found value " << val << endl;
        selected_files.AddEntry(obj_name, "./f1.root"); // val goes here if in real use
    }
//    selected_files.PrintDebug();
    selected_files.DisplayInTreeView(file_view);
}
