/*
** EPITECH PROJECT, 2024
** R-Type [WSL: Ubuntu]
** File description:
** LevelConfig
*/

#ifndef LEVELCONFIG_HPP_
#define LEVELCONFIG_HPP_

#include <fstream>
#include <iostream>
#include <map>

class LevelConfig {

    public:

        explicit LevelConfig(const std::string &filename);
        ~LevelConfig();

        const std::map<int, std::string> &getLevelConfig();

        void saveLevelConfig();
        void parseLevelConfig();
        void validateOrCreateLevelConfig();
        void updateLevelConfigValue(const std::string &key, const std::string &newValue);


    private:
        std::string _filename;
        std::map<int, std::string> _configData;

};

#endif /* !LEVELCONFIG_HPP_ */
