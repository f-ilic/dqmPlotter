#include <fstream>
#include <map>

enum ConfigOpts{
  USERCERTPATH = 0,
  USERPUBLICKEYPATH,
  UPDATEDATABASESCRIPTPATH,
  DATABASECREATION,
  DATABASEPATH
};

class CConfiguration final
{
  string config_path{};
  map<string, string> configMap;
  
  const string configTitles[5] = {"UserCertPath",
                                 "UserPublicKeyPath",
                                 "UpdateDataBaseScriptPath",
                                 "DatabaseCreation",
                                 "DatabasePath"};
  
  public:    
    const string& GetValue(ConfigOpts opt)
    {
      return configMap[string(configTitles[opt])];
    }
    
    friend ostream& operator<< (ostream& stream, const CConfiguration& config);
    
    static CConfiguration& GetConfiguration(string config_path = "DATA/con.fig")
    {
      static CConfiguration config(config_path);
      return config;
    }
    
    void UpdateKey(ConfigOpts opt, const string& val)
    {
      configMap[string(configTitles[opt])] = val;
    }
    
  private:
    void LoadConfiguration();
    
  protected:
    CConfiguration(string config_path = "DATA/con.fig") : config_path(config_path)
    {
      LoadConfiguration();
    }
    
};

void CConfiguration::LoadConfiguration()
{
  std::ifstream file(config_path);
  string line;
  char key[256], val[256];
  
  while(std::getline(file, line))
  {          
    if (line.length() == 0 || line[0] == ' ' || line[0] == '#')
      continue;
    
    sscanf(line.c_str(), "%s = %s", key, val);
    
    configMap[string(key)] = string(val);
  }
  
  file.close();        
}

ostream& operator<< (ostream& stream, const CConfiguration& config)
{
  for (auto kv : config.configMap)
  {
    stream << kv.first << " : " << kv.second << endl;
  }
  
  return stream;
}

// void Configuration(){
  // cout << CConfiguration::GetConfiguration();
  // cout << "$$$$$$$$$$$$$$$$$$$$$$" << endl;
  
  // CConfiguration::GetConfiguration().UpdateKey(USERCERTPATH, "wtf");
  // cout << CConfiguration::GetConfiguration().GetValue(USERCERTPATH) << endl;
// }