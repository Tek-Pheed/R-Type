/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** Config
*/

#include "Config.hpp"

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
}

void Config::parseConfig()
{
    std::ifstream infile(_filename);

    if (!infile.is_open()) {
        throw std::runtime_error("Error: file not found");
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
