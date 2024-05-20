#ifndef TAPE_STORAGE_ICONFIG_H
#define TAPE_STORAGE_ICONFIG_H

#include <string>

class IConfig
{
public:

    virtual void readFromFile(const std::string& path) = 0;
    virtual ~IConfig() = default;
};

#endif // TAPE_STORAGE_ICONFIG_H
