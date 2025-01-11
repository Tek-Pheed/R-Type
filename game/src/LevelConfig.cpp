/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu]
** File description:
** LevelConfig
*/

#include "LevelConfig.hpp"
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "ErrorClass.hpp"
#include "Factory.hpp"
#include "Game.hpp"
#include <unordered_map>

LevelConfig::LevelConfig(const std::string &filename) : _filename(filename)
{
}

LevelConfig::~LevelConfig()
{
}

std::vector<std::pair<std::string, std::vector<std::string>>>
LevelConfig::parseLevelConfig()
{
    std::ifstream infile(_filename);
    if (!infile.is_open()) {
        throw ErrorClass("RTC006 : Invalid file: could not open file.");
    }
    std::vector<std::pair<std::string, std::vector<std::string>>> map;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.size() > 0 && line[0] != '#') {
            if (line.find(')') == std::string::npos
                || line.find('(') == std::string::npos) {
                throw ErrorClass(
                    "RTC008 : Invalid config: token ) or ( not found.");
            }
            std::stringstream sline;
            sline << line;

            std::string cmd;
            std::vector<std::string> params;
            std::getline(sline, cmd, '(');

            sline.seekp(0, std::ios_base::end);
            std::string paramsStr;
            std::getline(sline, paramsStr, ')');
            std::string p;
            sline << paramsStr;
            while (std::getline(sline, p, ',')) {
                p.erase(std::remove_if(p.begin(), p.end(),
                            [](char c) {
                                return c == '(' || c == ')' || c == ',';
                            }),
                    p.end());
                params.emplace_back(p);
            }
            map.emplace_back(std::make_pair(cmd, params));
        }
    }
    infile.close();
    return (map);
}
