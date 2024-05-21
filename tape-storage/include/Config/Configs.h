#ifndef TAPE_STORAGE_CONFIGS_H
#define TAPE_STORAGE_CONFIGS_H

#include "JsonConfig.h"

JSON_CONFIG(TapeConfig, "/tape")
{
    config["readDelay"] = 100;   // nanoseconds
    config["writeDelay"] = 100;  // nanoseconds
    config["rewindDelay"] = 250; // nanoseconds
    config["shiftDelay"] = 10;   // nanoseconds
}

JSON_CONFIG(SystemConfig, "/system")
{
    config["memory"] = 16; // bytes
    config["tmpTapes"] = 4;
}

#endif // TAPE_STORAGE_CONFIGS_H
