#include "../include/FileTable.h"

void FileTable::PrintDebug() const{
    for(const auto& e : table)
        cout << "key: " << e.first << "  value: " << e.second << endl;
}

ostream& operator<< (ostream& stream, const FileTable& ft)
{
    ft.PrintDebug();
    return stream;
}

FileTable FileTable::FilterNamesBy(const vector<string>& filters) {
    FileTable filtered;

    for(const auto& entry : table) {
        bool is_ok = true;
        for(const auto& f : filters){

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


