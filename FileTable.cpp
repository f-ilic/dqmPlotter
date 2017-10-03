#include "FileTable.h"

// #include "Configuration.cpp"

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

void FileTable::DisplayInTreeView(FileViewer& fileview){
    for(auto& e : table) {
        fileview.OpenFileInTreeView(e.second);
    }

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


