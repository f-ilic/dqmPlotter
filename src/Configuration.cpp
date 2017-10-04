#include "../include/Configuration.h"

void Configuration::LoadConfiguration()
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

ostream& operator<< (ostream& stream, const Configuration& config)
{
  for (auto kv : config.configMap)
  {
    stream << kv.first << " : " << kv.second << endl;
  }
  
  return stream;
}
