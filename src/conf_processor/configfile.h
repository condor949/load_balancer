#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

struct ConfParameters
{
  int max_load;
  int input_port;
  std::vector<int> nodes;
  ConfParameters(int &&bvalue_t, int &&input_t, std::vector<int> &&nodes_t) : max_load(bvalue_t), input_port(input_t), nodes(nodes_t){};
};

class ConfigFile
{
public:
  ConfigFile(std::string filename);
  ConfigFile();
  bool load(std::string filename);

  ~ConfigFile();
  void print(void);

  template <typename T>
  T getvalue(std::string key)
  {
    std::string str = datamap[key];
    std::cout << str << std::endl;
    if (str == "")
    {
      std::cerr << "WARNING: '" << key << "' was not defined in " << filename << "! Value is undefined!" << std::endl;
    }
    std::stringstream ss;
    ss << str;
    T value;
    ss >> value;
    return value;
  }

  template <typename T>
  std::vector<T> getvector(std::string key)
  {
    std::string str = datamap[key];
    std::cout << str << std::endl;
    if (str == "")
    {
      std::cerr << "WARNING: '" << key << "' was not defined in " << filename << "! Value is undefined!" << std::endl;
    }
    std::stringstream ss(str);
    T el;
    std::vector<T> result;
    while (ss >> el)
      result.push_back(el);
    return result;
  }

  template <typename T>
  T getvalue(std::string key, T defaultValue)
  {
    std::string str = datamap[key];
    if (str == "")
    {
      return defaultValue;
    }
    return getvalue<T>(key);
  }

  template <typename T>
  T getvalueidx(std::string key, int idx, T defaultValue)
  {
    std::stringstream ss;
    ss << idx;
    std::string query = key + std::string("[") + ss.str() + std::string("]");
    return getvalue<T>(query, defaultValue);
  }

  template <typename T>
  T getvalueidx(std::string key, int idx)
  {
    std::stringstream ss;
    ss << idx;
    std::string query = key + std::string("[") + ss.str() + std::string("]");
    return getvalue<T>(query);
  }

private:
  std::map<std::string, std::string> datamap;
  std::string filename;
};

#endif // CONFIGFILE_H
