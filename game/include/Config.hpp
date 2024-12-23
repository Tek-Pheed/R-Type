#ifndef CONFIGCLASS_HPP_
#define CONFIGCLASS_HPP_

#include <fstream>
#include <iostream>
#include <map>

class Config {
  public:
    explicit Config(const std::string &filename);
    ~Config();

    const std::map<int, std::string> &getConfig();
    void saveConfig();
    void parseConfig();
    void validateOrCreateConfig();
    void updateConfigValue(
        const std::string &key, const std::string &newValue);

  private:
    std::string _filename;
    std::map<int, std::string> _configData;
};

#endif /* !CONFIGCLASS_HPP_ */
