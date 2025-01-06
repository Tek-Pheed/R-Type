/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu-24.04]
** File description:
** Config
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Config.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include "ErrorClass.hpp"

Config::Config(const std::string &filename) : _filename(filename)
{
    validateOrCreateConfig();
    parseConfig();
}

Config::~Config()
{
}

const std::map<int, std::string> &Config::getConfig()
{
    return _configData;
}

void Config::saveConfig()
{
    if (_filename.find(".cfg") != _filename.size() - 4) {
        _filename += ".cfg";
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

void Config::validateOrCreateConfig()
{
    std::ifstream infile(_filename);
    if (!infile.is_open()) {
        std::ofstream newfile(_filename);
        if (!newfile.is_open()) {
            throw ErrorClass("RTC006 : Invalid file: could not create file.");
        }
        newfile << "MOVE_UP=sf::Keyboard::Up\n";
        newfile << "MOVE_RIGHT=sf::Keyboard::Right\n";
        newfile << "MOVE_LEFT=sf::Keyboard::Left\n";
        newfile << "MOVE_DOWN=sf::Keyboard::Down\n";
        newfile << "SOUND_VOLUME=50\n";
        newfile << "RESOLUTION=1920x1080\n";
        newfile << "AUTO_FIRE=true\n";
        newfile.close();
        return;
    }

    std::string line;
    std::vector<std::string> expectedKeys = {
        "MOVE_UP=", "MOVE_RIGHT=", "MOVE_LEFT=", "MOVE_DOWN=", "SOUND_VOLUME=",
        "RESOLUTION=", "AUTO_FIRE="};
    size_t lineCount = 0;

    while (std::getline(infile, line)) {
        if (lineCount >= expectedKeys.size()
            || line.find(expectedKeys[lineCount]) != 0) {
            throw ErrorClass("RTC007 : Invalid config: file content does not "
                             "match expected format.");
        }

        if (lineCount == 4) {
            int volume =
                std::stoi(line.substr(expectedKeys[lineCount].size()));
            if (volume < 0 || volume > 100) {
                throw ErrorClass("RTC010 : Invalid config: SOUND_VOLUME must "
                                 "be between 0 and 100.");
            }
        } else if (lineCount == 5) {
            std::string resolution =
                line.substr(expectedKeys[lineCount].size());
            if (resolution != "1920x1080" && resolution != "3840x2160"
                && resolution != "2560x1440" && resolution != "1280x720") {
                throw ErrorClass(
                    "RTC011 : Invalid config: RESOLUTION must be 1920x1080 or "
                    "3840x2160 or 2560x1440 or 1280x720.");
            }
        } else if (lineCount == 6) {
            std::string autoFire = line.substr(expectedKeys[lineCount].size());
            if (autoFire != "true" && autoFire != "false") {
                throw ErrorClass("RTC012 : Invalid config: AUTO_FIRE must be "
                                 "true or false.");
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

void Config::parseConfig()
{
    std::ifstream infile(_filename);
    if (!infile.is_open()) {
        throw ErrorClass("RTC006 : Invalid file: could not open file.");
    }

    std::string line;
    int linenbr = 1;
    while (std::getline(infile, line)) {
        _configData[linenbr] = line;
        linenbr++;
    }
    infile.close();
}

void Config::updateConfigValue(
    const std::string &key, const std::string &newValue)
{
    for (auto &line : _configData) {
        if (line.second.find(key) == 0) {
            line.second = key + newValue;
            return;
        }
    }
    throw ErrorClass("RTC009 : Invalid config: key not found.");
}

std::string Config::getKeyFromConfig(const std::string &key)
{
    for (const auto &line : _configData) {
        if (line.second.find(key) == 0) {
            return line.second.substr(key.size() + 1);
        }
    }
    throw ErrorClass("RTC009 : Invalid config: key not found.");
}

bool Config::getAutoFireConfig()
{
    std::string autoFire = getKeyFromConfig("AUTO_FIRE");

    if (autoFire == "true") {
        return true;
    } else {
        return false;
    }
}
