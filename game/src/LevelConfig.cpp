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
    // validateOrCreateLevelConfig();
    parseLevelConfig();
}

LevelConfig::~LevelConfig()
{
}

const std::map<int, std::string> &LevelConfig::getLevelConfig()
{
    return _configData;
}

void LevelConfig::saveLevelConfig()
{
    if (_filename.find(".level") != _filename.size() - 4) {
        _filename += ".level";
    }

    std::ofstream savefile(_filename);

    if (!savefile.is_open()) {
        throw ErrorClass("RTC006 : Invalid file: file not found.");
    }
    if (_configData.empty()) {
        throw ErrorClass("RTC005 : Invalid config: the config file is empty.");
    }
    for (const auto &line : _configData) {
        savefile << line.second << std::endl;
    }

    savefile.close();
}

void LevelConfig::validateOrCreateLevelConfig()
{
    std::ifstream infile(_filename);

    if (!infile.is_open()) {
        std::ofstream newfile(_filename);
        if (!newfile.is_open()) {
            throw ErrorClass("RTC006 : Invalid file: could not create file.");
        }
        newfile << "NB_MAX_PLAYERS=4\n";
        newfile << "GAMEMODE=CLASSIC\n";
        newfile << "DIFFICULTY=NORMAL\n";
        newfile << "WAVES_BEFORE_BOSS=5\n";
        newfile << "BONUSES=false\n";
        newfile.close();
        return;
    }

    std::string line;
    std::vector<std::string> expectedKeys = {"NB_MAX_PLAYERS=", "GAMEMODE=",
        "DIFFICULTY=", "WAVES_BEFORE_BOSS=", "BONUSES="};
    size_t lineCount = 0;

    while (std::getline(infile, line)) {
        if (lineCount >= expectedKeys.size()
            || line.find(expectedKeys[lineCount]) != 0) {
            throw ErrorClass("RTC007 : Invalid config: file content does not "
                             "match expected format.");
        }

        if (lineCount == 0) {
            int maxPlayersNb =
                std::stoi(line.substr(expectedKeys[lineCount].size()));
            if (maxPlayersNb <= 0 || maxPlayersNb > 4) {
                throw ErrorClass("RTC013 : Invalid config: NB_MAX_PLAYERS "
                                 "must be between 1 and 4.");
            }
        } else if (lineCount == 1) {
            std::string gamemode = line.substr(expectedKeys[lineCount].size());
            if (gamemode != "CLASSIC" && gamemode != "PVP") {
                throw ErrorClass("RTC014 : Invalid config: GAMEMODE must be "
                                 "CLASSIC or PVP.");
            }
        } else if (lineCount == 2) {
            std::string difficulty =
                line.substr(expectedKeys[lineCount].size());
            if (difficulty != "EASY" && difficulty != "NORMAL"
                && difficulty != "HARD") {
                throw ErrorClass("RTC015 : Invalid config: DIFFICULTY must be "
                                 "EASY, NORMAL or HARD.");
            }
        } else if (lineCount == 3) {
            int wavesBeforeBoss =
                std::stoi(line.substr(expectedKeys[lineCount].size()));
            if (wavesBeforeBoss <= 0) {
                throw ErrorClass("RTC016 : Invalid config: WAVES_BEFORE_BOSS "
                                 "can't be 0 or less.");
            }
        } else if (lineCount == 4) {
            std::string bonuses = line.substr(expectedKeys[lineCount].size());
            if (bonuses != "true" && bonuses != "false") {
                throw ErrorClass(
                    "RTC017 : Invalid config: BONUSES must be true or false.");
            }
        }

        lineCount++;
    }

    if (lineCount != expectedKeys.size()) {
        throw ErrorClass(
            "RTC008 : Invalid config: file does not contain exactly 7 lines.");
    }

    infile.close();
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

void LevelConfig::updateLevelConfigValue(
    const std::string &key, const std::string &newValue)
{
    for (auto &line : _configData) {
        if (line.second.find(key + "=") == 0) {
            line.second = key + "=" + newValue;
            return;
        }
    }
    throw ErrorClass("RTC009 : Invalid config: key not found.");
}
