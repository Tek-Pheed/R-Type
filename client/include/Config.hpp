#ifndef CONFIGCLASS_HPP_
#define CONFIGCLASS_HPP_

#include <iostream>
#include <fstream>
#include <map>

class Config {
    public:
        Config(const std::string &filename);
        ~Config();
        const std::map<int, std::string>& getConfig();
        void setConfig(int line, const std::string &param);
        void saveConfig();
        void parseConfig();
    private:
        std::string _filename;
        std::map<int, std::string> _configData;

};

#endif /* !CONFIGCLASS_HPP_ */
