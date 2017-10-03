#include "FileTable.h"

R__LOAD_LIBRARY(lib/Configuration_cpp.so)
#include "Configuration.h"

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
        // TODO: something fishy here, "" entry is added
        if(displayname.compare(""))
            this->AddEntry(displayname, filenamepath);
    }
    in.close();
}

void FileTable::AddEntry(string k, string v) {
    table[k] = v;
}

void FileTable::DisplayInTreeView(FileViewer& fileview){
    fileview.RemoveAll();
    for(auto& e : table) {
        fileview.OpenFileInTreeView(e.second, e.first);
    }

}

set<string> FileTable::GetUniqueModulesFromFile(string filepath) {
    set<string> ret;
    std::ifstream in(filepath);

    char str[255];

    string filename;
    string modulename;

    while(in) {
        in.getline(str, 255);  // delim defaults to '\n'
        filename=string(str);
        int beginIdx = filename.rfind("/");
        filename = filename.substr(beginIdx + 1);

        // TODO: something fishy here, "" entry is added
        if(filename.compare("")) {
            filename = filename.substr(10, filename.size());
            modulename = filename.substr(0, filename.find("_"));
            ret.insert(modulename);
        }
    }
    in.close();
    return ret;
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


