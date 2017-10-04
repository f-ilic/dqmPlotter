#ifndef FILETABLE__H_
#define FILETABLE__H_ 

#include "TGListBox.h"
#include <set>

class TGListBox;

class FileTable {
public:
    FileTable() {}
    void AddEntry(string k, string v);
    FileTable FilterNamesBy(vector<string> filters);

    string GetPathFromName(string name);
    string GetNameFromPath(string path);

    map<string, string> GetMap() { return table; }

    void PrintDebug();

private:
    map<string, string> table;          // key=displayname, value=path
};
#endif
