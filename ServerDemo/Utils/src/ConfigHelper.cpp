#include "ConfigHelper.h"
#include <fstream>
#include <iostream>

namespace utils
{

    bool ConfigHelper::ReadCfg(const std::string & filename)
    {
        std::ifstream file(filename);
        if(file.fail()) return false;

        std::string line;
        while(std::getline(file,line))
        {
            std::cout << line << std::endl;
            if(line == "") continue;
            if(line[0] == '[')
            {
                int size = line.find_first_of(']');
                std::string section = line.substr(1,size);
            }
        }
        return true;
    }
}