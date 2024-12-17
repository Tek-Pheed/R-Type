/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** Config
*/
 
#include "Config.hpp"
#include "ErrorClass.hpp"
 
Config::Config(const std::string &filename) : _filename(filename)
{
    parseConfig();
}
 
Config::~Config()
{
}
 
const std::map<int, std::string>& Config::getConfig()
{
    return _configData;
}
 
void Config::setConfig(int line, const std::string &param)
{
    _configData[line] = param;
}
 
void Config::saveConfig()
{
     if (_filename.find(".cfg") != _filename.size() - 4) {
        _filename += ".cfg";
    }
 
    std::ofstream savefile(_filename);
 
    if (!savefile.is_open()) {
        throw ErrorClass(
            "RTC006 : Invalid file: file not found.");
    }
    if (_configData.empty()) {
        throw ErrorClass(
            "RTC005 : Invalid config: the config file is empty.");
    }
    for (const auto &line : _configData) {
        savefile << line.second << std::endl;
    }
 
    savefile.close();
}
 
void Config::parseConfig()
{
    std::ifstream infile(_filename);
    if (!infile.is_open()) {
        std::ofstream newfile(_filename);
        if (!newfile.is_open()) {
            throw ErrorClass(
                "RTC006 : Invalid file: could not create file.");
        }
        newfile << "MOVE_UP=ARROW_UP\n";
        newfile << "MOVE_RIGHT=ARROW_RIGHT\n";
        newfile << "MOVE_LEFT=ARROW_LEFT\n";
        newfile << "MOVE_DOWN=ARROW_DOWN\n";
        newfile << "SOUND_VOLUME=50\n";
        newfile << "RESOLUTION=1920,1080\n";
        newfile << "AUTO_FIRE=ON\n";
        newfile << "FULLSCREEN=ON\n";
        newfile.close();
        return;
    }
    std::string line;
    int linenbr = 1;
 
    while (std::getline(infile, line)) {
        _configData[linenbr] = line;
        linenbr++;
    }
    infile.close();
}
 