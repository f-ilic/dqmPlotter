#ifndef CONFIGURATION__H_
#define CONFIGURATION__H_

#include <fstream>
#include <map>

class Configuration final
{
  string config_path{};
  map<string, string> configMap;
  
  const string configTitles[6] = {"UserCertPath",
                                 "UserPublicKeyPath",
                                 "UpdateDataBaseScriptPath",
                                 "DatabaseCreation",
                                 "DatabasePath",
                                 "DatabaseFiltersPath"};
  
  public:    
  
    enum ConfigOpts{
      USERCERTPATH = 0,
      USERPUBLICKEYPATH,
      UPDATEDATABASESCRIPTPATH,
      DATABASECREATION,
      DATABASEPATH,
      DATABASEFILTERSPATH
    };
    
    const string& GetValue(ConfigOpts opt)
    {
      return configMap[string(configTitles[opt])];
    }
    
    friend ostream& operator<< (ostream& stream, const Configuration& config);
    
    static Configuration& GetConfiguration(string config_path = "DATA/con.fig")
    {
      static Configuration config(config_path);
      return config;
    }
    
    const string& UpdateKey(ConfigOpts opt, const string& val)
    {
      configMap[string(configTitles[opt])] = val;
      return val;
    }
    
  protected:
    Configuration(string config_path = "DATA/con.fig") : config_path(config_path)
    {
      LoadConfiguration();
    }
    
  private:
    void LoadConfiguration();
    
    // ClassDef(Configuration,1);
};

#endif