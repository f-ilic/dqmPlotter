#include "FileViewer.cpp"

class TGListBox;

class FileTable {
public:
    FileTable() {}
    string GetPathFromName(string name);
    string GetNameFromPath(string path);
    FileTable FilterNamesBy(vector<string> filters);
    void PrintDebug();
    void AddEntry(string k, string v);

    // more or less utility functions
    void FillFromFile(string path_to_file_to_load);
    void DisplayInListBox(TGListBox* listbox);
    void DisplayInTreeView(FileViewer& fileview);
    set<string> GetUniqueModulesFromFile(string filepath);


private:
    map<string, string> table;          // key=name, value=path
};
