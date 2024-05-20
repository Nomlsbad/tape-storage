#ifndef TAPE_STORAGE_TAPE_HARDWARE_SIMULATOR_H
#define TAPE_STORAGE_TAPE_HARDWARE_SIMULATOR_H

#include "ITapeSimulator.h"

#include <chrono>
#include <string>

namespace YTape
{

class TapeHardwareSimulator: public ITapeSimulator
{
private:

    TapeHardwareSimulator();

    using TimeDuration = std::chrono::nanoseconds;
    static TimeDuration getConfigParam(const std::string& param);

public:

    static ITapeSimulator& getInstance();

public:

    void shift() override;
    void rewind(size_t n) override;
    void read() override;
    void write() override;

private:

    TimeDuration shiftDelay;
    TimeDuration rewindDelay;
    TimeDuration readDelay;
    TimeDuration writeDelay;
};

}

#endif // TAPE_STORAGE_TAPE_HARDWARE_SIMULATOR_H
