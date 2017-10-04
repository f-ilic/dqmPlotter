#include "../include/RemoteFileFilter.h"
#include <fstream>
#include <iostream>

RemoteFileFilter::RemoteFileFilter() {
    FillFromFile(Configuration::GetConfiguration().GetValue(Configuration::DATABASEPATH));
}

void RemoteFileFilter::DrawInFrame(TGCompositeFrame* mf) {
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

    FillModuleFilters(Configuration::GetConfiguration().GetValue(Configuration::DATABASEFILTERSPATH));

    module_dropdown->Resize(200, 20);
    module_dropdown->Select(0);

    available_files_box->SetMultipleSelections(true);
    applyfilter_button->Connect("Clicked()", "RemoteFileFilter", this, "ApplyFilter()");

    selectfiles_button->Connect("Clicked()", "RemoteFileFilter", this, "SelectFiles()");
    DisplayInListBox(this->table);

    selection_in_box = new TList;
    mf->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
}

void RemoteFileFilter::ApplyFilter() {
    available_files_box->RemoveAll();
    string module_str = module_dropdown->GetSelectedEntry()->GetTitle();
    string query_str = search_box->GetText();

    vector<string> filters;
    filters.push_back("_" + module_str + "_");
    filters.push_back(query_str);

    FileTable result = table.FilterNamesBy(filters);
    DisplayInListBox(result);
}

void RemoteFileFilter::FillFromFile(const string& filepath, bool cleanup) {
  
    if (cleanup)
        this->table.CleanEntries();
  
    std::ifstream in(filepath);
    char str[255];

    string filenamepath, displayname;
    while(in) {
        in.getline(str, 255);  // delim defaults to '\n'
        filenamepath=string(str);

        int beginIdx = filenamepath.rfind('/');
        displayname = filenamepath.substr(beginIdx + 1);

        // TODO: something fishy here, "" entry is added
        if(displayname.compare(""))
            this->table.AddEntry(displayname, filenamepath);
    }
    in.close();
}


void RemoteFileFilter::SelectFiles() {
    selection_in_box->Clear();
    available_files_box->GetSelectedEntries(selection_in_box);
//    selection_in_box->ls();

    FileTable selected_files;

    for(const auto&& obj: *selection_in_box) {
        string obj_name = obj->GetTitle();
        string val = table.GetPathFromName(obj_name);
//        cout << "found value " << val << endl;
        selected_files.AddEntry(obj_name, val); // val goes here if in real use
    }

    Emit("FilesSelected(int)", selected_files.GetMap().size());
}

void RemoteFileFilter::FillModuleFilters(const string& filepath, bool updateMode) {
    std::ifstream file(filepath);
    string line;
    int j = ((updateMode) ? module_dropdown->GetNumberOfEntries() + 1 : 1);
    
    while(std::getline(file, line))
    {
        if (line.length() != 0) {
            if (!updateMode || (updateMode && !module_dropdown->FindEntry(line.c_str())))
                module_dropdown->AddEntry(line.c_str(), j++);
        }        
    }
    
    file.close();
}

void RemoteFileFilter::DisplayInListBox(FileTable ftable) {
    available_files_box->RemoveAll();
    int i = 0;
    for(auto& elem : ftable.GetMap()) {
        available_files_box->AddEntry(elem.first.c_str(), i++);
    }
    available_files_box->Layout();
}
