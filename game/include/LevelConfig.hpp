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
#include <vector>
#include <unordered_map>

class LevelConfig {

    public:

        explicit LevelConfig(const std::string &filename);
        ~LevelConfig();

        std::vector<std::pair<std::string, std::vector<std::string>>> parseLevelConfig();

    private:
        std::string _filename;
        std::map<int, std::string> _configData;

};

#endif /* !LEVELCONFIG_HPP_ */
