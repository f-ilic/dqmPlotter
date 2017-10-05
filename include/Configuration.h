#ifndef CONFIGURATION__H_
#define CONFIGURATION__H_

#include <fstream>
#include <map>

#define DEVMODE true
#define CONFIGPATH "DATA/con.fig"

class Configuration final {
public:
    enum ConfigOpts {
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
        return configMap[string(configTitles[opt])];
    }
    
    friend ostream& operator<< (ostream& stream, const Configuration& config);
    
    static Configuration& GetConfiguration(string config_path = CONFIGPATH) {
        static Configuration config(config_path);
        return config;
    }
    
    const string& UpdateKey(ConfigOpts opt, const string& val) {
        configMap[string(configTitles[opt])] = val;
        return val;
    }
    
private:
    Configuration(string config_path = CONFIGPATH) : config_path(config_path) {
        LoadConfiguration();
    }

    void LoadConfiguration();


    string config_path{};
    map<string, string> configMap;

    const string configTitles[8] = {"UserCertPath",
                                    "UserPublicKeyPath",
                                    "UpdateDataBaseScriptPath",
                                    "DatabaseCreation",
                                    "DatabasePath",
                                    "DatabaseFiltersPath",
                                    "TmpDataDirectory",
                                    "LocalCopies"};

};

#endif
