#ifndef FILETABLE__H_
#define FILETABLE__H_ 

#include <iostream>
#include <map>


class FileTable {
public:
    FileTable() {}
    void AddEntry(const string& k, const string& v) { table[k] = v; };
    FileTable FilterNamesBy(const vector<string>& filters);

    const string& GetPathFromName(const string& name) { return table[name]; };
    // string GetNameFromPath(string path);

    const map<string, string>& GetMap() const { return table; }

    void PrintDebug() const;
    friend ostream& operator<< (ostream& stream, const FileTable& ft);
    
    void CleanEntries() { this->table.clear(); }

private:
    map<string, string> table;          // key=displayname, value=path
};
#endif
