#include <vector>
#include <map>

class TGListBox;

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
