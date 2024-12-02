#ifndef CONFIGCLASS_HPP_
#define CONFIGCLASS_HPP_

#include <iostream>

class Config {
    public:
        Config();
        ~Config();
        void getConfig();
        void setConfig(line: int, param: string);
        void saveConfig();
        void parseConfig();
    private:
};

#endif /* !CONFIGCLASS_HPP_ */
