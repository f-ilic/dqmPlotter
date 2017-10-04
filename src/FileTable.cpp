#include "../include/FileTable.h"
#include <iostream>

string FileTable::GetPathFromName(string name) {
    return table[name];
}

void FileTable::AddEntry(string k, string v) {
    table[k] = v;
}

void FileTable::PrintDebug() {
    for(auto& e : table)
        cout << "key: " << e.first << "  value: " << e.second << endl;
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


