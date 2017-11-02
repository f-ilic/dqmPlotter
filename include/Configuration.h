#ifndef CONFIGURATION__H_
#define CONFIGURATION__H_

#include <fstream>
#include <map>


#define DEVMODE false
#define CONFIGPATH "DATA/con.fig"

//FIXME: make into good singleton
class Configuration {

public:
    enum ConfigOpts{
        USERCERTPATH = 0,
        USERPUBLICKEYPATH,
        UPDATEDATABASESCRIPTPATH,
        DATABASECREATION,
        DATABASEPATH,
        DATABASEFILTERSPATH,
        TMPDATADIRECTORY,
        LOCALCOPIES
    };
    
    const string& GetValue(ConfigOpts opt) {
        return config_map[string(config_titles[opt])];
    }
    
    friend ostream& operator<< (ostream& stream, const Configuration& config);
    
    static Configuration& Instance(string config_path = CONFIGPATH) {
        static Configuration config(config_path);
        return config;
    }
    
    const string& UpdateKey(ConfigOpts opt, const string& val) {
        config_map[string(config_titles[opt])] = val;
        return val;
    }
    

private:
    Configuration(string config_path = CONFIGPATH) : config_path(config_path) {
        LoadConfiguration();
    }

    void LoadConfiguration();

    string config_path{};
    map<string, string> config_map;

    const string config_titles[8] = {"UserCertPath",
                                     "UserPublicKeyPath",
                                     "UpdateDataBaseScriptPath",
                                     "DatabaseCreation",
                                     "DatabasePath",
                                     "DatabaseFiltersPath",
                                     "TmpDataDirectory",
                                     "LocalCopies"};
};

#endif
